//
//  DataCostsExtractor.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 19/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "DataCostsExtractor.hpp"
#include <iostream>
#include <algorithm>
#include "Utils.h"
#include "Arguments.h"

DataCostsExtractor::DataCostsExtractor(const Mesh & mesh, View & view) : mesh(mesh), view(view){
    width  = view.photoWidth;
    height = view.photoHeight;
    depthBuffer.resize(width*height);
    idBuffer.resize(width*height);
    transformation.setCamera(view.camera);
    transformation.setAspectRatio(width, height);
    view.loadImage();
    sobelImage = view.sourceImage->toSobel();
    sourceImage = view.sourceImage;
    clearBuffer();
}

DataCostsExtractor::~DataCostsExtractor(){
    view.releaseImage();
}

void
DataCostsExtractor::traversePartition(const PartitionNode * node){
    if(node == nullptr)
        return;
    if(!isInsideViewFrustrum(node->boundingBox, transformation))
         return;
    if(node->direction == NONE){ //node has no children
        for(auto triangle : node->triangles){
            processTriangle(mesh.triangles.at(triangle));
        }
        return;
    }
    traversePartition(node->leftNode);
    traversePartition(node->rightNode);
    
}

std::map<uint,PatchQuality> DataCostsExtractor::calculateCosts(){

    for(auto & o: mesh.objects){
        traversePartition(&o.partitionRoot);
    }

    std::map<uint,PatchQuality> costs;
    for(auto i : patchInfos){
        PatchQuality info = i.second;
        if(info.sampleCount != 0 ){
            if(arguments.strictOclusionCheck && info.sampleCount==info.potentialSampleCount)
                continue;
            costs[i.first] = info;
        }
    }
    return costs;
}


void DataCostsExtractor::processTriangle(const Triangle & triangle){
    Vertex vOne   = mesh.verticies.at(triangle.verticies[0]);
    Vertex vTwo   = mesh.verticies.at(triangle.verticies[1]);
    Vertex vThree = mesh.verticies.at(triangle.verticies[2]);
    
    vOne.texCoord   = mesh.texCoords.at(triangle.texCoords.at(vOne.id  )).coord;
    vTwo.texCoord   = mesh.texCoords.at(triangle.texCoords.at(vTwo.id  )).coord;
    vThree.texCoord = mesh.texCoords.at(triangle.texCoords.at(vThree.id)).coord;
    
    glm::mat4 cameraModelTransform = transformation.getViewProjection()*
    transformation.getModelMatrix();
    
    vOne =   cameraModelTransform * vOne;
    vTwo =   cameraModelTransform * vTwo;
    vThree = cameraModelTransform * vThree;
    
    if(isInsideViewFrustrum(vOne) && isInsideViewFrustrum(vTwo) && isInsideViewFrustrum(vThree)){
        rasterizeTriangle(vOne, vTwo, vThree, triangle);
        return;
    }
}


void DataCostsExtractor::rasterizeTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle){
    glm::mat4 screenSpaceTransform = transformation.getScreenTransform();
    
    minYVert = transformation.doPerspectiveDevide(screenSpaceTransform * minYVert);
    midYVert = transformation.doPerspectiveDevide(screenSpaceTransform * midYVert);
    maxYVert = transformation.doPerspectiveDevide(screenSpaceTransform * maxYVert);
    
    uint id = triangle.id;
    
    if (triangleArea(minYVert, maxYVert, midYVert) <= 0){
        id = 0;
    }
    
    if( (minYVert.z()<-minYVert.w() || minYVert.z()>minYVert.w()) &&
       (midYVert.z()<-midYVert.w() || midYVert.z()>midYVert.w()) &&
       (maxYVert.z()<-maxYVert.w() || maxYVert.z()>maxYVert.w())    ){
        return;
    }
    
    if(minYVert.y()>midYVert.y())
        std::swap(minYVert, midYVert);
    if(midYVert.y()>maxYVert.y())
        std::swap(maxYVert, midYVert);
    if(minYVert.y()>midYVert.y())
        std::swap(minYVert, midYVert);
    
    float area = triangleArea(minYVert, maxYVert, midYVert);
    
    bool handedness = (area >= 0);
    
    fillTriangle(minYVert, midYVert, maxYVert, handedness, id);
}


void DataCostsExtractor::fillTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, uint id){
    
    Gradient gradient(minYVert,midYVert,maxYVert);
    
    Edge topToBottom(minYVert, maxYVert, gradient, 0);
    Edge topToMiddle(minYVert, midYVert, gradient, 0);
    Edge middleToBottom(midYVert, maxYVert, gradient, 1);
    
    
    Edge * left = &topToBottom;
    Edge * right = &topToMiddle;
    
    if(handedness)
        std::swap(left,right);
    
    int yStart = topToMiddle.yStart;
    int yEnd   = topToMiddle.yEnd;
    
    for(int j = yStart; j<yEnd; j++){
        drawScanLine(*left,*right,j,gradient,id);
        left->Step();
        right->Step();
    }
    
    left = & topToBottom;
    right = & middleToBottom;
    
    if(handedness)
        std::swap(left,right);
    
    yStart =  middleToBottom.yStart;
    yEnd   =  middleToBottom.yEnd;
    
    for(int j = yStart; j<yEnd; j++){
        drawScanLine(*left,*right,j,gradient,id);
        left->Step();
        right->Step();
    }
    
}


void DataCostsExtractor::drawScanLine(Edge left, Edge right, int y, Gradient & gradient, uint id){
    int xMin = (int)ceil(left.currentX);
    int xMax = (int)ceil(right.currentX);
    float xPrestep = xMin - left.currentX;
    float xDist = right.currentX - left.currentX;
    
    //depth
    float depthXStep = (right.depth - left.depth)/xDist;
    float depth = left.depth + depthXStep*xPrestep;
    
    for(int x = xMin; x<xMax ; x++){
        int index = x + y*width;
        patchInfos[id].potentialSampleCount++;
        if(depth < depthBuffer[index]){
            glm::vec4 color = sobelImage.at(x, y);
            glm::vec4 originColor = sourceImage->at(x,y);
            
            if(idBuffer[index]!=0){
                uint previousID = idBuffer[index];
                patchInfos[previousID].gradientMagnitudeSum -= color[0];
                patchInfos[previousID].sampleCount--;
                patchInfos[previousID].colorSum -= originColor;
            }
            
            if(id != 0){
                patchInfos[id].sampleCount++;
                patchInfos[id].gradientMagnitudeSum += color[0];

                patchInfos[id].colorSum += originColor;
            }
            
            idBuffer[index] = id;
            depthBuffer[index] = depth;
        }
        depth += depthXStep;
    }
}


float DataCostsExtractor::triangleArea(Vertex v1, Vertex v2, Vertex v3){
    glm::vec4 a = v1.coord;
    glm::vec4 b = v2.coord;
    glm::vec4 c = v3.coord;
    
    glm::vec4 p1;
    p1.x = b.x - a.x;
    p1.y = b.y - a.y;
    
    glm::vec4 p2;
    p2.x = c.x - a.x;
    p2.y = c.y - a.y;
    
    return ( p1.x * p2.y - p2.x * p1.y);
}


void DataCostsExtractor::clearBuffer(){
    std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::max());
    std::fill(idBuffer.begin(), idBuffer.end(), 0);
}



//
//  Rasterizator.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//
#include "Rasterizer.hpp"
#include <iostream>
#include <algorithm>
#include "Utils.h"
bool useColor = false;

Rasterizer::Rasterizer(int width, int height){
    this->width  = width;
    this->height = height;
    depthBuffer.resize(width*height);
    scoreTable.resize(width*height);
    idBuffer.resize(width*height);
}

void Rasterizer::setRenderContext (RenderContext * rc) {
    context = rc;
    width = rc->width;
    height = rc->height;
    depthBuffer.resize(width*height);
    scoreTable.resize(width*height);

}

void Rasterizer::drawMesh(){
    for(auto triangle : mesh.triangles){
        drawTriangle(triangle.second);
    }
}

void Rasterizer::drawTriangle(const Triangle & triangle){
    Vertex vOne = mesh.verticies.at(triangle.verticies[0]);
    Vertex vTwo = mesh.verticies.at(triangle.verticies[1]);
    Vertex vThree = mesh.verticies.at(triangle.verticies[2]);
    
    vOne.texCoord = mesh.texCoords.at(triangle.texCoords.at(vOne.id)).coord;
    vTwo.texCoord = mesh.texCoords.at(triangle.texCoords.at(vTwo.id)).coord;
    vThree.texCoord = mesh.texCoords.at(triangle.texCoords.at(vThree.id)).coord;
    
    //model camera check clip
    
//    printMatrix(transformation.getViewProjection());
//    printMatrix(transformation.getModelMatrix());

    
    glm::mat4 cameraModelTransform = transformation.getViewProjection()*
                                     transformation.getModelMatrix();
    
//    glm::mat4 cameraModelTransform = transformation.getViewProjection();
    
//    printMatrix(cameraModelTransform);
    
    vOne =   cameraModelTransform * vOne;
    vTwo =   cameraModelTransform * vTwo;
    vThree = cameraModelTransform * vThree;
    
    if(isInsideViewFrustrum(vOne) && isInsideViewFrustrum(vTwo) && isInsideViewFrustrum(vThree)){
        rasterizeTriangle(vOne, vTwo, vThree, triangle);
        return;
    }
        
    if(!isInsideViewFrustrum(vOne) && !isInsideViewFrustrum(vTwo) && !isInsideViewFrustrum(vThree)){
        return;
    }

    std::vector<Vertex> verticies;
    verticies.push_back(vOne);
    verticies.push_back(vTwo);
    verticies.push_back(vThree);
    
    if(clipPoligonAxis(verticies, 0) &&
       clipPoligonAxis(verticies, 1) &&
       clipPoligonAxis(verticies, 2)){
        
        Vertex initVertex = *verticies.begin();
        for(int i = 1; i<(verticies.size()-1); i++ ){
            rasterizeTriangle(initVertex, verticies.at(i), verticies.at(i+1), triangle);
        }
        
    }
    
}

bool Rasterizer::clipPoligonAxis (std::vector<Vertex> & verticies, int component){
    std::vector<Vertex> result;
    clipPoligonComponent(verticies, result, component, 1);
    verticies.clear();
    if(result.empty()){
        return false;
    }
    clipPoligonComponent(result, verticies, component, -1);
    return !verticies.empty();
}

void Rasterizer::clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor){
    
    Vertex prevVertex = verticies.back();
    float prevComponent = prevVertex.get(component) * clipFactor;
    bool prevInside = prevComponent <= prevVertex.w();
    
    for(auto currentVertex = verticies.begin() ; currentVertex< verticies.end();currentVertex++){
        float currentComponent = currentVertex->get(component) * clipFactor;
        bool currentInside = currentComponent <= currentVertex->w();
        
        if(currentInside ^ prevInside){
            float lerpAmmount = (prevVertex.w() - prevComponent) /
            ((prevVertex.w() - prevComponent) - ( currentVertex->w() - currentComponent));
            
            result.push_back(prevVertex.lerp(*currentVertex, lerpAmmount));
        }
        if(currentInside){
            result.push_back(*currentVertex);
        }
        prevVertex = *currentVertex;
        prevComponent = currentComponent;
        prevInside = currentInside;
    }
    
}

void Rasterizer::rasterizeTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle){
    glm::mat4 screenSpaceTransform = transformation.getScreenTransform();
    
    minYVert = transformation.doPerspectiveDevide(screenSpaceTransform * minYVert);
    midYVert = transformation.doPerspectiveDevide(screenSpaceTransform * midYVert);
    maxYVert = transformation.doPerspectiveDevide(screenSpaceTransform * maxYVert);
    
    if (triangleArea(minYVert, maxYVert, midYVert) <= 0){
        return; //BackFace culling
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
    
    fillTriangle(minYVert, midYVert, maxYVert, handedness, triangle);
}


void Rasterizer::fillTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness,const Triangle & triangle){
    
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
    
    for(int j = yStart; j< yEnd; j++){
        drawScanLine(*left,*right,j,gradient,triangle.id);
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
        drawScanLine(*left,*right,j,gradient,triangle.id);
        left->Step();
        right->Step();
    }
    
}


void Rasterizer::drawScanLine(Edge left, Edge right, int y, Gradient & gradient, uint id){
    int xMin = (int)ceil(left.currentX);
    int xMax = (int)ceil(right.currentX);
    float xPrestep = xMin - left.currentX;
    float xDist = right.currentX - left.currentX;
    
    //texture
    float texCoordXXStep = (right.texCoord.x - left.texCoord.x)/xDist;
    float texCoordYXStep = (right.texCoord.y - left.texCoord.y)/xDist;
    glm::vec2 texCoord;
    texCoord.x = left.texCoord.x + texCoordXXStep*xPrestep;
    texCoord.y = left.texCoord.y + texCoordYXStep*xPrestep;
    
    //overZ
    float oneOverZXStep = (right.oneOverZ - left.oneOverZ)/xDist;
    float oneOverZ = left.oneOverZ + oneOverZXStep*xPrestep;
    
    //depth
    float depthXStep = (right.depth - left.depth)/xDist;
    float depth = left.depth + depthXStep*xPrestep;
    
    
    for(int x = xMin; x<xMax ; x++){
//        glm::vec4 lerpedColor = glm::mix(minColor, maxColor, lerpAmmount);
//        clampRGBA(lerpedColor);
//        lerpAmmount += lerpStep;
        
        int index = x + y*width;
        if(depth < depthBuffer[index]){
        
            if(idBuffer[index]!=0)
                visibleFaces.erase(idBuffer[index]);
            visibleFaces.insert(id);
            idBuffer[index] = id;
            
            if(depth < minDepth)
                minDepth = depth;
            if(depth > maxDepth)
                maxDepth = depth;

            depthBuffer[index] = depth;
            scoreTable[index] = std::pair<uint,float>(id,1.0f/depth);
            float z = 1.0f / oneOverZ; //OMG!!!!
            int srcX = (int)((texCoord.x * z) * (texture->width - 1) + 0.5f);
            int srcY = (int)((texCoord.y * z) * (texture->height - 1) + 0.5f);
            
            context -> putPixel(x, y, texture->at(srcX, srcY));

        }else{
            visibleFaces.erase(idBuffer[index]);
        }
    
        texCoord.x += texCoordXXStep;
        texCoord.y += texCoordYXStep;
        oneOverZ   += oneOverZXStep;
        depth += depthXStep;
    }
}


float Rasterizer::triangleArea(Vertex v1, Vertex v2, Vertex v3){
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


void Rasterizer::clearBuffer(){
    std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::max());
    std::fill(idBuffer.begin(), idBuffer.end(), 0);
    std::fill(scoreTable.begin(), scoreTable.end(),std::pair<uint,float>(0,1));
    minDepth = std::numeric_limits<float>::max();
    maxDepth = std::numeric_limits<float>::min();
    visibleFaces.clear();
}


bool Rasterizer::isInsideViewFrustrum (Vertex v){
    return (abs(v.x()) <= abs(v.w())) &&
           (abs(v.y()) <= abs(v.w())) &&
           (abs(v.z()) <= abs(v.w()));
}


void Rasterizer::_getDepthBitmap(Bitmap & bitmap){
    bitmap = Bitmap(width,height);
    std::cout<<"Getting grayscale\n";
    float grayFactor = 1/(maxDepth - minDepth);
    for(int x = 0; x<width; x++){
        for(int y = 0; y<height; y++){
            int index = x + y*width;
            if(depthBuffer[index] == std::numeric_limits<float>::max()){
                bitmap.putPixel(x, y, glm::vec4(0.2,0.1,0.5,1));
            }else{
                float c = (depthBuffer[index]-minDepth)*grayFactor;
                bitmap.putPixel(x, y, glm::vec4(c,c,c,1));
            }
        }
    }
}






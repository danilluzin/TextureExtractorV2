//
//  ExtractionWorker_extend.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 25/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "ExtractionWorker.hpp"
#include <glm/ext.hpp>
#include "Utils.h"

void ExtractionWorker::extend(Triangle face,Bitmap & tex){
    this->texture = &tex;
    extendTriangle(face);
    this->texture = nullptr;
}

void ExtractionWorker::extendTriangle(Triangle triangle){
    Vertex minYVert = mesh.verticies.at(triangle.verticies[0]);
    Vertex midYVert = mesh.verticies.at(triangle.verticies[1]);
    Vertex maxYVert = mesh.verticies.at(triangle.verticies[2]);
    
    minYVert.texCoord = mesh.texCoords.at(triangle.texCoords.at(minYVert.id)).coord;
    midYVert.texCoord = mesh.texCoords.at(triangle.texCoords.at(midYVert.id)).coord;
    maxYVert.texCoord = mesh.texCoords.at(triangle.texCoords.at(maxYVert.id)).coord;
    
    if(minYVert.texCoord.y>midYVert.texCoord.y)
        std::swap(minYVert, midYVert);
    if(midYVert.texCoord.y > maxYVert.texCoord.y)
        std::swap(maxYVert, midYVert);
    if(minYVert.texCoord.y>midYVert.texCoord.y)
        std::swap(minYVert, midYVert);
    
    minYVert.texCoord.x = minYVert.texCoord.x*(texture->width - 1);
    minYVert.texCoord.y = minYVert.texCoord.y*(texture->height - 1);
    
    midYVert.texCoord.x = midYVert.texCoord.x*(texture->width - 1);
    midYVert.texCoord.y = midYVert.texCoord.y*(texture->height - 1);
    
    maxYVert.texCoord.x = maxYVert.texCoord.x*(texture->width - 1);
    maxYVert.texCoord.y = maxYVert.texCoord.y*(texture->height - 1);
    
    float area = triangleAreaTexture(minYVert, maxYVert, midYVert);
    bool handedness = (area >= 0);
    
    
    int dist = 4;
    Vertex minYVert2;
    Vertex maxYVert2;
    Vertex midYVert2;
    //    //// 1
    TextureEdge topToBottom(minYVert, maxYVert);
    minYVert2 = minYVert;
    maxYVert2 = maxYVert;
    if(handedness){
        //ttb is right;
        minYVert2.texCoord.x += dist;
        maxYVert2.texCoord.x += dist;
    }else{
        minYVert2.texCoord.x -= dist;
        maxYVert2.texCoord.x -= dist;
    }
    TextureEdge topToBottom2(minYVert2, maxYVert2);
    if(handedness){
        fillBetweenEdges(topToBottom, topToBottom2, true);
    }else{
        fillBetweenEdges(topToBottom2, topToBottom, false);
    }
    
    //// 2
    TextureEdge topToMiddle(minYVert, midYVert);
    minYVert2 = minYVert;
    midYVert2 = midYVert;
    if(handedness){
        //ttb is right;
        midYVert2.texCoord.x -= dist;
        minYVert2.texCoord.x -= dist;
    }else{
        midYVert2.texCoord.x += dist;
        minYVert2.texCoord.x += dist;
    }
    TextureEdge topToMiddle2(minYVert2, midYVert2);
    if(handedness){
        fillBetweenEdges(topToMiddle2, topToMiddle, false);
    }else{
        fillBetweenEdges(topToMiddle, topToMiddle2, true);
    }
    ///3
    TextureEdge middleToBottom(midYVert, maxYVert);
    maxYVert2 = maxYVert;
    midYVert2 = midYVert;
    if(handedness){
        midYVert2.texCoord.x -= dist;
        maxYVert2.texCoord.x -= dist;
    }else{
        midYVert2.texCoord.x += dist;
        maxYVert2.texCoord.x += dist;
    }
    TextureEdge middleToBottom2(midYVert2, maxYVert2);
    if(handedness){
        fillBetweenEdges(middleToBottom2, middleToBottom, false);
    }else{
        fillBetweenEdges(middleToBottom, middleToBottom2, true);
    }
}



//func pointer
void ExtractionWorker::fillBetweenEdges(TextureEdge left, TextureEdge right, bool leftIsSample){
    int yStart = left.yStart;
    int yEnd   = left.yEnd;
    for(int j = yStart; j< yEnd; j++){
        glm::vec4 color;
        if(leftIsSample){
            color= texture->at((int)floor(left.currentX),j);
        }else{
            color= texture->at((int)floor(right.currentX),j);
        }
        //        color = glm::vec4(0,1,0.2,1);
        drawScanLineColorNoMask(left,right,j,color);
        left.Step();
        right.Step();
    }
}


void ExtractionWorker::drawScanLineColorNoMask(TextureEdge left, TextureEdge right, int y, glm::vec4 color){
    int xMin = (int)ceil(left.currentX);
    int xMax = (int)ceil(right.currentX);
    for(int x = xMin; x<xMax ; x++){
        if(mask->at(x,y)!=glm::vec4(1,1,1,1))
            texture->putPixel(x, y, color);
    }
}


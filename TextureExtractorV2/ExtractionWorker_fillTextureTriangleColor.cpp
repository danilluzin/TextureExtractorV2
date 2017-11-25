//
//  ExtractionWorker_fillTriangleColor.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 25/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "ExtractionWorker.hpp"
#include <glm/ext.hpp>
#include "Utils.h"


void ExtractionWorker::fillTextureTriangle(Triangle face,glm::vec4 color, Bitmap & destination){
    texture = &destination;
    
    Vertex minYVert = mesh.verticies.at(face.verticies[0]);
    Vertex midYVert = mesh.verticies.at(face.verticies[1]);
    Vertex maxYVert = mesh.verticies.at(face.verticies[2]);
    
    minYVert.texCoord = mesh.texCoords.at(face.texCoords.at(minYVert.id)).coord;
    midYVert.texCoord = mesh.texCoords.at(face.texCoords.at(midYVert.id)).coord;
    maxYVert.texCoord = mesh.texCoords.at(face.texCoords.at(maxYVert.id)).coord;
    
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
    
    fillTriangle(minYVert, midYVert, maxYVert, handedness, color);
    texture = nullptr;
}


void ExtractionWorker::fillTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness,glm::vec4 color){
    
    TextureEdge topToBottom(minYVert, maxYVert);
    TextureEdge topToMiddle(minYVert, midYVert);
    TextureEdge middleToBottom(midYVert, maxYVert);
    
    TextureEdge * left = &topToBottom;
    TextureEdge * right = &topToMiddle;
    
    if(handedness)
        std::swap(left,right);
    
    int yStart = topToMiddle.yStart;
    int yEnd   = topToMiddle.yEnd;
    
    for(int j = yStart; j< yEnd; j++){
        drawScanLineColor(*left,*right,j,color);
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
        drawScanLineColor(*left,*right,j,color);
        left->Step();
        right->Step();
    }
}


void ExtractionWorker::drawScanLineColor(TextureEdge left, TextureEdge right, int y, glm::vec4 color){
    int xMin = (int)ceil(left.currentX);
    int xMax = (int)ceil(right.currentX);
    for(int x = xMin; x<xMax ; x++){
        texture->putPixel(x, y, color);
    }
}







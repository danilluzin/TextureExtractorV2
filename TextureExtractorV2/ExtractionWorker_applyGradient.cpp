//
//  ExtractionWorker_applyGradient.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 25/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "ExtractionWorker.hpp"
#include <glm/ext.hpp>
#include "Utils.h"

void ExtractionWorker::applyGradient(Triangle face,Bitmap & src,Bitmap & dst, glm::vec4 color [3],Bitmap & levelingTexture){
    this->texture = &dst;
    this->source = &src;
    this->levelingTexture = &levelingTexture;
    proccesGradientTriangle(face,color);
    this->texture = nullptr;
    this->source = nullptr;
    this->levelingTexture = nullptr;
}

void ExtractionWorker::proccesGradientTriangle(const Triangle & triangle, glm::vec4 color [3]){
    Vertex minYVert = mesh->verticies.at(triangle.verticies[0]);
    Vertex midYVert = mesh->verticies.at(triangle.verticies[1]);
    Vertex maxYVert = mesh->verticies.at(triangle.verticies[2]);
    
    minYVert.texCoord = mesh->texCoords.at(triangle.texCoords.at(minYVert.id)).coord;
    midYVert.texCoord = mesh->texCoords.at(triangle.texCoords.at(midYVert.id)).coord;
    maxYVert.texCoord = mesh->texCoords.at(triangle.texCoords.at(maxYVert.id)).coord;

    if(minYVert.texCoord.y>midYVert.texCoord.y){
        std::swap(minYVert, midYVert);
        std::swap(color[0], color[1]);
    }
    if(midYVert.texCoord.y>maxYVert.texCoord.y){
        std::swap(maxYVert, midYVert);
        std::swap(color[2], color[1]);
    }
    if(minYVert.texCoord.y>midYVert.texCoord.y){
        std::swap(minYVert, midYVert);
        std::swap(color[0], color[1]);
    }
    
    minYVert.texCoord.x = minYVert.texCoord.x*(texture->width - 1);
    minYVert.texCoord.y = minYVert.texCoord.y*(texture->height - 1);
    
    midYVert.texCoord.x = midYVert.texCoord.x*(texture->width - 1);
    midYVert.texCoord.y = midYVert.texCoord.y*(texture->height - 1);
    
    maxYVert.texCoord.x = maxYVert.texCoord.x*(texture->width - 1);
    maxYVert.texCoord.y = maxYVert.texCoord.y*(texture->height - 1);
    
    float area = triangleAreaTexture(minYVert, maxYVert, midYVert);
    
    bool handedness = (area >= 0);
    
    fillTriangleGradient(minYVert, midYVert, maxYVert, handedness, triangle, color);
}

void ExtractionWorker::fillTriangleGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness,const Triangle & triangle,glm::vec4 color [3]){
    TextureGradient gradient(minYVert,midYVert,maxYVert,color);

    TextureEdge topToBottom(minYVert, maxYVert, gradient, 0);
    TextureEdge topToMiddle(minYVert, midYVert, gradient, 0);
    TextureEdge middleToBottom(midYVert, maxYVert, gradient, 1);


    TextureEdge * left = &topToBottom;
    TextureEdge * right = &topToMiddle;

    if(handedness)
    std::swap(left,right);

    int yStart = topToMiddle.yStart;
    int yEnd   = topToMiddle.yEnd;

    for(int j = yStart; j< yEnd; j++){
        addGradientLine(*left,*right,j,gradient,triangle.id);
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
         addGradientLine(*left,*right,j,gradient,triangle.id);
        left->Step();
        right->Step();
    }
}


void ExtractionWorker::addGradientLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient, uint id){
    int xMin = (int)floor(left.currentX);
    int xMax = (int)ceil(right.currentX);
    float xPrestep = xMin - left.currentX;
    
    //color
    glm::vec4 minColor = left.color + gradient.colorXStep*xPrestep;
    glm::vec4 maxColor = right.color + gradient.colorYStep*xPrestep;
    double lerpAmmount = 0;
    double lerpStep = 1.0f/(xMax - xMin);
    
    for(int x = xMin; x<xMax ; x++){
        glm::vec4 lerpedColor = glm::mix(minColor, maxColor, lerpAmmount);
        lerpAmmount += lerpStep;
        glm::vec4 gradientColor =  source->at(x, y);
        gradientColor += lerpedColor;
        clampRGBA(gradientColor);
        texture->putPixel(x, y, gradientColor);
        if(levelingTexture){
            glm::vec4 c = levelingTexture->at(x,y);
            glm::vec4 levelColor = c + lerpedColor;
            clampRGBA(levelColor);
            levelingTexture->putPixel(x, y, levelColor);
        }
    }
    
}



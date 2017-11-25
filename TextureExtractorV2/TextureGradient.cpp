//
//  Gradient.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "TextureGradient.hpp"
#include <algorithm>
TextureGradient::TextureGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert){
    
//    float dx =(midYVert.x()-maxYVert.x())*(minYVert.y()-maxYVert.y()) -
//              (midYVert.y()-maxYVert.y())*(minYVert.x()-maxYVert.x());
//    float dy = -dx;
    
    float oneOverdx = 1.0f/((midYVert.texCoord.x-maxYVert.texCoord.x)*(minYVert.texCoord.y-maxYVert.texCoord.y) -
                      (midYVert.texCoord.y-maxYVert.texCoord.y)*(minYVert.texCoord.x-maxYVert.texCoord.x));
    float oneOverdy = -oneOverdx;
    
    
//    //Color
//    color[0] = minYVert.color;
//    color[1] = midYVert.color;
//    color[2] = maxYVert.color;
//
//    glm::vec4 dColorX =(color[1]-color[2])*(minYVert.texCoord.y-maxYVert.texCoord.y) -
//    (color[0]-color[2])*(midYVert.texCoord.y-maxYVert.texCoord.y);
//
//    glm::vec4 dColorY =(color[1]-color[2])*(minYVert.texCoord.x-maxYVert.texCoord.x) -
//    (color[0]-color[2])*(midYVert.texCoord.x-maxYVert.texCoord.x);
//
//    colorXStep = dColorX * oneOverdx;
//    colorYStep = dColorY * oneOverdy;
    
    //OneOverZ
    
    float w [3];
    w[0] = minYVert.coord.w;
    w[1] = midYVert.coord.w;
    w[2] = maxYVert.coord.w;
    
    std::sort(std::begin(w),std::end(w));
    
    float middle = ( w[0] + w[2] )/2;
    
    float wFlipped [3];
    wFlipped[0] = middle-(minYVert.coord.w-middle);
    wFlipped[1] = middle-(midYVert.coord.w-middle);
    wFlipped[2] = middle-(maxYVert.coord.w-middle);
    
    oneOverZ[0] = 1.0f / wFlipped[0];
    oneOverZ[1] = 1.0f / wFlipped[1];
    oneOverZ[2] = 1.0f / wFlipped[2];
    
    oneOverZStep.x = ((oneOverZ[1]-oneOverZ[2])*(minYVert.texCoord.y-maxYVert.texCoord.y) -
                      (oneOverZ[0]-oneOverZ[2])*(midYVert.texCoord.y-maxYVert.texCoord.y) ) * oneOverdx;
    
    oneOverZStep.y = ((oneOverZ[1]-oneOverZ[2])*(minYVert.texCoord.x-maxYVert.texCoord.x) -
                      (oneOverZ[0]-oneOverZ[2])*(midYVert.texCoord.x-maxYVert.texCoord.x) ) * oneOverdy;
        
    photoCoords[0] = minYVert.coord * oneOverZ[0];
    photoCoords[1] = midYVert.coord * oneOverZ[1];
    photoCoords[2] = maxYVert.coord * oneOverZ[2];

//    photoCoords[0] = minYVert.coord ;
//    photoCoords[1] = midYVert.coord ;
//    photoCoords[2] = maxYVert.coord ;
    
    
    photoCoordXXStep = ((photoCoords[1].x-photoCoords[2].x)*(minYVert.texCoord.y-maxYVert.texCoord.y) -
                       (photoCoords[0].x-photoCoords[2].x)*(midYVert.texCoord.y-maxYVert.texCoord.y) ) * oneOverdx ;
    
    photoCoordXYStep = ((photoCoords[1].x-photoCoords[2].x)*(minYVert.texCoord.x-maxYVert.texCoord.x) -
                      (photoCoords[0].x-photoCoords[2].x)*(midYVert.texCoord.x-maxYVert.texCoord.x) ) * oneOverdy;
    
    photoCoordYXStep = ((photoCoords[1].y-photoCoords[2].y)*(minYVert.texCoord.y-maxYVert.texCoord.y) -
                      (photoCoords[0].y-photoCoords[2].y)*(midYVert.texCoord.y-maxYVert.texCoord.y) ) * oneOverdx;
    
    photoCoordYYStep = ((photoCoords[1].y-photoCoords[2].y)*(minYVert.texCoord.x-maxYVert.texCoord.x) -
                      (photoCoords[0].y-photoCoords[2].y)*(midYVert.texCoord.x-maxYVert.texCoord.x) ) * oneOverdy;
}


TextureGradient::TextureGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert, glm::vec4 color [3]){
    float oneOverdx = 1.0f/((midYVert.texCoord.x-maxYVert.texCoord.x)*(minYVert.texCoord.y-maxYVert.texCoord.y) -
                            (midYVert.texCoord.y-maxYVert.texCoord.y)*(minYVert.texCoord.x-maxYVert.texCoord.x));
    float oneOverdy = -oneOverdx;
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
    
    glm::vec4 dColorX =(color[1]-color[2])*(minYVert.texCoord.y-maxYVert.texCoord.y) -
    (color[0]-color[2])*(midYVert.texCoord.y-maxYVert.texCoord.y);
    
    glm::vec4 dColorY =(color[1]-color[2])*(minYVert.texCoord.x-maxYVert.texCoord.x) -
    (color[0]-color[2])*(midYVert.texCoord.x-maxYVert.texCoord.x);
    
    colorXStep = dColorX * oneOverdx;
    colorYStep = dColorY * oneOverdy;
    
}



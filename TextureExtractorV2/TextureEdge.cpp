//
//  Edge.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "TextureEdge.hpp"

TextureEdge::TextureEdge(Vertex start, Vertex end, TextureGradient gradient,int minYVertIndex){
    yStart = (int)ceil(start.texCoord.y);
    yEnd   = (int)ceil(end.texCoord.y); //0-1 wrong
    
    float yDistance = end.texCoord.y - start.texCoord.y;
    float xDistance =  end.texCoord.x - start.texCoord.x;
    
    float yPrestep = yStart - start.texCoord.y;
    xStep = (float)xDistance/(float)yDistance;
    currentX = start.texCoord.x + yPrestep*xStep ;
    float xPrestep = currentX - start.texCoord.x;
    
    
    //calc texture gradientStep
    photoCoord.x = gradient.photoCoords[minYVertIndex].x +
    gradient.photoCoordXXStep*xPrestep +
    gradient.photoCoordXYStep*yPrestep;
    photoCoordStep.x = gradient.photoCoordXYStep+gradient.photoCoordXXStep*xStep;
    
    photoCoord.y = gradient.photoCoords[minYVertIndex].y +
    gradient.photoCoordYXStep*xPrestep +
    gradient.photoCoordYYStep*yPrestep;
    photoCoordStep.y = gradient.photoCoordYYStep+gradient.photoCoordYXStep*xStep;
    
    oneOverZ =  gradient.oneOverZ[minYVertIndex] +
    gradient.oneOverZStep.x*xPrestep +
    gradient.oneOverZStep.y*yPrestep;
    oneOverZStep = gradient.oneOverZStep.y + gradient.oneOverZStep.x*xStep;

}

TextureEdge::TextureEdge(Vertex start, Vertex end){
    yStart = (int)ceil(start.texCoord.y);
    yEnd   = (int)ceil(end.texCoord.y); //0-1 wrong
    float yDistance = end.texCoord.y - start.texCoord.y;
    float xDistance =  end.texCoord.x - start.texCoord.x;
    
    float yPrestep = yStart - start.texCoord.y;
    xStep = (float)xDistance/(float)yDistance;
    currentX = start.texCoord.x + yPrestep*xStep ;
    float xPrestep = currentX - start.texCoord.x;
}


void TextureEdge::Step(){
    currentX += xStep;
    photoCoord.x += photoCoordStep.x;
    photoCoord.y += photoCoordStep.y;
    oneOverZ += oneOverZStep;
}


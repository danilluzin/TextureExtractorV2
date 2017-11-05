//
//  Edge.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Edge.hpp"

Edge::Edge(Vertex start, Vertex end, Gradient gradient,int minYVertIndex){
    yStart = (int)ceil(start.y());
    yEnd   = (int)ceil(end.y());
    
    float yDistance = end.y() - start.y();
    float xDistance = end.x() - start.x();

    float yPrestep = yStart - start.y();
    xStep = (float)xDistance/(float)yDistance;
    currentX = start.x() + yPrestep*xStep ;
    float xPrestep = currentX - start.x();
    
    //calc color gradientStep
    color = gradient.color[minYVertIndex] + gradient.colorYStep*yPrestep +
            gradient.colorXStep*xPrestep;
    colorStep = gradient.colorYStep + gradient.colorXStep*xStep;
    
    //calc texture gradientStep
    texCoord.x = gradient.texCoords[minYVertIndex].x +
                 gradient.texCoordXXStep*xPrestep +
                 gradient.texCoordXYStep*yPrestep;
    texCoordStep.x = gradient.texCoordXYStep+gradient.texCoordXXStep*xStep;
    
    texCoord.y = gradient.texCoords[minYVertIndex].y +
                 gradient.texCoordYXStep*xPrestep +
                 gradient.texCoordYYStep*yPrestep;
    texCoordStep.y = gradient.texCoordYYStep+gradient.texCoordYXStep*xStep;
    
    oneOverZ =  gradient.oneOverZ[minYVertIndex] +
                gradient.oneOverZStep.x*xPrestep +
                gradient.oneOverZStep.y*yPrestep;
    oneOverZStep = gradient.oneOverZStep.y + gradient.oneOverZStep.x*xStep;
    
    depth =  gradient.depth[minYVertIndex] +
             gradient.depthStep.x*xPrestep +
             gradient.depthStep.y*yPrestep;
    depthStep = gradient.depthStep.y + gradient.depthStep.x*xStep;
    
    
    
    
}


void Edge::Step(){
    currentX += xStep;
    color += colorStep;
    texCoord.x += texCoordStep.x;
    texCoord.y += texCoordStep.y;
    oneOverZ += oneOverZStep;
    depth+= depthStep;
}

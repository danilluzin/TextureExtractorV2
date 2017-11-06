//
//  Gradient.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Gradient.hpp"

Gradient::Gradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert){
    
    float oneOverdx = 1.0f/
    ((midYVert.x()-maxYVert.x())*(minYVert.y()-maxYVert.y()) -
     (midYVert.y()-maxYVert.y())*(minYVert.x()-maxYVert.x()));
    
    float oneOverdy = -oneOverdx;
    
    glm::vec4 dColorX =(color[1]-color[2])*(minYVert.y()-maxYVert.y()) -
    (color[0]-color[2])*(midYVert.y()-maxYVert.y());
    
    glm::vec4 dColorY =(color[1]-color[2])*(minYVert.x()-maxYVert.x()) -
    (color[0]-color[2])*(midYVert.x()-maxYVert.x());

    colorXStep = dColorX * oneOverdx;
    colorYStep = dColorY * oneOverdy;
    
    //OneOverZ
    oneOverZ[0] = 1.0f / minYVert.coord.w;
    oneOverZ[1] = 1.0f / midYVert.coord.w;
    oneOverZ[2] = 1.0f / maxYVert.coord.w;
    
    oneOverZStep.x = ((oneOverZ[1]-oneOverZ[2])*(minYVert.y()-maxYVert.y()) -
                      (oneOverZ[0]-oneOverZ[2])*(midYVert.y()-maxYVert.y()) ) * oneOverdx;
    
    oneOverZStep.y = ((oneOverZ[1]-oneOverZ[2])*(minYVert.x()-maxYVert.x()) -
                      (oneOverZ[0]-oneOverZ[2])*(midYVert.x()-maxYVert.x()) ) * oneOverdy;
    
    //TexCoord
    texCoords[0] = minYVert.texCoord * oneOverZ[0];
    texCoords[1] = midYVert.texCoord * oneOverZ[1];
    texCoords[2] = maxYVert.texCoord * oneOverZ[2];
    
    texCoordXXStep = ((texCoords[1].x-texCoords[2].x)*(minYVert.y()-maxYVert.y()) -
                     (texCoords[0].x-texCoords[2].x)*(midYVert.y()-maxYVert.y()) ) * oneOverdx;
    
    texCoordXYStep = ((texCoords[1].x-texCoords[2].x)*(minYVert.x()-maxYVert.x()) -
                     (texCoords[0].x-texCoords[2].x)*(midYVert.x()-maxYVert.x()) ) * oneOverdy;
    
    texCoordYXStep = ((texCoords[1].y-texCoords[2].y)*(minYVert.y()-maxYVert.y()) -
                      (texCoords[0].y-texCoords[2].y)*(midYVert.y()-maxYVert.y()) ) * oneOverdx;
    
    texCoordYYStep = ((texCoords[1].y-texCoords[2].y)*(minYVert.x()-maxYVert.x()) -
                      (texCoords[0].y-texCoords[2].y)*(midYVert.x()-maxYVert.x()) ) * oneOverdy;
    
    ///depth
    depth[0] = minYVert.z();
    depth[1] = midYVert.z();
    depth[2] = maxYVert.z();

    depthStep.x =((depth[1]-depth[2])*(minYVert.y()-maxYVert.y()) -
                  (depth[0]-depth[2])*(midYVert.y()-maxYVert.y()) ) * oneOverdx;
    depthStep.y =((depth[1]-depth[2])*(minYVert.x()-maxYVert.x()) -
                  (depth[0]-depth[2])*(midYVert.x()-maxYVert.x()) ) * oneOverdy;
    
}

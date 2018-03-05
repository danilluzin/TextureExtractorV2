//
//  Transformation.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Transformation.hpp"
#include "Utils.h"
Transformation::Transformation(const glm::vec3 & pos,
                               const glm::vec3 & rot,
                               const glm::vec3 & scale){
    
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
    screenSpaceTransform[0][0] = 1;
    screenSpaceTransform[1][1] = 1;
    screenSpaceTransform[2][2] = 1;
    screenSpaceTransform[3][3] = 1;
}

void Transformation::setAspectRatio(int width, int height){
    setScreenTransform(height/2, width/2);
    float aspect = (float)width/ (float)height;
    camera.perspective = glm::perspective(camera.fov,aspect,camera.zNear,camera.zFar);
}

void Transformation::setCamera (const Camera & camera){
    this->camera = camera;
    cameraIsSet = true;
}

void Transformation::setScreenTransform(int halfHeight, int halfWidth){
    screenSpaceTransform[0][0] = halfWidth;
    screenSpaceTransform[1][1] = -halfHeight;
    screenSpaceTransform[3][0] = halfWidth - 0.5;
    screenSpaceTransform[3][1] = halfHeight - 0.5;
}


Vertex Transformation::doPerspectiveDevide(const Vertex & coord) const {
 Vertex res(glm::vec4(coord.x() / coord.w(),
                            coord.y() / coord.w(),
                            coord.z() / coord.w(),
                            coord.w()));
    res.id = coord.id;
    res.texCoord = coord.texCoord;
    return res;
}

glm::mat4 Transformation::getScreenTransform() const{
    return  screenSpaceTransform;
}

glm::mat4 Transformation::getViewProjection() const{
    return camera.getViewProjection();
}


glm::mat4 Transformation::getModelMatrix() const{
    glm::mat4 posMatrix = glm::translate(pos);
    glm::mat4 scaleMatrix = glm::scale(scale);
    glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
    glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
    glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
    
    glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
   
    return posMatrix*rotMatrix*scaleMatrix;
}

bool componentFitsW (int component,const Vertex & v){
    //0 = x >= -w
    //1 = x <=  w
    //2 = y >= -w
    //3 = y <=  w
    //4 = z >= -w
    //5 = z <=  w
    switch (component) {
        case 0:
            return v.x() >= -v.w();
        case 1:
            return v.x() <= v.w();
        case 2:
            return v.y() >= -v.w();
        case 3:
            return v.y() <= v.w();
        case 4:
            return v.z() >= -v.w();
        case 5:
            return v.z() <= v.w();
    }
  
    return false;
};

bool isInsideViewFrustrum (const BoundingBox & boundingBox, const Transformation & transformation){
    float x[2],y[2],z[2];
    glm::mat4 cameraModelTransform = transformation.getViewProjection() * transformation.getModelMatrix();
    
    x[0] = boundingBox.minVec.x; x[1] = boundingBox.maxVec.x;
    y[0] = boundingBox.minVec.y; y[1] = boundingBox.maxVec.y;
    z[0] = boundingBox.minVec.z; z[1] = boundingBox.maxVec.z;
    
    std::vector<Vertex> corners;
    
    for(int i = 0; i<2; i++){
        for(int m = 0; m<2; m++){
            for(int q = 0; q<2; q++){
                corners.push_back(cameraModelTransform * Vertex(x[i],y[m],z[q]));
            }
        }
    }
    
    for(int i=0; i < 6; i++) {
        int out=0,in=0;
        for (int k = 0; k < 8 && (in==0 || out==0); k++) {
            if (componentFitsW(i,corners[k]))
                in++;
            else
                out++;
        }
        if (!in)
            return false;
    }
    
    return true;
}



















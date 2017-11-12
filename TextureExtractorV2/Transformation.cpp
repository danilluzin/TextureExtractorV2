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






















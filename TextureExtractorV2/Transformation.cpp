//
//  Transformation.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Transformation.hpp"


Camera::Camera(const glm::vec3 pos, float fov, float aspect,
               glm::vec3 forward, glm::vec3 up, float zNear, float zFar,
               bool useLookAt){
    perspective = glm::perspective(fov,aspect,zNear,zFar);
    position = pos;
    this->forward = forward;
    this->up = up;
    this->fov = fov ;
    this->zNear = zNear;
    this->zFar = zFar;
    this->useLookAt = useLookAt;
}

glm::mat4 Camera::getViewProjection() const {
//    std::cout<<"Perspective\n";
//    printMatrix(perspective);
//    std::cout<<"lookAt\n";
//    printMatrix(glm::lookAt(position, forward, up));
    if(useLookAt){
        return perspective * glm::lookAt(position, forward, up);
    }else{
        glm::vec3 pos(7.48113f,5.34367f,6.50764f);
        glm::mat4 posMatrix = glm::translate(pos);
        glm::mat4 rotXMatrix = glm::rotate(glm::radians(-26.4f), glm::vec3(1, 0, 0));
        glm::mat4 rotYMatrix = glm::rotate(glm::radians(46.7f), glm::vec3(0, 1, 0));
        glm::mat4 rotZMatrix = glm::rotate(glm::radians(0.0f), glm::vec3(0, 0, 1));
        
        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
        
        glm::mat4 cameraPose = posMatrix * rotMatrix;
//        printMatrix(cameraPose);
        glm::mat4 cameraView = glm::inverse(cameraPose);
//        printMatrix(cameraView);
        return perspective * cameraView;

    }
}


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


void printMatrix(const glm::mat4 & m){
    for(int y=0;y<4;y++){
        for(int x=0;x<4; x++){
            std::cout<<m[x][y]<<" ";
        }
        std::cout<<"\n";
    }
}



















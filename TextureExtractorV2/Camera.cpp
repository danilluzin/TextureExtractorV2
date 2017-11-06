//
//  Camera.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 06/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Camera.hpp"


Camera::Camera(const glm::vec3 pos, float fov, float aspect,
               glm::vec3 forward, glm::vec3 up, float zNear, float zFar){
    perspective = glm::perspective(fov,aspect,zNear,zFar);
    position = pos;
    this->forward = forward;
    this->up = up;
    this->fov = fov ;
    this->zNear = zNear;
    this->zFar = zFar;
}

glm::mat4 Camera::getViewProjection() const {
    //    std::cout<<"Perspective\n";
    //    printMatrix(perspective);
    //    std::cout<<"lookAt\n";
    //    printMatrix(glm::lookAt(position, forward, up));
   
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

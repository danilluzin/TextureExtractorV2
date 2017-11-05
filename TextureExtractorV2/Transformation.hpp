//
//  Transformation.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Transformation_hpp
#define Transformation_hpp

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Mesh.hpp"

class Camera{
public:
    Camera(const glm::vec3 pos = glm::vec3(0,0,0),
           float fov = 70.0f,
           float aspect = 1,
           glm::vec3 forward = glm::vec3(0,0,0),
           glm::vec3 up = glm::vec3(0,1,0),
           float zNear = 0.01f,
           float zFar = 1000.0f,
           bool useLookAt = true
           );
    bool useLookAt = true;
    
    glm::mat4 getViewProjection() const;
    
    glm::vec3 position;
    
    glm::vec3 forward;
    
    glm::vec3 up;
    
    float fov,zNear,zFar;

    glm::mat4 perspective;

};


class Transformation{
    
    glm::mat4 screenSpaceTransform;
    
    bool cameraIsSet = false;
    
    Camera camera;

public:

    glm::vec3 pos, rot, scale;
    
    Transformation(const glm::vec3 & pos = glm::vec3(),
                   const glm::vec3 & rot = glm::vec3(),
                   const glm::vec3 & scale = glm::vec3(1.0,1.0,1.0));
    
    void setAspectRatio(int width, int height);
    
    void setCamera (const Camera & camera);
    
    void setScreenTransform(int halfHeight, int halfWidth);
    
    Vertex doPerspectiveDevide(const Vertex & coord) const ;
    
    glm::mat4 getModelMatrix() const;
    
    glm::mat4 getScreenTransform() const;

    glm::mat4 getViewProjection() const;
    
private:
    
};

void printMatrix(const glm::mat4 & m);

#endif /* Transformation_hpp */

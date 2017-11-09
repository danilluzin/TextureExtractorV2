//
//  Camera.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 06/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Camera{
public:
    Camera(bool eh, const glm::vec3 pos = glm::vec3(0,0,0),
           float fov = 70.0f,
           float aspect = 1,
           glm::vec3 forward = glm::vec3(0,0,0),
           glm::vec3 up = glm::vec3(0,1,0),
           float zNear = 0.01f,
           float zFar = 1000.0f
           );
    
    Camera(){};
    
    glm::mat4 getViewProjection() const;
    
    glm::vec3 position;
    
    glm::vec3 forward;
    
    glm::vec3 up;
    
    float fov;
    float zNear = 0.01f;
    float zFar = 1000.0f;
    
    glm::mat4 perspective;
    
    float focalLength;
    
    glm::mat4 rotationMatrix;
    
    glm::vec3 translation;
    
    
};
#endif /* Camera_hpp */

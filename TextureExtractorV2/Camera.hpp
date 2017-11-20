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
#include <limits.h>

class Camera{
public:

    Camera(){};
    
    glm::mat4 getViewProjection() const;
    
    glm::vec3 position;
    
    glm::vec3 forward;
    
    glm::vec3 up;
    
    float fov;
    float zNear = 0.00001f;
    float zFar = 1000.0f;
    
    glm::mat4 perspective;
    
    float focalLength;
    
    glm::mat4 rotationMatrix;
    
    glm::vec3 translation;
    
    
};
#endif /* Camera_hpp */

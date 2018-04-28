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

/**
 * Class that hold camera parameters and position for a certain photo
 */
class Camera{
public:

    Camera(){};
    
    /**
     * Calculates a projection matrix
     * @return a projection matrix using the bundel matricies and glm perspective
     */
    glm::mat4 getViewProjection() const;
    
    /** Field of view estimation of the camera */
    float fov;
    
    /** Near clipping plane */
    float zNear = 0.01f;
    
    /** Far clipping plane */
    float zFar = 1000.0f;
    
    /** Perspective projection matrix */
    glm::mat4 perspective;
    
    /** focal lenght in pixels */
    float focalLength;
    
    /** Camera rotation matrix */
    glm::mat4 rotationMatrix;
    
    /** Translation vector */
    glm::vec3 translation;
    
    
};
#endif /* Camera_hpp */

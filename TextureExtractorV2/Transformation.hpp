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
#include "Camera.hpp"

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

#endif /* Transformation_hpp */

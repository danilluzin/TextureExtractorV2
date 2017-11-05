//
//  Gradient.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Gradient_hpp
#define Gradient_hpp

#include "glm/glm.hpp"
#include "Mesh.hpp"


class Gradient {

public:
    glm::vec4 colorXStep;
    glm::vec4 colorYStep;
    glm::vec4 color [3];

    glm::vec2 texCoords [3];
    float texCoordXXStep;
    float texCoordXYStep;
    float texCoordYXStep;
    float texCoordYYStep;
    
    float oneOverZ [3];
    glm::vec2 oneOverZStep;
    
    float depth [3];
    glm::vec2 depthStep;
    
    Gradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert) ;
    
};



#endif /* Gradient_hpp */

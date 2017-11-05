//
//  Edge.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "Gradient.hpp"
class Edge{
public:
    
    float currentX;
    float xStep;
    float yStart;
    float yEnd;
    
    glm::vec4 color;
    glm::vec4 colorStep;
    
    float oneOverZ;
    float oneOverZStep;
    
    float depth;
    float depthStep;
    
    glm::vec2 texCoord;
    glm::vec2 texCoordStep;
    
    Edge (Vertex start, Vertex end, Gradient gradient,int minYVertIndex);
    void Step();
};


#endif /* Edge_hpp */

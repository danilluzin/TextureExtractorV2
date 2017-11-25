//
//  Edge.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef TextureEdge_hpp
#define TextureEdge_hpp

#include <glm/glm.hpp>
#include "Mesh.hpp"
#include "TextureGradient.hpp"

class TextureEdge{
public:
    
    float currentX;
    float xStep;
    float yStart;
    float yEnd;

    float oneOverZ;
    float oneOverZStep;
    
    glm::vec2 photoCoord;
    glm::vec2 photoCoordStep;
    
    glm::vec4 color;
    glm::vec4 colorStep;
    
    TextureEdge (Vertex start, Vertex end, TextureGradient gradient,int minYVertIndex);
    TextureEdge (Vertex start, Vertex end);
    void Step();
};


#endif /* Edge_hpp */


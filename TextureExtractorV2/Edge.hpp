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

/**
 * Class used for rasterization. Defines a polygon edge.
 */
class Edge{
public:
    /** Current x value fo the edge step position */
    float currentX;
    
    /** Step of x value as we move down the edge */
    float xStep;
    
    /** Y value at the start of the edge */
    float yStart;
    
    /** Y value at the end of the edge */
    float yEnd;
    
    /** Current step color value */
    glm::vec4 color;
    
    /** Color step value. Gets added to color as we move down the edge */
    glm::vec4 colorStep;
    
    /** Current one over z (1/z) value   */
    float oneOverZ;

    /** One over z (1/z) step */
    float oneOverZStep;
    
    /** Current depth value.  Gets added to the depth as we move down the edge */
    float depth;
    
    /** Depth step value. Gets added to depth as we move down the edge */
    float depthStep;
    
    /** Current texture coordinate value.  Gets added to the texture coordinate as we move down the edge */
    glm::vec2 texCoord;
    
    /** Texture coordinate step value. Gets added to texture coordinate as we move down the edge */
    glm::vec2 texCoordStep;
    
    Edge (Vertex start, Vertex end, Gradient gradient,int minYVertIndex);
    
    /**
     * Steps all the values on the edge. Gets called when we move down the edge
     */
    void Step();
};


#endif /* Edge_hpp */

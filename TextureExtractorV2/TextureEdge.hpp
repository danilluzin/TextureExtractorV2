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


/**
 * Class that sets up a texture edge. Used for forward mapping
 */
class TextureEdge{
public:
    
    
    /** Current x value fo the edge step position */
    float currentX;
    
    /** Step of x value as we move down the edge */
    float xStep;
    
    /** Y value at the start of the edge */
    float yStart;
    
    /** Y value at the end of the edge */
    float yEnd;

    /** Current one over z (1/z) value   */
    float oneOverZ;
    
    /** One over z (1/z) step */
    float oneOverZStep;
    
    /** Current photo coordinate value.  Gets added to the photo coordinates as we move down the edge */
    glm::vec2 photoCoord;
    
    /** Photo coordinate step value. Gets added to photo coordinate as we move down the edge */
    glm::vec2 photoCoordStep;

    /** Current step color value. Used for applyng color gradient when doing seam leveling */
    glm::vec4 color;
    /** Color step value. Gets added to color as we move down the edge */
    glm::vec4 colorStep;
    
    TextureEdge (Vertex start, Vertex end, TextureGradient gradient,int minYVertIndex);
    TextureEdge (Vertex start, Vertex end);
    
    /**
     * Steps all the values on the edge. Gets called when we move down the edge
     */
    void Step();
};


#endif /* Edge_hpp */


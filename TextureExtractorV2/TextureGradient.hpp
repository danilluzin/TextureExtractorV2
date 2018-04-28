//
//  Gradient.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef TextureGradient_hpp
#define TextureGradient_hpp

#include "glm/glm.hpp"
#include "Mesh.hpp"


class TextureGradient {
    
public:
    /**
     * Sets up a gradient. Uses three verticies to acalculate steppin values.
     */
    TextureGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert) ;
    
    /**
     * Sets up a gradient. Uses three verticies to acalculate steppin values. Adds color gradient as well
     */
    TextureGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert, glm::vec4 color [3]);
    
    
    /** Photo coordinate x step as we move in the x direction on a texture */
    float photoCoordXXStep;
    /** Photo coordinate x step as we move in the y direction on a texture */
    float photoCoordXYStep;
    /** Photo coordinate y step as we move in the x direction on a texture */
    float photoCoordYXStep;
    /** Photo coordinate y step as we move in the y direction on a texture */
    float photoCoordYYStep;
    
    /** One overZ step */
    glm::vec2 oneOverZStep;
    
    /** Photo coordinates at the texture verticies that make up gradient */
    glm::vec4 photoCoords [3];

    /** Color step as we move in the x direction */
    glm::vec4 colorXStep;
    /** Color step as we move in the y direction */
    glm::vec4 colorYStep;
    /** Color adjustemnt values at the verticies that make up gradient */
    glm::vec4 color [3];
    
     /** One overZ step */
    float oneOverZ [3];
private:
};



#endif /* Gradient_hpp */


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

/**
 * Class that sets up a interpolation gradient
 */
class Gradient {

public:
    /** Color step as we move in the x direction  */
    glm::vec4 colorXStep;
    /** Color step as we move in the y direction */
    glm::vec4 colorYStep;
    
    /** Color values at the verticies that make up gradient */
    glm::vec4 color [3];

    /** Texture coordinates at the verticies that make up gradient */
    glm::vec2 texCoords [3];
    
     /** Texture coordinate x step as we move in the x direction on a raster */
    float texCoordXXStep;
    /** Texture coordinate x step as we move in the y direction on a raster */
    float texCoordXYStep;
    /** Texture coordinate y step as we move in the x direction on a raster */
    float texCoordYXStep;
    /** Texture coordinate y step as we move in the y direction on a raster */
    float texCoordYYStep;
    
    /** One over z at the verticies that make up gradient */
    float oneOverZ [3];
    /** One overZ step */
    glm::vec2 oneOverZStep;
    
    /** Depth value at the verticies that make up gradient */
    float depth [3];
    
    /** Depth step step */
    glm::vec2 depthStep;
    
    /**
     * Sets up a gradient. Uses three verticies to acalculate steppin values.
     */
    Gradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert) ;
    
};



#endif /* Gradient_hpp */

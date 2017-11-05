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
    TextureGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert) ;

    float photoCoordXXStep;
    float photoCoordXYStep;
    float photoCoordYXStep;
    float photoCoordYYStep;
    
    glm::vec2 oneOverZStep;
    
    glm::vec4 photoCoords [3];

    float oneOverZ [3];
private:
};



#endif /* Gradient_hpp */


//
//  RenderContext.h
//  TextureExtractor
//
//  Created by Danil Luzin on 09/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef RenderContext_h
#define RenderContext_h

#include <glm/glm.hpp>


class RenderContext{
public:
    RenderContext(int width, int height){
        this->width = width;
        this->height = height;
    };
    int width, height;
    virtual void putPixel(int x,int y, glm::vec4 color) = 0;
    virtual void clear(glm::vec4 color) = 0;
};

#endif /* RenderContext_h */

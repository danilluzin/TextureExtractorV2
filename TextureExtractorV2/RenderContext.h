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

/**
 * Class that is common for every render context. Was useful when i had output to the screen. Tight now only render context is a Bitmap
 */
class RenderContext{
public:
    /**
     * Iniitialize new render context
     * @param width new context width
     * @param height new context height
     */
    RenderContext(int width, int height){
        this->width = width;
        this->height = height;
    };
    int width, height;
    
    /**
     * Abstract method. Puts pixel of certain color into a render context
     * @param <#name#> <#description#>
     */
    virtual void putPixel(int x,int y, glm::vec4 color) = 0;
    
    /**
     * Abstract method. Fill whole imagwith solid color
     * @param color color to use for filling
     */
    virtual void clear(glm::vec4 color) = 0;
};

#endif /* RenderContext_h */

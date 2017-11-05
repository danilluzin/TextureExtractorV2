//
//  Bitmap.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Bitmap_hpp
#define Bitmap_hpp

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "RenderContext.h"




class Bitmap : public RenderContext{
    
    std::vector<glm::vec4> imageData;
    
public:
    Bitmap(int width, int height) : RenderContext(width,height){
        imageData.resize(width * height);
    }
    Bitmap():RenderContext(0,0){};
    Bitmap(const std::string & filename);
    
    glm::vec4 at(int x, int y) const{
        return imageData[y * width + x];
    }
    
    void putPixel(int x, int y, glm::vec4 color){
        if(x<0 || x>=width || y<0 || y>=height )
            return;
        imageData[y * width + x] = color;
    }
    
    void toPPM(std::string destFilename);
    
    void clear(glm::vec4 color);
    
};

#endif /* Bitmap_hpp */

//
//  Bitmap.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Bitmap.hpp"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void Bitmap::clear(glm::vec4 color){
    std::fill(imageData.begin(), imageData.end(), color);
}

Bitmap::Bitmap(const std::string & filename) : RenderContext(0,0){
    int numComponents;
    unsigned char * imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
    
    if(imageData == nullptr){
        std::cerr << "ERROR Texture Loading Failed: filename = " + filename+"\n";
        return;
    }
    
    this->imageData.resize(width*height);
    
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            float r = ((float)imageData[y*width*4 + x*4 + 0])/255;
            float g = ((float)imageData[y*width*4 + x*4 + 1])/255;
            float b = ((float)imageData[y*width*4 + x*4 + 2])/255;
            float a = ((float)imageData[y*width*4 + x*4 + 3])/255;
            
            putPixel(x, y, glm::vec4(r,g,b,a));
        }
    }
    toPPM("./resources/w.ppm");
    
}

void Bitmap::toPPM(std::string destFilename){
    std::ofstream file;
    file.open(destFilename.c_str());
    if(!file.is_open()){
        std::cerr << "ERROR: couldnt open file to write:" << destFilename << "\n";
        return;
    }
    std::stringstream ss;
    ss << "P6\n" << width << " " << height << "\n" << 255 << "\n";
    file.write(ss.str().c_str(), ss.str().size());
    
    char pixelBuffer [3];
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            glm::vec4 color = at(x, y);
            pixelBuffer[0] = (int)(color[0] * 255);
            pixelBuffer[1] = (int)(color[1] * 255);
            pixelBuffer[2] = (int)(color[2] * 255);
            file.write(pixelBuffer, 3);
        }
    }
    file.close();
    if(!file.good()){
        std::cerr << "ERROR: Writing to file :" << destFilename << " failed\n";
        return;
    }
}



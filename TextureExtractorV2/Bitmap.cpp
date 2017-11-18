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
    
    image = cv::Scalar((uchar)(color[2]*255), (uchar)(color[1]*255),(uchar)(color[0]*255));
    
//    std::fill(imageData.begin(), imageData.end(), color);
}

Bitmap::Bitmap(const std::string & filename) : RenderContext(0,0){
    
    image = cv::imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);
    if(! image.data) {
        std::cerr << "ERROR Texture Loading Failed: filename = " + filename+"\n";
        return ;
    }
    
    cv::Size s = image.size();
    width = s.width;
    height = s.height;
    return;
}

void Bitmap::toPPM(std::string destFilename){
    
    bool fileOk;
    fileOk = cv::imwrite (destFilename.c_str(),image);
    if(!fileOk){
        std::cerr << "ERROR: couldnt open file to write:" << destFilename << "\n";
        return;
    }
    return;
}



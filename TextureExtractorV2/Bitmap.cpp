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

Bitmap::Bitmap(const cv::Mat & img) : RenderContext(0,0){
    image = img;
    cv::Size s = image.size();
    width = s.width;
    height = s.height;
    return;
}

Bitmap Bitmap::toSobel() const{
    cv::Mat grayImg;
    cv::Mat sobelImg;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    
    cvtColor( image, grayImg, CV_BGR2GRAY );
    Sobel( grayImg, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    Sobel( grayImg, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );
    
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobelImg );
    return Bitmap(sobelImg);
}


Bitmap Bitmap::toHSV() const{
    cv::Mat HSV;
    cvtColor(image, HSV,CV_BGR2HSV);
    return Bitmap(HSV);
}


void Bitmap::save(std::string destFilename){
    
    bool fileOk;
    fileOk = cv::imwrite (destFilename.c_str(),image);
    if(!fileOk){
        std::cerr << "ERROR: couldnt open file to write:" << destFilename << "\n";
        return;
    }
    return;
}



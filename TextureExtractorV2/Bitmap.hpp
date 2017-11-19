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
#include <opencv2/opencv.hpp>



class Bitmap : public RenderContext{
    
    std::vector<glm::vec4> imageData;
    cv::Mat image;
    
public:
    Bitmap(int width, int height) : RenderContext(width,height){
        image = cv::Mat(height, width, CV_8UC3);
        imageData.resize(width * height);
    }
    Bitmap():RenderContext(0,0){};
    Bitmap(const std::string & filename);
    
    glm::vec4 at(int x, int y) const{
        if(x<0||x>=width||y<0||y>=height){
            return glm::vec4(0,0,0,1);
        }
        cv::Vec3b vec = image.at<cv::Vec3b>(y,x);
        return glm::vec4((float)vec[2]/255,(float)vec[1]/255,(float)vec[0]/255,1);
//        return imageData[y * width + x];
    }
    
    void putPixel(int x, int y, glm::vec4 color){
        if(x<0 || x>=width || y<0 || y>=height )
            return;
//        imageData[y * width + x] = color;
        cv::Vec3b vec = image.at<cv::Vec3b>(y,x);

        image.at<cv::Vec3b>(y,x)[0] = color[2]*255;
        image.at<cv::Vec3b>(y,x)[1] = color[1]*255;
        image.at<cv::Vec3b>(y,x)[2] = color[0]*255;
        
        vec = image.at<cv::Vec3b>(y,x);
    }
    
    void toPPM(std::string destFilename);
    
    void clear(glm::vec4 color);
    
};

#endif /* Bitmap_hpp */

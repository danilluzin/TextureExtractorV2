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


/**
 * Class that manages all the bitmap operations, including reading from file and sobel ooperator modification.
 */
class Bitmap : public RenderContext{
    /** OpenCV representaion of the image */
    cv::Mat image;
    
public:
    
    Bitmap & operator = (const Bitmap & other){
        if (this != &other) { // self-assignment check expected
            width = other.width;
            height = other.height;
            image = other.image.clone();
        }
        return *this;
    }
    
    /**
     * Create a new image of certain size
     * @param width new image width
     * @param height new image height
     */
    Bitmap(int width, int height) : RenderContext(width,height){
        image = cv::Mat(height, width, CV_8UC3);
    }
    
    Bitmap():RenderContext(0,0){};
    
    /**
     * Read an image from file
     * @param filename path to a file
     */
    Bitmap(const std::string & filename);
    
    /**
     * Create an image from anothe opneCV image
     * @param img image file
     */
    Bitmap(const cv::Mat & img);
    
    /**
     * Apply a Sobel filter to an image
     */
    Bitmap toSobel() const;
    
    /**
     * Convert file to HSV
     */
    Bitmap toHSV() const;
    
    /**
     * Get a pixel at position x, y
     */
    glm::vec4 at(int x, int y) const{
        if(x<0||x>=width||y<0||y>=height){
            return glm::vec4(0,0,0,1);
        }
        
        
        int channels = image.channels();
        if(channels == 1){
            uchar val =  image.at<unsigned char>(y,x);
            return glm::vec4((float)val/255,(float)val/255,(float)val/255,1);
        }else{
            cv::Vec3b vec = image.at<cv::Vec3b>(y,x);
            return glm::vec4((float)vec[2]/255,(float)vec[1]/255,(float)vec[0]/255,1);
        }
    }
    /**
     * Write color pixel at position x, y
     * @param color Color to write at position x,y
     */
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
    
    /**
     * Save an image into file
     * @param destFilename fillename for the output image
     */
    void save(std::string destFilename);
    
    /**
     * Fill whole imagwith solid color
     * @param color color to use for filling
     */
    void clear(glm::vec4 color);
    
};

#endif /* Bitmap_hpp */

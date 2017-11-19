//
//  TextureExtractor.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 15/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef TextureExtractor_hpp
#define TextureExtractor_hpp

#include <stdio.h>
#include <vector>
#include "Bitmap.hpp"
#include "Mesh.hpp"
#include "TextureEdge.hpp"
#include "View.hpp"
#include "Transformation.hpp"
#include "Display.hpp"
#include "Arguments.h"


class TextureExtractor {
    
    Bitmap texture;
    
    Mesh mesh;
        
    std::map<uint,View> views;

    Arguments arguments;

public:

    ~TextureExtractor();
    
    bool prepareViews();
    
    bool calculateDataCosts();
    
    bool selectViews();
    
    void setMesh (const Mesh & m){mesh = m;}
    
    bool generateTexture();
    
    uint numberOfViews(){return (uint)views.size();}
    
    void setArguments(const Arguments & arguments){this->arguments = arguments;}
 
    bool readLabelsFromFile();
    
    //DEBUG
    
    void checkCameraInfo();

    void checkCameraInfo(uint viewId);

    bool _old_test_render();
    
    void renderView(Bitmap & bitmap,const Bitmap & texture,uint viewId);
    
    void renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,const Bitmap & texture,uint viewId);
    
    void windowRender( uint view ) ;

private:
    
    uint addView(const View & view);
    
    bool isValidViewId(int id);
    
    bool isValidFaceId(int id);
    
    bool extractPhotoList();
    
    bool extractCameraInfoCreateViews();
    
    bool parseCameraInfo(std::ifstream & file,uint readCounter);
    
    bool parseLabelingLine(const std::string & line);
    
    bool get3Floats(std::vector<float> & tokens, std::ifstream & file);
    
    bool get2Ints(std::vector<int> & tokens,const std::string & line);
    
    bool writeLabelingToFile();
};



#endif /* TextureExtractor_hpp */


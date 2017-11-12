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



class TextureExtractor {
    
    Bitmap texture;
    
    Mesh mesh;
    
    std::string photoFolderPath;
    
    std::map<uint,View> views;

public:

    ~TextureExtractor();
    
    bool prepareViews(const std::string & cameraInfoPath, const std::string &  cameraListFilePath);
    
    bool selectViews();
    
    void setMesh (const Mesh & m){mesh = m;}
    
    void setPhotoFolderPath(const std::string &photoFolderPath){this->photoFolderPath = photoFolderPath;}
    
    bool generateTexture(const std::string & newTexturePath, int width, int height);
    
    uint numberOfViews(){return (uint)views.size();}
    
    //DEBUG
    
    void checkCameraInfo();

    void checkCameraInfo(uint viewId);

    bool _old_test_render();
    
    void renderView(Bitmap & bitmap,const Bitmap & texture,uint viewId);
    
    void renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,const Bitmap & texture,uint viewId);
    
    void windowRender( uint view ) ;

private:
    
    uint addView(const View & view);
    
    bool extractPhotoList(std::vector<std::string> & photoPaths,const std::string & cameraListFilePath);
    
    bool extractCameraInfoCreateViews(const std::vector<std::string> &photoPaths,const std::string & cameraInfoPath);
    
    bool parseCameraInfo(std::ifstream & file,uint readCounter);
    
    bool get3Floats(std::vector<float> & tokens, std::ifstream & file);
    
};



#endif /* TextureExtractor_hpp */


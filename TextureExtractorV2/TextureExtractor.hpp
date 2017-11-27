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
#include "PatchQuality.h"
#include "ExtractionWorker.hpp"

class TextureExtractor {
    
    Bitmap texture;
    
    Bitmap mask;
    
    Mesh mesh;
        
    std::map<uint,View> views;

    std::map<uint,glm::vec4> viewColors;
    
    Arguments arguments;
    
    std::map<uint,std::map<uint,PatchQuality>> dataCosts;
    
    std::map<uint,std::map<uint,glm::vec4>> faceViewAverages;
    
    std::map<uint,std::map<uint,glm::vec4>> colorSamples;
    
    std::map<uint,glm::vec4> colorAverages;
    
    std::map<uint,glm::vec4> faceAverages;
    
    ExtractionWorker worker;

public:

    ~TextureExtractor();
    
    bool prepareViews();
    
    bool calculateDataCosts();
    
    bool selectViews();
    
    void setMesh (const Mesh & m){mesh = m; worker.setMesh(&mesh);}
    
    bool generateTexture();
    
    uint numberOfViews(){return (uint)views.size();}
    
    void setArguments(const Arguments & arguments){this->arguments = arguments;}
 
    bool readLabelsFromFile();
    
    bool readDataCostsFromFile();
    
    //DEBUG
    
    void checkCameraInfo();

    void checkCameraInfo(uint viewId);

    bool _old_test_render();
    
    void renderView(Bitmap & bitmap,const Bitmap & texture,uint viewId);
    
    void renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,const Bitmap & texture,uint viewId);
    
    void windowRender( uint view ) ;

private:
    
    void extractAllFaces(Bitmap & labelTexture);
    
    void extendAllFaces();
    
    void applyGradientAllFaces(Bitmap & textureCopy, Bitmap & levelingTexture);
    
    bool mapMapGetLabeling();
    
    uint addView(const View & view);
    
    bool isValidViewId(int id);
    
    bool isValidFaceId(int id);
    
    bool extractPhotoList();
    
    bool extractCameraInfoCreateViews();
    
    bool parseCameraInfo(std::ifstream & file,uint readCounter);
    
    bool parseLabelingLine(const std::string & line);
    
    bool get3Floats(std::vector<float> & tokens, std::ifstream & file);
    
    bool get2Ints(std::vector<int> & tokens,const std::string & line);
    
    bool  parseFaceDataCost(std::ifstream & file);
    
    bool writeLabelingToFile();
    
    bool writeDataCostsToFile();
    
    void getSampleList(Bitmap & texture, Bitmap & mask);
};



#endif /* TextureExtractor_hpp */


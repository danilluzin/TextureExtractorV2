
//  main.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include <iostream>
#include "Timer.hpp"
#include "Mesh.hpp"
#include "TextureExtractor.hpp"
#include "Utils.h"

bool prepareMesh(Mesh & mesh,const std::string & objFilePath);

bool prepareViews( TextureExtractor & extractor, const std::string & cameraInfoPath, const std::string &  cameraListFilePath);

bool performViewSelection(TextureExtractor & extractor);

bool generateTexture(const std::string & newTexturePath, TextureExtractor & extractor, int width, int height);


int main(int argc, const char * argv[]) {
    TextureExtractor extractor;
    Timer mainTimer;
    mainTimer.start();
    
    std::string objFilePath = "resources/pig/pig_3_blender.obj";
    std::string cameraListFilePath = "resources/pig/list.txt";
    std::string cameraInfoPath = "resources/pig/bundle.rd.out";
    std::string newTexturePath = "resources/pig/derived/texture.txt";
    std::string photoFolderPath = "resources/pig";
    
    int textureWidth = 1000, textureHeight = 1000;


    Mesh mesh;
    bool meshIsOk = prepareMesh(mesh, objFilePath);
    if( !meshIsOk ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    extractor.setPhotoFolderPath(photoFolderPath);
    extractor.setMesh( mesh );
    bool viewsOK;
    viewsOK = prepareViews( extractor, cameraInfoPath, cameraListFilePath );
    if( !viewsOK ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    bool viewSelectionOK;
    viewSelectionOK = performViewSelection(extractor);
    if( !viewSelectionOK ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    bool textureOk;
    textureOk = generateTexture(newTexturePath, extractor, textureWidth, textureHeight);
    if( !textureOk ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    printBold(mainTimer.stopGetResults( "\nTottal run time " ));
}


bool generateTexture(const std::string & newTexturePath, TextureExtractor & extractor,int width, int height){
    Timer timer;
    timer.start();
    print("\nPerforming Texture Generation:\n");
    bool textureOk;
    textureOk = extractor.generateTexture(newTexturePath, width, height);
    if( !textureOk ){
        printBold(timer.stopGetResults( "\tTexture generation failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tLables generated.: " ));
    return true;
}


bool performViewSelection(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("\nPerforming Lable Assignment:\n");
    bool viewSelectionOK;
    viewSelectionOK = extractor.selectViews();
    if( !viewSelectionOK ){
        printBold(timer.stopGetResults( "\tLables generation failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tLables generated.: " ));
    return true;
}


bool prepareMesh(Mesh & mesh,const std::string & objFilePath){
    Timer timer;
    timer.start();
    print("\nReading and Preparing the Mesh from obj File:\n");
    bool meshIsOk;
    meshIsOk = mesh.initialize(objFilePath);
    if( !meshIsOk ){
        printBold(timer.stopGetResults( "\tMesh inicialization failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tMesh inicialized.: " ));
    return true;
}


bool prepareViews( TextureExtractor & extractor, const std::string & cameraInfoPath, const std::string &  cameraListFilePath){
    Timer timer;
    timer.start();
    std::cout <<"\nReading and Preparing Camera Views:\n";
    bool viewsAreOK;
    viewsAreOK = extractor.prepareViews( cameraInfoPath, cameraListFilePath);
    if( !viewsAreOK ){
        std::cout << timer.stopGetResults( "\tViews inicialization failed.: " );
        return false;
    }
    std::cout << timer.stopGetResults( "\tViews inicialized.: " );
    return true;
}




//  main.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include <iostream>
#include <glm/glm.hpp>
#include "Timer.hpp"
#include "Mesh.hpp"
#include "TextureExtractor.hpp"
#include "Utils.h"

bool prepareMesh(Mesh & mesh,const std::string & objFilePath);
bool prepareViews( TextureExtractor & extractor, const std::string & cameraInfoPath, const std::string &  cameraListFilePath);
bool performViewSelection(TextureExtractor & extractor);

int main(int argc, const char * argv[]) {
    TextureExtractor extractor;
    Timer mainTimer;
    mainTimer.start();
    
    std::string objFilePath = "./resources/monkey2.obj";
    std::string cameraListFilePath = "./resources/camera_list.txt";
    std::string cameraInfoPath = "./resources/camera_info.txt";

    Mesh mesh;
    bool meshIsOk = prepareMesh(mesh, objFilePath);
    if( !meshIsOk ){
        printBold(mainTimer.stopGetResults("Exited with error"));
        return -1;
    }
    
    extractor.setMesh( mesh );
    bool viewsOK;
    viewsOK = prepareViews( extractor, cameraInfoPath, cameraListFilePath );
    if( !viewsOK ){
        printBold(mainTimer.stopGetResults("Exited with error"));
        return -1;
    }
    
    bool viewSelectionOK;
    viewSelectionOK = performViewSelection(extractor);
    if( !viewSelectionOK ){
        printBold(mainTimer.stopGetResults("Exited with error"));
        return -1;
    }
    
    printBold(mainTimer.stopGetResults( "Tottal run time " ));
    
}

bool performViewSelection(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("Performing Lable Assignment\n");
    bool viewSelectionOK;
    viewSelectionOK = extractor.selectViews();
    if( !viewSelectionOK ){
        printBold(timer.stopGetResults( "Lables generation failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tLables generated.: " ));
    return true;
}


bool prepareMesh(Mesh & mesh,const std::string & objFilePath){
    Timer timer;
    timer.start();
    print("Reading and Preparing the Mesh from obj File\n");
    bool meshIsOk;
    meshIsOk = mesh.initialize(objFilePath);
    if( !meshIsOk ){
        printBold(timer.stopGetResults( "Mesh inicialization failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tMesh inicialized.: " ));
    return true;
}


bool prepareViews( TextureExtractor & extractor, const std::string & cameraInfoPath, const std::string &  cameraListFilePath){
    Timer timer;
    timer.start();
    std::cout <<"Reading and Preparing Camera Views\n";
    bool viewsAreOK;
    viewsAreOK = extractor.prepareViews( cameraInfoPath, cameraListFilePath);
    if( !viewsAreOK ){
        std::cout << timer.stopGetResults( "Views inicialization failed.: " );
        return false;
    }
    std::cout << timer.stopGetResults( "\tViews inicialized.: " );
    return true;
}



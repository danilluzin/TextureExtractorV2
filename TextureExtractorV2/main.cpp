
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
#include <numeric>
#include "Arguments.h"

bool prepareMesh(Mesh & mesh,const std::string & objFilePath);

bool prepareViews( TextureExtractor & extractor);

bool performViewSelection(TextureExtractor & extractor);

bool calculateDataCosts(TextureExtractor & extractor);

bool generateTexture(TextureExtractor & extractor);

void _renderViewsWithTexture(TextureExtractor & extractor);

bool calcDataCostsAndGetLebeling(TextureExtractor & extractor);

bool loadLabelsFromFile(TextureExtractor & extractor);

Arguments arguments;

bool justRender = true;

int main(int argc, const char * argv[]) {
    TextureExtractor extractor;
    Timer mainTimer;
    mainTimer.start();

    extractor.setArguments(arguments);


    Mesh mesh;
    bool meshIsOk = prepareMesh(mesh, arguments.objFilePath);
    if( !meshIsOk ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    extractor.setMesh( mesh );
    bool viewsOK;
    viewsOK = prepareViews( extractor);
    if( !viewsOK ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    if(justRender){
        
        _renderViewsWithTexture(extractor);
        return 1;
    }
    
    
    if(arguments.getLabelingFromFile){
        bool lebelingReadingOK;
        lebelingReadingOK = loadLabelsFromFile(extractor);
        if( !lebelingReadingOK ){
            printBold(mainTimer.stopGetResults("\nExited with error"));
            return -1;
        }
    }else{
        bool calculationOK;
        calculationOK = calcDataCostsAndGetLebeling(extractor);
        if( !calculationOK ){
            printBold(mainTimer.stopGetResults("\nExited with error"));
            return -1;
        }
    }
    
   
    
    bool textureOk;
    textureOk = generateTexture(extractor);
    if( !textureOk ){
        printBold(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    _renderViewsWithTexture(extractor);
    
    printBold(mainTimer.stopGetResults( "\nTottal run time " ));
}

void _renderViewsWithTexture(TextureExtractor & extractor){
    {
        Bitmap depth;
        Bitmap bitmap;
        Bitmap texture(arguments.newTexturePath);
//        std::vector<uint> photoSet={};
        //            std::vector<uint> photoSet={28,27};
        
//        extractor.renderViewAndDepth(bitmap, depth, texture, 43);
//        depth.toPPM("resources/slany/extract/res6/slany_d_43.ppm");
        std::vector<uint> photoSet(extractor.numberOfViews());
//        std::vector<uint> photoSet={1,2,3,44,5,46,7,8,9,30,31,12,13,22,51,16,50};
        std::iota(photoSet.begin(),photoSet.end(),1);
        for(int t=0;t<photoSet.size();t++){
            std::cout<<"\rRasterizing photos %"<<(100*((float)t/photoSet.size()))<<"     "<<std::flush;
            extractor.renderView(bitmap,texture, photoSet[t]);
            bitmap.toPPM("resources/slany/extract/res6/slany_" + std::to_string(photoSet[t]) + ".ppm");
        }
        std::cout<<"\rRasterizing photos %100      \n";
    }
}


bool calcDataCostsAndGetLebeling(TextureExtractor & extractor){
    
    if(arguments.getDataCostsFromFile){
        std::cout<<"\tGetting data costs from file:\n";
        bool dataCostsOK;
        dataCostsOK = extractor.readDataCostsFromFile();
        if( !dataCostsOK )
            return false;
    }else{
        bool dataCostsOK;
        dataCostsOK = calculateDataCosts( extractor );
        if( !dataCostsOK )
            return false;
    }
    
    bool viewSelectionOK;
    viewSelectionOK = performViewSelection(extractor);
    if( !viewSelectionOK )
        return false;
    return true;
}

bool loadLabelsFromFile(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("\nGetting labeling from file:\n");
    bool labelingOk;
    labelingOk = extractor.readLabelsFromFile();
    if( !labelingOk ){
        printBold(timer.stopGetResults( "\tLabeling reading failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tLabeling extracted.: " ));
    return true;
}

bool generateTexture(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("\nPerforming Texture Generation:\n");
    bool textureOk;
    textureOk = extractor.generateTexture();
    if( !textureOk ){
        printBold(timer.stopGetResults( "\tTexture generation failed.: " ));
        return false;
    }
    print(timer.stopGetResults( "\tTexture generated.: " ));
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


bool prepareViews( TextureExtractor & extractor){
    Timer timer;
    timer.start();
    std::cout <<"\nReading and Preparing Camera Views:\n";
    bool viewsAreOK;
    viewsAreOK = extractor.prepareViews();
    if( !viewsAreOK ){
        std::cout << timer.stopGetResults( "\tViews inicialization failed.: " );
        return false;
    }
    std::cout << timer.stopGetResults( "\tViews inicialized.: " );
    return true;
}


bool calculateDataCosts(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    std::cout <<"\nCalculating Data Costs:\n";
    bool dataCostsOK;
    dataCostsOK = extractor.calculateDataCosts();
    if( !dataCostsOK ){
        std::cout << timer.stopGetResults( "\tData Costs Calculation failed.: " );
        return false;
    }
    std::cout << timer.stopGetResults( "\tData Costs Calculated.: " );
    return true;
}



//DEBUG






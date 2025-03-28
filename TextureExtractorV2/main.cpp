
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

/*! \mainpage
 
 \section intro_sec Introduction
 
 This is the code documentation for a texture reconstruction tool.
 
 Tool generates texture images for 3D models using camera information from reconstruction tools and source images.
 
 Supported reconstruction tools:
 - Visual SFM
 - Bundler SFM
 - 3D laser scanning software developed in the CTU FEE Department of Computer Graphics and Interaction
 
 \section sec_u Usage
 Configuration of the tool is handled using .INI files.
 
 You can generate example .INI file using the following command:
 \code{.cpp}
 ./tex_extractor -genIni
 \endcode
 The complete explanation of .INI file can be found in the user manual.
 
 A tool can then be used as following:
 \code{.cpp}
 ./tex_extractor path/to/config.ini
 \endcode
 
 \section sec_i Instalation
 A tool can be build using CMake.
 
 Required libraries are
 - OpenCV https://opencv.org
 - GLM https://glm.g-truc.net/0.9.8/index.html
 - TBB https://www.threadingbuildingblocks.org
 */

/**
 * Prepare a mesh from obj file. Starts and stops a timer.
 */
bool prepareMesh(Mesh & mesh,const std::string & objFilePath);

/**
 * Reads and parses the .INI config from file.
 */
bool prepareConfig(int argc, const char * argv[]);

/**
 * Wrapper function. Starts the timer ask extractor to prepare views
 */
bool prepareViews( TextureExtractor & extractor);

/**
 * Wrapper function. Starts the timer ask extractor to perform view selection
 */
bool performViewSelection(TextureExtractor & extractor);

/**
 * Wrapper function. Starts the timer ask extractor to calculate data costs
 */
bool calculateDataCosts(TextureExtractor & extractor);

/**
 * Wrapper function. Starts the timer ask extractor to generate textures
 */
bool generateTexture(TextureExtractor & extractor);

/**
 * Wrapper DEBUG function. Starts the timer ask extractor to render views with texture
 */
void _renderViewsWithTexture(TextureExtractor & extractor);

/**
 * Wrapper function. Starts the timer ask extractor to calculate data costs and get labeling
 */
bool calcDataCostsAndGetLebeling(TextureExtractor & extractor);

/**
 * Wrapper function. Starts the timer ask extractor to load Labels from file
 */
bool loadLabelsFromFile(TextureExtractor & extractor);

Arguments arguments;
int pad = 0;
bool verbose = true;

/**
 * Checks command line flags.
 */
bool checkFlags(const char * argv[]);

int main(int argc, const char * argv[]) {
    
    print(COLOR_BOLD+"\n\t[[[  Texture Extractor  ]]]\n\n"+COLOR_RESET);
    
    TextureExtractor extractor;
    Timer mainTimer;
    mainTimer.start();
   
    if(argc >= 2){
        bool continueRun = checkFlags(argv);
        if(!continueRun)
            return 1;
    }
    
    bool configSuccessful;
    configSuccessful = prepareConfig(argc, argv);
    if(!configSuccessful){
        printError("Error occured when parsing the config. Stopping\n");
        printError(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }else{
        print(s(2)+"Config parsed\n");
    }
    

    Mesh mesh;
    bool meshIsOk = prepareMesh(mesh, arguments.objFilePath);
    if( !meshIsOk ){
        printError(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    extractor.setMesh( mesh );
    bool viewsOK;
    viewsOK = prepareViews( extractor);
    if( !viewsOK ){
        printError(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    if(arguments._justRender){
        _renderViewsWithTexture(extractor);
        return 1;
    }
    
    
    if(arguments.getLabelingFromFile){
        bool lebelingReadingOK;
        lebelingReadingOK = loadLabelsFromFile(extractor);
        if( !lebelingReadingOK ){
            printError(mainTimer.stopGetResults("\nExited with error"));
            return -1;
        }
    }else{
        bool calculationOK;
        calculationOK = calcDataCostsAndGetLebeling(extractor);
        if( !calculationOK ){
            printError(mainTimer.stopGetResults("\nExited with error"));
            return -1;
        }
    }

    
    bool textureOk;
    textureOk = generateTexture(extractor);
    if( !textureOk ){
        printError(mainTimer.stopGetResults("\nExited with error"));
        return -1;
    }
    
    print(COLOR_TEAL+COLOR_BOLD+mainTimer.stopGetResults( "\nTottal run time " ));
    
    if(arguments._renderInTheEnd)
        _renderViewsWithTexture(extractor);

}

bool prepareConfig(int argc, const char * argv[]){
    if(argc < 2){
        print(COLOR_RED+"No arguments provided\n"+COLOR_RESET+COLOR_BOLD+
              "Usage:\n"+std::string(argv[0])+" <ini config file path>\n"
              "To generate examaple ini file:\n"+std::string(argv[0])+" -genIni\n" );
        return false;
    }
    return arguments.initializeConfig(argv[1]);
}

bool calcDataCostsAndGetLebeling(TextureExtractor & extractor){
    
    if(arguments.getDataCostsFromFile){
        print("Getting data costs from file :\n");
        adjPad(2);
        bool dataCostsOK;
        dataCostsOK = extractor.readDataCostsFromFile();
        adjPad(-2);
        if( !dataCostsOK )
            return false;
        extractor.postprocessDataCosts();
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
    print("Getting labeling from file :\n");
    adjPad(2);
    bool labelingOk;
    labelingOk = extractor.readLabelsFromFile();
    if( !labelingOk ){
        printError(timer.stopGetResults( "Labeling reading failed : " ));
        adjPad(-2);
        return false;
    }
    print(COLOR_TEAL+timer.stopGetResults( "Labeling extracted : " ));
    adjPad(-2);
    return true;
}

bool generateTexture(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("Performing Texture Generation:\n");
    adjPad(2);
    bool textureOk;
    textureOk = extractor.generateTexture();
    if( !textureOk ){
        printError(timer.stopGetResults( "Texture generation failed : " ));
        adjPad(-2);
        return false;
    }
    print(COLOR_TEAL+timer.stopGetResults( "Texture generated : " ));
    adjPad(-2);
    return true;
}


bool performViewSelection(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("Performing Lable Assignment:\n");
    adjPad(2);
    bool viewSelectionOK;
    viewSelectionOK = extractor.selectViews();
    if( !viewSelectionOK ){
        printError(timer.stopGetResults( "Lables generation failed : " ));
        adjPad(-2);
        return false;
    }
    print(COLOR_TEAL+timer.stopGetResults( "Lables generated : " ));
    adjPad(-2);
    return true;
}


bool prepareMesh(Mesh & mesh,const std::string & objFilePath){
    Timer timer;
    timer.start();
    print("Reading and preparing the mesh from obj file :\n");
    adjPad(2);
    bool meshIsOk;
    meshIsOk = mesh.initialize(objFilePath);
    if( !meshIsOk ){
        printError(timer.stopGetResults( "Mesh inicialization failed : " ));
        adjPad(-2);
        return false;
    }
    print(COLOR_TEAL+timer.stopGetResults( "Mesh inicialized : " ));
    adjPad(-2);
    return true;
}


bool prepareViews( TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("Reading and preparing camera views :\n");
    adjPad(2);
    bool viewsAreOK;
    viewsAreOK = extractor.prepareViews();
    if( !viewsAreOK ){
        printError(timer.stopGetResults( "Views inicialization failed : "));
        return false;
    }
    print(COLOR_TEAL+ timer.stopGetResults( "Views inicialized : " ));
    adjPad(-2);
    return true;
}


bool calculateDataCosts(TextureExtractor & extractor){
    Timer timer;
    timer.start();
    print("Calculating Data Costs :\n");
    adjPad(2);
    bool dataCostsOK;
    dataCostsOK = extractor.calculateDataCosts();
    if( !dataCostsOK ){
       printError(timer.stopGetResults( "Data Costs Calculation failed : "));
        return false;
    }
    print(COLOR_TEAL+timer.stopGetResults( "Data Costs Calculated : "));
    adjPad(-2);
    return true;
}


bool checkFlags(const char * argv[]){
    std::string flag = std::string(argv[1]);
    if(flag == "-genIni"){
        print("Generating ini :\n");
        adjPad(2);
        bool generationOk;
        generationOk = arguments.generateIni();
        if(!generationOk){
            printError("Error when generationg example ini config\n");
        }else{
            print(COLOR_BOLD+"Example ini config generated\n");
        }
        adjPad(-2);
        return false;
    }
    return true;
}


//// DEBUG ////


void _renderViewsWithTexture(TextureExtractor & extractor){
    {
        Bitmap depth;
        Bitmap bitmap;
        //        std::vector<uint> photoSet={};
        //            std::vector<uint> photoSet={28,27};
        //        std::cout<<"depth\n";
        //        extractor.renderViewAndDepth(bitmap, depth, 27);
        //    depth.save("working_resources/slany/derived1/slany_d_27.png");
        //        std::cout<<"done\n";
        
        std::vector<uint> photoSet(extractor.numberOfViews());
        //        std::vector<uint> photoSet={1,2,3,44,5,46,7,8,9,30,31,12,13,22,51,16,50};
        std::iota(photoSet.begin(),photoSet.end(),1);
        for(int t=0;t<photoSet.size();t++){
            std::cout<<"\rRasterizing photos %"<<(100*((float)t/photoSet.size()))<<"     "<<std::flush;
            extractor.renderView(bitmap, photoSet[t]);
            bitmap.save(arguments.resultRenderFolder+"/"+arguments.projectName+"_" + std::to_string(photoSet[t]) + ".png");
        }
        std::cout<<"\rRasterizing photos %100      \n";
    }
}



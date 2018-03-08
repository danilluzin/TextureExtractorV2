//
//  Arguments.h
//  TextureExtractorV2
//
//  Created by Danil Luzin on 18/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Arguments_h
#define Arguments_h

#include <iostream>
#include "INIReader.h"
#include "Utils.h"
#include<fstream>
#include<iostream>

struct Arguments{
    
    int textureWidth;
    int textureHeight;
    
    int threadCount;
    int BVHMinNode;

    std::string objFilePath ;
    std::string cameraListFilePath;
    std::string cameraInfoPath;
    std::string newTextureFolderPath;
    std::string photoFolderPath;
    std::string newLabelingFilePath;
    std::string labelingFilePath;
    std::string dataCostsFilePath;
    std::string newDataCostsFilePath;
    
    std::string projectName;
    std::string resultRenderFolder;
    std::string rasterLabelAssignmentFolder;
    std::string imageFormat;
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
    bool getDataCostsFromFile;
    bool writeDataCostsToFile;
    
    bool doGloabalAdjustment;
    bool doSeamLeveling;
    bool doTextureExtension;
    bool doColorConsistency;
    
    float colorConsistencyThreshold;
    
    bool genLebelingTexture;
    bool genLevelingTexture;
    bool genMaskTexture;
    bool genGlobalTexture;
    bool genRawTexture;
    
    bool addProjectNameToFiles;
    
    bool _justRender;
    bool _renderInTheEnd;
    bool rasterLabelAssignment;
    
    
    bool initializeConfig(std::string filename){
        INIReader reader(filename);
        
        if (reader.ParseError() < 0) {
            printError("ERROR: Can't load config file: "+filename+"\n");
            return false;
        }
        print("Config loaded from " + filename +". Parsing...\n");
        //basics
        objFilePath = reader.Get("basics", "objFilePath", "UNKNOWN");
        if(objFilePath == "UNKNOWN"){
            printError("ERROR: objFilePath is required in the .ini file\n");
            return false;
        }
        cameraListFilePath = reader.Get("basics", "cameraListFilePath", "UNKNOWN");
        if(cameraListFilePath == "UNKNOWN"){
            printError("ERROR: cameraListFilePath is required in the .ini file\n");
            return false;
        }
        
        cameraInfoPath = reader.Get("basics", "cameraInfoPath", "UNKNOWN");
        if(cameraInfoPath == "UNKNOWN"){
            printError("ERROR: cameraInfoPath is required in the .ini file\n");
            return false;
        }
        
        photoFolderPath = reader.Get("basics", "photoFolderPath", "UNKNOWN");
        if(photoFolderPath == "UNKNOWN"){
            printError("ERROR: photoFolderPath is required in the .ini file\n");
            return false;
        }
        
        newTextureFolderPath = reader.Get("basics", "newTextureFolderPath", "UNKNOWN");
        if(photoFolderPath == "UNKNOWN"){
            printError("ERROR: newTextureFolderPath is required in the .ini file\n");
            return false;
        }
        textureHeight = (int)reader.GetInteger("basics", "textureHeight", -1);
        if(textureHeight == -1){
            printWarning("WARNING: no textureHeight provided in the .ini file. Defaulting to 1000\n");
            textureHeight = 1000;
        }
        textureWidth = (int)reader.GetInteger("basics", "textureWidth", -1);
        if(textureWidth == -1){
            printWarning("WARNING: no textureWidth provided in the .ini file. Defaulting to 1000\n");
            textureWidth = 1000;
        }
        

        getLabelingFromFile = reader.GetBoolean("basics", "getLabelingFromFile", false);
        if(getLabelingFromFile == true){
            labelingFilePath = reader.Get("optional", "labelingFilePath", "UNKNOWN");
            if(labelingFilePath == "UNKNOWN"){
            printError("ERROR: getLabelingFromFile was set to TRUE, but the labelingFilePath was not provided the .ini file\n");
                return false;
            }
        }
        
        writeLabelingToFile = reader.GetBoolean("basics", "writeLabelingToFile", false);
        if(writeLabelingToFile == true){
            newLabelingFilePath = reader.Get("optional", "newLabelingFilePath", "UNKNOWN");
            if(newLabelingFilePath == "UNKNOWN"){
                printWarning("WARNING: writeLabelingToFile was set to TRUE, but newLabelingFilePath was not provided in the .ini file. Defaulting to \"new_labeling.txt\"\n");
                newLabelingFilePath = "new_labeling.txt";
            }
        }
        
        
        getDataCostsFromFile = reader.GetBoolean("basics", "getDataCostsFromFile", false);
        if(getDataCostsFromFile == true){
            dataCostsFilePath = reader.Get("optional", "dataCostsFilePath", "UNKNOWN");
            if(dataCostsFilePath == "UNKNOWN"){
                printError("ERROR: getDataCostsFromFile was set to TRUE, but the dataCostsFilePath was not provided the .ini file\n");
                return false;
            }
        }
        
        writeDataCostsToFile = reader.GetBoolean("basics", "writeDataCostsToFile", false);
        if(writeDataCostsToFile == true){
            newDataCostsFilePath = reader.Get("optional", "newDataCostsFilePath", "UNKNOWN");
            if(newDataCostsFilePath == "UNKNOWN"){
                printWarning("WARNING: writeDataCostsToFile was set to TRUE, but newDataCostsFilePath was not provided in the .ini file. Defaulting to \"new_datacosts.txt\"\n");
                newDataCostsFilePath = "new_datacosts.txt";
            }
        }
        
        verbose = reader.GetBoolean("basic", "verbose", true);

        BVHMinNode = (int)reader.GetInteger("perfomance","BVHMinNode",200);
        threadCount = (int)reader.GetInteger("basics", "threadCount", 1);
        if(threadCount < 1){
            printWarning("WARNING: Invalid number of working threads(threadCount). Defaulting to 1\n");
            threadCount = 1;
        }
        
        
        imageFormat = reader.Get("basics", "imageFormat", "UNKNOWN");
        if(imageFormat == "UNKNOWN"){
            printWarning("WARNING: imageFormat was not specified. Defaulting to png\n");
            imageFormat = "png";
        }
        
        projectName = reader.Get("optional", "projectName", "projectName");
        addProjectNameToFiles = reader.GetBoolean("optional", "addProjectNameToFiles", false);
        
        doGloabalAdjustment = reader.GetBoolean("optional", "doGloabalAdjustment", true);
        doSeamLeveling = reader.GetBoolean("optional", "doSeamLeveling", true);
        doTextureExtension = reader.GetBoolean("optional", "doTextureExtension", true);
        
        doColorConsistency = reader.GetBoolean("optional", "doColorConsistency", true);
        if(doColorConsistency == true){
            colorConsistencyThreshold = reader.GetReal("optional", "colorConsistencyThreshold", -1.0);
            if(colorConsistencyThreshold==-1.0){
                printWarning("WARNING: Valid colorConsistencyThreshold was not specified. Defaulting to 0.5\n");
                colorConsistencyThreshold = 0.5;
            }
        }
        
        
        //debug
        genRawTexture = reader.GetBoolean("debug", "genRawTexture", false);
        genLevelingTexture = reader.GetBoolean("debug", "genLevelingTexture", false);
        genMaskTexture = reader.GetBoolean("debug", "genMaskTexture", false);
        genLebelingTexture = reader.GetBoolean("debug", "genLebelingTexture", false);
        genGlobalTexture = reader.GetBoolean("debug", "genGlobalTexture", false);
        
        
         _justRender = reader.GetBoolean("debug", "justRender", false);
         _renderInTheEnd = reader.GetBoolean("debug", "renderInTheEnd", false);
        
        if(_justRender || _renderInTheEnd){
            resultRenderFolder = reader.Get("debug", "resultRenderFolder", "UNKNOWN");
            if(resultRenderFolder == "UNKNOWN"){
                printError("ERROR: justRender or renderInTheEnd was set to TRUE, but the resultRenderFolder was not provided the .ini file\n");
                return false;
            }
        }
        
        return true;
    }
    
    
    std::string appendix(){
        if(addProjectNameToFiles)
            return projectName+"_";
        return "";
    }
    
    std::string genLevelingTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_leveling." + imageFormat;
    }
    
    std::string genMaskTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_mask." + imageFormat;
    }
    
    
    std::string genRawTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_raw." + imageFormat;
    }
    
    std::string genLebelingTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_labeling." + imageFormat;
    }
    
    std::string genFinalTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_texture." + imageFormat;
    }
    
    std::string genGlobalTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_global." + imageFormat;
    }
    
    bool generateIni(){
        std::string iniExampleFilename = "example.ini";
        std::ofstream iniFile;
        iniFile.open (iniExampleFilename);
        if(!iniFile.is_open()){
            printError("Error openning file to write exampleIni!\n");
            return false;
        }
        iniFile << exampleIni;
        if(!iniFile.good()){
            printError("Error occured when writing exampleIni!\n");
            return false;
        }
        iniFile.close();
        return true;
    }
    
};

extern Arguments arguments;


#endif /* Arguments_h */

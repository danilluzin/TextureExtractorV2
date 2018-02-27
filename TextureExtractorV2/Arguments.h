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

//TODO: message by enum

struct Arguments{
    
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

        
        //debug
        projectName = reader.Get("basic", "projectName", "projectName");
        addProjectNameToFiles = reader.GetBoolean("basic", "addProjectNameToFiles", false);
        
        genRawTexture = reader.GetBoolean("debug", "genRawTexture", false);
        if(genRawTexture == true){
            rawTextureFilePath = reader.Get("optional", "rawTextureFilePath", "rawTexture");
        }
        
        genLevelingTexture = reader.GetBoolean("debug", "genLevelingTexture", false);
        if(genLevelingTexture == true){
            levelingTextureFilePath = reader.Get("optional", "levelingTextureFilePath", "levelingTexture");
        }
        
        genMaskTexture = reader.GetBoolean("debug", "genMaskTexture", false);
        if(genMaskTexture == true){
            maskTextureFilePath = reader.Get("optional", "maskTextureFilePath", "maskTexture");
        }
        
        genLebelingTexture = reader.GetBoolean("debug", "genLebelingTexture", false);
        if(genLebelingTexture == true){
            viewAssignmentFilePath = reader.Get("optional", "viewAssignmentFilePath", "labelingTexture");
        }
        
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
    
    int textureWidth;
    int textureHeight;
    std::string objFilePath ;
    std::string cameraListFilePath;
    std::string cameraInfoPath;
    std::string newTextureFolderPath;
    std::string photoFolderPath;
    std::string newLabelingFilePath;
    std::string labelingFilePath;
    std::string dataCostsFilePath;
    std::string newDataCostsFilePath;
    std::string viewAssignmentFilePath;
    std::string levelingTextureFilePath;
    std::string maskTextureFilePath;
    std::string rawTextureFilePath;
    std::string projectName;
    std::string resultRenderFolder;
    std::string gloabalAdjustementPath;
    std::string rasterLabelAssignmentFolder;
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
    bool getDataCostsFromFile;
    bool writeDataCostsToFile;
    bool genLebelingTexture;
    bool genLevelingTexture;
    bool genMaskTexture;
    bool rasterLabelAssignment;
    bool addProjectNameToFiles;
    
    bool genRawTexture;
    bool _justRender;
    bool _renderInTheEnd;
};

static Arguments arguments;


#endif /* Arguments_h */

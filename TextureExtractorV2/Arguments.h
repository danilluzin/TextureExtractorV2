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
/**
 * Structure that manages all the argument checking and reading.
 */

struct Arguments{
    /** output file texture width */
    int textureWidth;
    /** output file texture height  */
    int textureHeight;
    
    /** number of working threads used during data cost extraction*/
    int threadCount;
    
    /** size of the minimal node. Used for constructing Bounding Volume Hierrachy */
    int BVHMinNode;

    /** path to an obj file */
    std::string objFilePath ;
    /** path to list that contrains paths to photos */
    std::string cameraListFilePath;
    /** path to blundler file 'bundle.rd.out' */
    std::string cameraInfoPath;
    /** path to the output directory */
    std::string newTextureFolderPath;
    /** folder that gets added to the beginng of the path for the photograph paths in the cameraListFilePath*/
    std::string photoFolderPath;
    /** path and name of the output labeling */
    std::string newLabelingFilePath;
    /** path to the labeling file for reading */
    std::string labelingFilePath;
    /** path to the data cost file for reading */
    std::string dataCostsFilePath;
    /** path and name of ned data cost file */
    std::string newDataCostsFilePath;
    
    /** project name for a given dataset */
    std::string projectName;
    /** folder to outtput debug denders */
    std::string resultRenderFolder;
    
    /** string containing the output image format */
    std::string imageFormat;
    
    /** flag that defines if tool will attempt to get labeling from the file */
    bool getLabelingFromFile;
    /** flag that defines if tool will attempt to write labeling into the file */
    bool writeLabelingToFile;
    /** flag that defines if tool will attempt to get data costs from the file */
    bool getDataCostsFromFile;
    /** flag that defines if tool will attempt to write data costs into the file */
    bool writeDataCostsToFile;
    
    /** flag that defines if tool will perform global adjestement step */
    bool doGloabalAdjustment;
    /** flag that defines if tool will perform seam leveling step */
    bool doSeamLeveling;
    /** flag that defines if tool will perform texture extension step */
    bool doTextureExtension;
    /** flag that defines if tool will perform color consistency check  */
    bool doColorConsistency;
    
    /** delta derivation from the mean limit. Makes color consistency check stricter */
    float colorConsistencyThreshold;
    
    /** DEBUG: should the 'labeling' texture be generated on its on */
    bool genLebelingTexture;
    /** DEBUG: should the 'leveling' texture be generated on its on */
    bool genLevelingTexture;
    /** DEBUG: should the 'mask' texture be generated on its on */
    bool genMaskTexture;
    /** DEBUG: should the 'global' texture be generated on its on */
    bool genGlobalTexture;
    /** DEBUG: should the 'raw' texture be generated on its on */
    bool genRawTexture;
    
    /** flag that defines if tool will do a strict oclusion check. If TRUE even if a fragment is ocluded by only one pixel its triangle-view pair get discarded. if FALSE only the view-triangle pairs where triangle is fully invisible get discarded */
    bool strictOclusionCheck;
    
    /** flag that defines if tool should add projectName to files  */
    bool addProjectNameToFiles;
    
    /** DEBUG: ask program to only render test renders */
    bool _justRender;
    /** DEBUG: ask program to render test renders after program execution*/
    bool _renderInTheEnd;
    

    /**
     * Read and verify the consiguration file§
     * @param filename Pacth to the ini config
     * @return true if all went well.
     */
    bool initializeConfig(std::string filename){
        INIReader reader(filename);
        
        if (reader.ParseError() < 0) {
            printError("ERROR: Can't load config file: "+filename+"\n");
            return false;
        }
        print("Config loaded from " + filename +". Parsing :\n");
        //basics
        objFilePath = reader.Get("basics", "objFilePath", "UNKNOWN");
        if(objFilePath == "UNKNOWN"||objFilePath == ""){
            printError("ERROR: objFilePath is required in the .ini file\n");
            return false;
        }
        cameraListFilePath = reader.Get("basics", "cameraListFilePath", "UNKNOWN");
        if(cameraListFilePath == "UNKNOWN"||cameraListFilePath == ""){
            printError("ERROR: cameraListFilePath is required in the .ini file\n");
            return false;
        }
        
        cameraInfoPath = reader.Get("basics", "cameraInfoPath", "UNKNOWN");
        if(cameraInfoPath == "UNKNOWN"||cameraInfoPath == ""){
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
        

        projectName = reader.Get("optional", "projectName", "projectName");
        addProjectNameToFiles = reader.GetBoolean("optional", "addProjectNameToFiles", false);
        
        getLabelingFromFile = reader.GetBoolean("basics", "getLabelingFromFile", false);
        if(getLabelingFromFile == true){
            labelingFilePath = reader.Get("optional", "labelingFilePath", "UNKNOWN");
            if(labelingFilePath == "UNKNOWN" || labelingFilePath == ""){
                labelingFilePath = genDeaultLabelingPath();
            printWarning("WARNING: getLabelingFromFile was set to TRUE, but the labelingFilePath was not provided the .ini file\n Defaulting to \""+labelingFilePath+"\"\n ");
            }
        }
        
        writeLabelingToFile = reader.GetBoolean("basics", "writeLabelingToFile", false);
        newLabelingFilePath = genDeaultLabelingPath();
        
        
        getDataCostsFromFile = reader.GetBoolean("basics", "getDataCostsFromFile", false);
        if(getDataCostsFromFile == true){
            dataCostsFilePath = reader.Get("optional", "dataCostsFilePath", "UNKNOWN");
            if(dataCostsFilePath == "UNKNOWN" || dataCostsFilePath == ""){
                dataCostsFilePath = genDeaultDataCostPath();
                printWarning("WARNING: getDataCostsFromFile was set to TRUE, but the dataCostsFilePath was not provided the .ini file\n Defaulting to \""+dataCostsFilePath+"\"\n ");
            }
        }
        
        writeDataCostsToFile = reader.GetBoolean("basics", "writeDataCostsToFile", false);
        newDataCostsFilePath = genDeaultDataCostPath();
        
        verbose = reader.GetBoolean("basics", "verbose", true);
        if(!verbose){
            std::cout<<"Parsed\nVerbose = FALSE. Only warning and errors will be displayed\n";
        }

        BVHMinNode = (int)reader.GetInteger("perfomance","BVHMinNode",200);
        threadCount = (int)reader.GetInteger("basics", "threadCount", 1);
        if(threadCount < 1){
            printWarning("WARNING: Invalid number of working threads(threadCount). Defaulting to 1\n");
            threadCount = 1;
        }
        
        
        imageFormat = reader.Get("basics", "imageFormat", "UNKNOWN");
        if(imageFormat == "UNKNOWN"||imageFormat == ""){
            printWarning("WARNING: imageFormat was not specified. Defaulting to png\n");
            imageFormat = "png";
        }
        
        
        
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
        
        //TODO: get strick occlusioncheck
        strictOclusionCheck = false;
        
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
    
    /**
     * Return file appendix to add to the begining of file
     */
    std::string appendix(){
        if(addProjectNameToFiles)
            return projectName+"_";
        return "";
    }
    
    
    /**
     * Returns path to leveling texture for an object
     * @param objName object name.
     * @return path and name for a new leveling texture
     */
    std::string genLevelingTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_leveling." + imageFormat;
    }
    
    /**
     * Returns path to mask texture for an object
     * @param objName object name.
     * @return path and name for a new mask texture
     */
    std::string genMaskTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_mask." + imageFormat;
    }
    
    /**
     * Returns path to raw texture for an object
     * @param objName object name.
     * @return path and name for a new raw texture
     */
    std::string genRawTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_raw." + imageFormat;
    }
    
    /**
     * Returns path to lebeling texture for an object
     * @param objName object name.
     * @return path and name for a new lebeling texture
     */
    std::string genLebelingTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_labeling." + imageFormat;
    }
    
    /**
     * Returns path to final texture for an object
     * @param objName object name.
     * @return path and name for a new final texture
     */
    std::string genFinalTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_texture." + imageFormat;
    }
    
    /**
     * Returns path to global texture for an object
     * @param objName object name.
     * @return path and name for a new global texture
     */
    std::string genGlobalTexturePath(std::string objName){
        return newTextureFolderPath + "/" + appendix() + objName + "_global." + imageFormat;
    }
    
    /**
     * Returns a default path to data cost file
     * @return path and name for a data cost file
     */
    std::string genDeaultDataCostPath(){
        return newTextureFolderPath + "/" + appendix() + "datacosts.txt";
    }
    
    /**
     * Returns a default path to labeling file
     * @return path and name for a labeling file
     */
    std::string genDeaultLabelingPath(){
        return newTextureFolderPath + "/" + appendix() + "labeling.txt";
    }
    
    /**
     * Generates an example ini file
     * @return true if generation successful
     */
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

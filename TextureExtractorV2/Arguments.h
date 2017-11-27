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

struct Arguments{
    Arguments(){
        //TODO: proper arg getting
//        
//        objFilePath = "./resources/pig/pig_3_blender.obj";
//        cameraListFilePath = "resources/pig/list.txt";
//        cameraInfoPath = "resources/pig/bundle.rd.out";
//        newTexturePath = "resources/pig/derived/texture_sv13_dis.ppm";
//        photoFolderPath = "resources/pig";
//        newLabelingFilePath = "resources/pig/derived/new_labeling.txt";
//        labelingFilePath = "resources/pig/derived/new_labeling.txt";
//        dataCostsFilePath = "resources/pig/derived/dataCosts_small.txt";
//        newDataCostsFilePath = "resources/pig/derived/sv13_dis_dataCosts.txt";
//        viewAssignmentFilePath = "resources/pig/derived/view_labeling.ppm";
//        levelingTextureFilePath = "resources/pig/derived/leveling.ppm";
//        maskTextureFilePath = "resources/pig/derived/mask.ppm";
//        rawTextureFilePath = "resources/pig/derived/raw.ppm";
//        resultRenderFolder = "resources/pig/extract/res2/";
//        projectName = "pig";
        
        
//                objFilePath = "./resources/pig2/pig2_3_blender.obj";
//                cameraListFilePath = "resources/pig2/list.txt";
//                cameraInfoPath = "resources/pig2/bundle.rd.out";
//                newTexturePath = "resources/pig2/derived/new_texture.ppm";
//                photoFolderPath = "resources/pig2";
//                newLabelingFilePath = "resources/pig2/derived/new_labeling.txt";
//                labelingFilePath = "resources/pig2/derived/new_labeling.txt";
//                dataCostsFilePath = "resources/pig2/derived/dataCosts_small.txt";
//                newDataCostsFilePath = "resources/pig2/derived/new_dataCosts.txt";
//                viewAssignmentFilePath = "resources/pig2/derived/view_labeling.ppm";
//                levelingTextureFilePath = "resources/pig2/derived/leveling.ppm";
//                maskTextureFilePath = "resources/pig/2derived/mask.ppm";
//                rawTextureFilePath = "resources/pig2/derived/raw.ppm";
//                resultRenderFolder = "resources/pig2/extract/res/";
//                projectName = "pig2";
        
        
        
        objFilePath = "resources/slany/slany_pieremoved.obj";
        cameraListFilePath = "resources/slany/list3.txt";
        cameraInfoPath = "resources/slany/bundle.rd.out";
        newTexturePath = "resources/slany/derived/out_ext2_texture_sv13.ppm";
        photoFolderPath = "resources/slany";
        newLabelingFilePath = "resources/slany/derived/new_labeling.txt";
        labelingFilePath = "resources/slany/derived/new_labeling.txt";
        dataCostsFilePath = "resources/slany/derived/dataCosts.txt";
        newDataCostsFilePath = "resources/slany/derived/out_dataCosts.txt";
        viewAssignmentFilePath = "resources/slany/derived/view_labeling.ppm";
        levelingTextureFilePath = "resources/slany/derived/leveling.ppm";
        maskTextureFilePath = "resources/slany/derived/mask.ppm";
        rawTextureFilePath = "resources/slany/derived/raw.ppm";
        resultRenderFolder = "resources/slany/extract/res9/";
        gloabalAdjustementPath = "resources/slany/derived/gloabalAdjustement.ppm";
        projectName = "slany";
        
//        objFilePath = "resources/slany2/slany_pieremoved.obj";
//        cameraListFilePath = "resources/slany2/list3.txt";
//        cameraInfoPath = "resources/slany2/s.bundle.rd.out";
//        newTexturePath = "resources/slany2/derived/texture3.ppm";
//        photoFolderPath = "resources/slany2";
//        newLabelingFilePath = "resources/slany2/derived/new_labeling.txt";
//        labelingFilePath = "resources/slany2/derived/labeling.txt";
//        dataCostsFilePath = "resources/slany2/derived/dataCosts.txt";
//        newDataCostsFilePath = "resources/slany2/derived/new_dataCosts.txt";
        
        textureWidth = 4000;
        textureHeight = 4000;
        getLabelingFromFile = true;
        writeLabelingToFile = true;
        
        getDataCostsFromFile = false;
        writeDataCostsToFile = true;
        genRawTexture = true;
        genLevelingTexture = true;
        genMaskTexture = true;
        genLebelingTexture = true;
        rasterLabelAssignment = false;
    }
    
    int textureWidth;
    int textureHeight;
    std::string objFilePath ;
    std::string cameraListFilePath;
    std::string cameraInfoPath;
    std::string newTexturePath;
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
    
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
    bool getDataCostsFromFile;
    bool writeDataCostsToFile;
    bool genLebelingTexture;
    bool genLevelingTexture;
    bool genMaskTexture;
    bool rasterLabelAssignment;
    bool genRawTexture;
};




#endif /* Arguments_h */

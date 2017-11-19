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
        
            objFilePath = "./resources/pig/pig_3_blender.obj";
            cameraListFilePath = "resources/pig/list.txt";
            cameraInfoPath = "resources/pig/bundle.rd.out";
            newTexturePath = "resources/pig/derived/texture2.ppm";
            photoFolderPath = "resources/pig";
            newLabelingFilePath = "resources/pig/derived/new_labeling.txt";
            labelingFilePath = "resources/pig/derived/labeling.txt";
        
//        objFilePath = "resources/slany/slany_blender_0.obj";
//        cameraListFilePath = "resources/slany/list2.txt";
//        cameraInfoPath = "resources/slany/bundle.rd.out";
//        newTexturePath = "resources/slany/derived/texture2.ppm";
//        photoFolderPath = "resources/slany";
//        newLabelingFilePath = "resources/lany/derived/new_labeling.txt";
//        labelingFilePath = "resources/lany/derived/labeling.txt";
        
        textureWidth = 4000;
        textureHeight = 4000;
        getLabelingFromFile = true;
        writeLabelingToFile = true;
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
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
};




#endif /* Arguments_h */

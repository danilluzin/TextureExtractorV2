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
        
        objFilePath = "resources/slany/slany_blender_0.obj";
        cameraListFilePath = "resources/slany/list2.txt";
        cameraInfoPath = "resources/slany/bundle.rd.out";
        newTexturePath = "resources/slany/derived/texture2.ppm";
        photoFolderPath = "resources/slany";
        newLabelingFilePath = "resources/lany/derived/new_labeling.txt"
        labelingFilePath = "resources/lany/derived/labeling.txt"
        
        getLabelingFromFile = false;
        writeLabelingToFile = true;
    }
    std::string objFilePath ;
    std::string cameraListFilePath;
    std::string cameraInfoPath;
    std::string newTexturePath;
    std::string photoFolderPath;
    std::string newLabelingFilePath;
    std::string labelingFilePath;
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
}




#endif /* Arguments_h */

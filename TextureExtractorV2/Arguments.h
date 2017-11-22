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
//        labelingFilePath = "resources/pig/derived/labeling.txt";
//        dataCostsFilePath = "resources/pig/derived/dataCosts_small.txt";
//        newDataCostsFilePath = "resources/pig/derived/sv13_dis_dataCosts.txt";
//
        objFilePath = "resources/slany/slany_blender_1.obj";
        cameraListFilePath = "resources/slany/list3.txt";
        cameraInfoPath = "resources/slany/bundle.rd.out";
        newTexturePath = "resources/slany/derived/texture_sv13.ppm";
        photoFolderPath = "resources/slany";
        newLabelingFilePath = "resources/slany/derived/new_labeling.txt";
        labelingFilePath = "resources/slany/derived/labeling.txt";
        dataCostsFilePath = "resources/slany/derived/dataCosts.txt";
        newDataCostsFilePath = "resources/slany/derived/new_dataCosts.txt";

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
        getLabelingFromFile = false;
        writeLabelingToFile = true;
        
        getDataCostsFromFile = false;
        writeDataCostsToFile = true;
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
    
    
    bool getLabelingFromFile;
    bool writeLabelingToFile;
    bool getDataCostsFromFile;
    bool writeDataCostsToFile;
};




#endif /* Arguments_h */

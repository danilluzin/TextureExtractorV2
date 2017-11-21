//
//  DataCostExtractionManager.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 21/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef DataCostExtractionManager_hpp
#define DataCostExtractionManager_hpp

#include "DataCostsExtractor.hpp"
#include <mutex>

class DataCostExtractionManager{
public:
    uint * progressCounter;
    uint totalViewCount;
    const Mesh & mesh;
    std::mutex * mtx;
    DataCostExtractionManager(const Mesh & mesh) : mesh(mesh){}
    std::map<uint,std::map<uint,float>> * dataCosts;
    std::vector<View *> viewsToDo;
    void doWork();
};

#endif /* DataCostExtractionManager_hpp */

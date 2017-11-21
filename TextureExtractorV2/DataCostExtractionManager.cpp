//
//  DataCostExtractionManager.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 21/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "DataCostExtractionManager.hpp"

void DataCostExtractionManager::doWork(){
    for(auto v : viewsToDo){
        
        View & view = *v;
        DataCostsExtractor extractor(mesh,view);
        (*dataCosts)[v->id] = extractor.calculateCosts();
        
        mtx->lock();
        *progressCounter = *progressCounter + 1;
        std::cout<<"\rProgress %"<<(100*((float)(*progressCounter)/totalViewCount))<<"     "<<std::flush;
        mtx->unlock();
    }
}

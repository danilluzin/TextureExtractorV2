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
        
        float progress = (100*((float)(*progressCounter)/totalViewCount));
        printOver(COLOR_TEAL+"["+fitPercent(progress)+"%]"+COLOR_RESET + " Getting Data Costs");
        
//        std::cout<<"\rProgress %"<<(100*((float)(*progressCounter)/totalViewCount))<<"     "<<std::flush;
        mtx->unlock();
    }
}

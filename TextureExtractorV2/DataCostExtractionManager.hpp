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

/**
 * Class that is responsible for keeping track of the workload and performs the main thread loop in the doWork method, and a DataCostsExtractor class perform an extraction for one source image itself.
 */
class DataCostExtractionManager{
public:
    /** Counter for keeping track of progress. is shared between parallel mangaers. Gets incremented i nthe mutex protected part.  */
    uint * progressCounter;
    
    /** Tottal number of views a mangager has to analyze. It is its workload */
    uint totalViewCount;
    
    /** Work mesh */
    const Mesh & mesh;
    
    /** Common mutex between extraction managers. USed to report the progress */
    std::mutex * mtx;
    
    DataCostExtractionManager(const Mesh & mesh) : mesh(mesh){}
    
    /** Set of data costs for every triangle visible from one particular source image. Those sets are combined into one dataCosts set local to a DataCostExtractionManager. This means, that since we have as many DataCostExtractionManagers as threads, we have to later merge them. */
    std::map<uint,std::map<uint,PatchQuality>> * dataCosts;
    
    /** List of views a managher should do. It is its workload */
    std::vector<View *> viewsToDo;
    
    /** Main function that is called in the begging of the thread */
    void doWork();
};

#endif /* DataCostExtractionManager_hpp */

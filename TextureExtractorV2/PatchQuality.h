//
//  DataCost.h
//  TextureExtractorV2
//
//  Created by Danil Luzin on 19/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef DataCost_h
#define DataCost_h

struct PatchQuality {
    uint sampleCount = 0;
    uint potentialCount = 0;
    double colorR = 0;
    double colorG = 0;
    double colorB = 0;
    float gradientMagnitudeSum=0;
    float quality(){
        if(sampleCount>0)
            return gradientMagnitudeSum;
        return 0;
    }
};


#endif /* DataCost_h */

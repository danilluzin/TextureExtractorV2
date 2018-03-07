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
    glm::vec4 colorSum;
    float gradientMagnitudeSum=0;
    void calcQuality(){
        if(sampleCount>0)
            quality =  gradientMagnitudeSum;
        else
        quality = 0;
    }
    float quality;
};


#endif /* DataCost_h */

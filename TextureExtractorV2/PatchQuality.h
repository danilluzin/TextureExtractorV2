//
//  DataCost.h
//  TextureExtractorV2
//
//  Created by Danil Luzin on 19/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef DataCost_h
#define DataCost_h

/**
 * Structure that hold all the information that we gather during the data cost calculation step.
 */
struct PatchQuality {
    /** a number of pixels that were sampled for this triangle during the rasterization phase. It gets increased every time a pixel is sampled, it gets decreased if that pixel is later determined to belong to another triangle (due to occlusion) */
    uint sampleCount           = 0;

    /** a number of pixels a triangle could have, po- tentially, had from a view regardless of occlusion (only gets incremented). If a strict occlusion is picked in the configuration then view-triangle pair gets discarded if a sampleCount ̸= potentialSampleCount. */
    uint potentialSampleCount  = 0;
    
    /** a total color sum of samples taken from the source modified by Sobel operator. In the same fashion as sampleCount gets decreased if sample is later determined to belong to other triangle */
    float gradientMagnitudeSum = 0;
    
    /** a color sum, sampled from the original source image. Same decrease/increase principle is applied to this variable as well. Used for the color consistency check  */
    glm::vec4 colorSum;

    
    /**
     * Calculates and sets the quality. If the sample count is 0 (i.e polygon was invisible then quality is 0)
     */
    void calcQuality(){
        if(sampleCount>0)
            quality =  gradientMagnitudeSum;
        else
        quality = 0;
    }
    
    /** calculated patch aulaity */
    float quality;
};


#endif /* DataCost_h */

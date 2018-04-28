//
//  TexturePatch.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 01/03/2018.
//  Copyright © 2018 Danil Luzin. All rights reserved.
//

#ifndef TexturePatch_hpp
#define TexturePatch_hpp

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <vector>
#include <glm/glm.hpp>
/**
 * Sructure that is used for constructing  texture patch sets.
 */
struct EdgeVertex{
    /** Vertex that is shared between one polygon and a neigbouring polygon */
    uint commonVertex;
    
    /** ID of a neighbouring triangle */
    uint neighbourID;
  
    /** difference in color on a common vertex */
    glm::vec4 colorDiff;
};


/**
 * Sructure that defines texture patch set. All all the adjacent texture polyogns that are assigned the same labeling
 */
struct TexturePatch{
    /** set of triangle ids that belong to a set */
    std::set<uint> myTriangles;
    
    /** set of vertex samples on the edge of the set */
    std::vector<EdgeVertex> edgeVerticies;
    
    /** set of neighbouring triangles that are adjacent to our set but are assigned a different source image */
    std::set<uint> neighbourTriangles;
    
    /** ID of a patch set */
    uint patchID;
     /** ID of a view assignment common to a set */
    uint viewID;
    
     /** Mean color differene calculated using edge samples */
    glm::vec4 meanColorDiff;
};

/** Structure that holds information of patch membership in a set*/
struct TexturePatchDictionary{
    /** Dictionary of patch membership in a set*/
    std::map<uint,uint> triangleMembership;
    
    /** Collection of all texture patch sets(name is missleadnig TexturePatch is refers as texture patch set in a BP) */
    std::map<uint,TexturePatch> patches;
    
    /**
     * Generates a new patch set and returns its newly generated ID
     * @return new ID of a set
     */
    uint genNewPatch(){
        TexturePatch tp;
        tp.patchID = patches.size()+1;
        patches[tp.patchID] = tp;
        return tp.patchID;
    }
};





#endif /* TexturePatch_hpp */

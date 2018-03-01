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

struct EdgeVertex{
    uint commonVertex;
    uint neighbourID;
    glm::vec4 colorDiff;
};

struct TexturePatch{
    
    std::set<uint> myTriangles;
    std::vector<EdgeVertex> edgeVerticies;
    std::set<uint> neighbourTriangles;
    uint patchID;
    uint viewID;
    glm::vec4 meanColorDiff;
};

struct TexturePatchDictionary{
    std::map<uint,uint> triangleMembership;
    std::map<uint,TexturePatch> patches;
    uint genNewPatch(){
        TexturePatch tp;
        tp.patchID = patches.size()+1;
        patches[tp.patchID] = tp;
        return tp.patchID;
    }
};





#endif /* TexturePatch_hpp */

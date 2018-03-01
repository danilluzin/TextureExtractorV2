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
#include <map>

struct TexturePatch{
    static std::map<uint,uint> vertexMembership;
    std::vector<uint> myTriangles;
    std::vector<uint> neighbourVerticies;
    std::vector<uint> neighbourTriangles;
    uint patchID;
};





#endif /* TexturePatch_hpp */

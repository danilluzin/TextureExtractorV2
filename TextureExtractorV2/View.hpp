//
//  View.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 06/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef View_hpp
#define View_hpp

#include "Transformation.hpp"
#include "Bitmap.hpp"
#include "Camera.hpp"
#include <unordered_set>

class View{
public:
    View(){};
    uint id;
    Bitmap * sourceImage = nullptr;
    std::string fileName;
    Camera camera;
    uint photoWidth;
    uint photoHeight;
    std::unordered_set<uint> visibleFaces;
    void loadImage();
    void releaseImage();
};


#endif /* View_hpp */

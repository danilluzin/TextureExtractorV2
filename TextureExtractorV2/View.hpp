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

class View{
    uint id;
    Bitmap sourceImage;
    Camera camera;
};
#endif /* View_hpp */

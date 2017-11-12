//
//  View.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 06/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "View.hpp"
void View::loadImage(){
    sourceImage = new Bitmap(fileName);
}

void View::releaseImage(){
    delete sourceImage;
    sourceImage = nullptr;
}

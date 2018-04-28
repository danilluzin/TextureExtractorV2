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

/**
 * Class that is used to represent the source image. Hold the information nessecary to recreate and analyse the source image.
 */
class View{
public:
    View(){};
    /** ID that is assigned to a view. Starts at 1 */
    uint id;
    /** Bitmap of the source photograph */
    Bitmap * sourceImage = nullptr;
    
    /** Filename of the source photograph */
    std::string fileName;
    
    /** Instance of the camera class that hold camera parameters and position*/
    Camera camera;
    
    /** Widths of the source photo */
    uint photoWidth;
    
    /** Height of the source photo */
    uint photoHeight;
    
    /** Set of the faces that are visible from the source photo */
    std::unordered_set<uint> visibleFaces;
    
    /** Loads an image into memory */
    void loadImage();
    
    /** Releases the image from the memory */
    void releaseImage();
};


#endif /* View_hpp */

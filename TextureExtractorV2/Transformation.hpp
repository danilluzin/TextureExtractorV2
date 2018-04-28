//
//  Transformation.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Transformation_hpp
#define Transformation_hpp

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Mesh.hpp"
#include "Camera.hpp"

/**
 * Class that holds matrix transformation information for the photographs
 */
class Transformation{
    
     /** Screen space transform matrix */
    glm::mat4 screenSpaceTransform;

    /** Camera parametrs that are associated with this transformation */
    Camera camera;

public:

    /** Position, Rotation, Scale vectors. Are constant for our implemenation */
    glm::vec3 pos, rot, scale;
    
    Transformation(const glm::vec3 & pos = glm::vec3(),
                   const glm::vec3 & rot = glm::vec3(),
                   const glm::vec3 & scale = glm::vec3(1.0,1.0,1.0));
    
    /**
     * Changes aspect ratio, updates perspective matrix
     * @param width width of the image
     * @param height height of the image
     */
    void setAspectRatio(int width, int height);
    
    /**
     * Sets a camera representation
     * @param camera a new camera
     */
    void setCamera (const Camera & camera);
    
    /**
     * Seta a new screen space transform. Used for recreating an image.
     * @param halfHeight half of a target image height
     * @param halfWidth half of a target image width
     */
    void setScreenTransform(int halfHeight, int halfWidth);
    
    /**
     * Performs a perspective devide by w
     * @param coord a source coord. Stays unchanged
     * @return vertex after the devision.
     */
    Vertex doPerspectiveDevide(const Vertex & coord) const ;
    
    /**
     * @return a model matrix. Constatn in  our implementation.
     */
    glm::mat4 getModelMatrix() const;
    
    /**
     * @return screen space transform matrix. Based on the shape and size of the photo
     */
    glm::mat4 getScreenTransform() const;

    /**
     * @return view projection matrix. Based on the camera parameters.
     */
    glm::mat4 getViewProjection() const;
    
private:
    
};

/**
 * Checks if a bounding box is in the frustum o a particular view, given a transformation that represent that view.
 * @param boundingBox box to check
 * @param transformation transformation to check
 * @return true if is insize, flase othervise
 */
bool isInsideViewFrustrum (const BoundingBox & boundingBox, const Transformation & transformation);


#endif /* Transformation_hpp */

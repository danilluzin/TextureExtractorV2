//
//  DataCostsExtractor.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 19/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef DataCostsExtractor_hpp
#define DataCostsExtractor_hpp


#include <stdio.h>
#include <vector>
#include "Bitmap.hpp"
#include "Mesh.hpp"
#include "Edge.hpp"
#include "Transformation.hpp"
#include "View.hpp"
#include "PatchQuality.h"
#include "Utils.h"
#include <unordered_set>
/***
*Class that performs data costs extraction for one source image. Analyzes and recreates the source image.
*/
class DataCostsExtractor{

    /** Width and height of the source image */
    int width, height;
    
    /** Transformation that describes the source photograph. Used to generate all the neccessary matricies */
    Transformation transformation;
    
    /** Working mesh */
    const Mesh & mesh;
    
    /** View that represents the target source photograph  */
    View & view;
    
    /** Depth buffer a z-buffer */
    std::vector<float> depthBuffer;
    
    /** buffer that keeps track of what triangle is assigned to what pixel*/
    std::vector<uint> idBuffer;
    
    /** Collection of the patch information that is gradually gathered during the data cost calculation */
    std::map<uint,PatchQuality> patchInfos;
    
    
    /** Source image that was modified using a Sobel operator */
    Bitmap sobelImage;
    
    /** Source image */
    Bitmap * sourceImage;
    
public:
    DataCostsExtractor(const Mesh & mesh, View & view);
    
    /**
     * starts data costs   calculation for the set source image. Start at the root of the BVH.
     * @return list of patch quality information for all the polygons that were visisble. Gets later merged into the main ledger.
     */
    std::map<uint,PatchQuality> calculateCosts();
    
    ~DataCostsExtractor();
    
private:
    /**
     * Sorts verticies and calculates their postions. calculates handedness
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param triangle current triangle
     */
    void rasterizeTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    /**
     * Starts the analysation process. Checks for clipping. And prepares verticies
     * @param triangle target triangle
     */
    void processTriangle(const Triangle & triangle);
    
    /**
     * Sets up a gradient and start scanline analysis of data costs
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param handedness of a current tringle
     * @param id triangle id
     */
    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, uint id);
    
    
    /**
     * Sets a transformation to represent the target source photo
     * @param transformation a new transformation
     */
    void setTransformation(Transformation transformation){
        this->transformation = transformation;
    }
    
    /**
     * Recursively checks nodes of BVH and checks them for frustum culling, is node is visible analyses it
     * @param node a node to start a partition.
     */
    void traversePartition(const PartitionNode * node);
    
    /**
     * Clears id and depth buffers
     */
    void clearBuffer();
    
    /**
     * Analyses whole scanline
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     * @param id current triangle ID
     */
    void drawScanLine(Edge left, Edge right, int y, Gradient & gradient, uint id);
    
    /**
    * Calculates triangle area
    */
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);
};





#endif /* DataCostsExtractor_hpp */

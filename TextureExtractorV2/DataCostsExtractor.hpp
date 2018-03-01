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
#include <unordered_set>


class DataCostsExtractor{
    
    int width, height;
    
    Transformation transformation;
    
    const Mesh & mesh;
    
    View & view;
    
    std::vector<float> depthBuffer;
    
    std::map<uint,PatchQuality> patchInfos;
    
    std::vector<uint> idBuffer;
    
    Bitmap sobelImage;
    Bitmap * sourceImage;
    
public:
    DataCostsExtractor(const Mesh & mesh, View & view);
    
    std::map<uint,PatchQuality> calculateCosts();
    
    ~DataCostsExtractor();
    
private:
    
    void rasterizeTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    void processTriangle(const Triangle & triangle);
    
    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, uint id);
    
    void setTransformation(Transformation transformation){
        this->transformation = transformation;
    }
    
    void traversePartition(const PartitionNode * node);
    
    void clearBuffer();
    
    void drawScanLine(Edge left, Edge right, int y, Gradient & gradient, uint id);
    
    bool isInsideViewFrustrum (Vertex v);
    
    bool isInsideViewFrustrum (const Object & object);
    
    bool isInsideViewFrustrum (const BoundingBox & boundingBox);
    
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);
};





#endif /* DataCostsExtractor_hpp */

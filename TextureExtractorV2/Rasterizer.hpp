//
//  Rasterizator.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Rasterizer_hpp
#define Rasterizer_hpp

#include <stdio.h>
#include <vector>
#include "Bitmap.hpp"
#include "Mesh.hpp"
#include "Edge.hpp"
#include "Transformation.hpp"
#include "RenderContext.h"
#include <unordered_set>

/**
 * DEBUG class. Not used for the main function of the tool
 */
class Rasterizer{
    
    const Bitmap * texture;
    
    int width, height;
    
    Transformation transformation;
    
    RenderContext * context;
    
    Mesh mesh;
    
    std::vector<float> depthBuffer;
    std::vector<uint> idBuffer;

    std::unordered_set<uint> visibleFaces;
    
public:
    std::vector<std::pair<uint,float>> scoreTable;

    void setRenderContext (RenderContext * rc);
    
    void setTexture (const Bitmap & txr) {texture = &txr;}

    //FIXME: no copy
    void bindMesh (const Mesh & m){mesh = m;}

    Rasterizer(int width, int height);
    
    void rasterizeTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
     
    void drawTriangle(const Triangle & triangle);

    void drawMesh();

    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, uint id);
    
    void setTransformation(Transformation transformation){
        this->transformation = transformation;
    }
    
    void clearBuffer();
    
    std::unordered_set<uint> getVisibleFaces() const { return visibleFaces;}
    
    //DEBUG
     void _getDepthBitmap(Bitmap & bitmap);
    
private:
    void traversePartition(const PartitionNode * node);
        
    bool clipPoligonAxis (std::vector<Vertex> & verticies, int component);
    
     void clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor);
    
    void drawScanLine(Edge left, Edge right, int y, Gradient & gradient, uint id);
    
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);

    float minDepth = std::numeric_limits<float>::max();
    float maxDepth = std::numeric_limits<float>::min();
    
    /*
    std::map <int,glm::vec4>colors = {
        {0,glm::vec4(0.0f,0.0f,0.0f,1)},
        {1,glm::vec4(0.5f,0.0f,0.0f,1)},
        {2,glm::vec4(0.0f,0.5f,0.0f,1)},
        {3,glm::vec4(0.0f,0.0f,0.5f,1)},
        {4,glm::vec4(0.5f,0.5f,0.0f,1)},
        {5,glm::vec4(0.0f,0.5f,0.5f,1)},
        {6,glm::vec4(0.5f,0.0f,0.5f,1)},
        {7,glm::vec4(1.0f,0.5f,0.0f,1)},
        {8,glm::vec4(0.0f,1.0f,0.5f,1)},
        {9,glm::vec4(0.5f,0.0f,1.0,1)}
    };
     */

};

#endif /* Rasterizator_hpp */

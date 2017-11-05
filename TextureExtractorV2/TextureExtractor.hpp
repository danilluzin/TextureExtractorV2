//
//  TextureExtractor.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 15/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef TextureExtractor_hpp
#define TextureExtractor_hpp

#include <stdio.h>
#include <vector>
#include "Bitmap.hpp"
#include "Mesh.hpp"
#include "TextureEdge.hpp"
#include "Transformation.hpp"

class TextureExtractor {
    
    Bitmap * source;
    
    Transformation transformation;
    
    Mesh mesh;
    
    std::vector<std::pair<uint,float>> * sourceScoreTable;
    std::vector<float>  scoreTable;
public:
    
    TextureExtractor (int width, int height){
        scoreTable.resize(width * height);
        std::fill(scoreTable.begin(), scoreTable.end(), std::numeric_limits<float>::min());
        texture = Bitmap(width, height);
        this->width = width;
        this->height = height;
    }
    
    int width, height;
    
    Bitmap texture;
    
    void processTriangle(const Triangle & triangle);
    
    void extractTexture(Bitmap * source, std::vector<std::pair<uint,float>> * scoreTable);
    
    void bindMesh (const Mesh & m){mesh = m;}
    
    void setTransformation(Transformation transformation){
        this->transformation = transformation;
    }
private:
    
    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, const Triangle & triangle);
    
    void rasterizeTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    void extractTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    void drawScanLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient, uint id);
    
    bool clipPoligonAxis (std::vector<Vertex> & verticies, int component);
    
    void clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor);
    
    bool isInsideViewFrustrum (Vertex v);
    
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);
    float triangleAreaTexture(Vertex v1, Vertex v2, Vertex v3);
    
    
};




#endif /* TextureExtractor_hpp */


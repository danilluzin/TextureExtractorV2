//
//  ExtractionWorker.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 12/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef ExtractionWorker_hpp
#define ExtractionWorker_hpp

#include "View.hpp"
#include "Mesh.hpp"
#include "Transformation.hpp"
#include "Bitmap.hpp"
#include "TextureEdge.hpp"

class ExtractionWorker{
private:
    
    Bitmap * texture = nullptr;
    
    Bitmap * source = nullptr;
    
    View * view = nullptr;
    
    Transformation transformation;
    
    const Mesh & mesh;
    
public:
    
    ExtractionWorker(const Mesh & mesh) : mesh(mesh){};
    
    void extract(Triangle face,Bitmap & tex, View & view);
    
    void fillTextureTriangle(Triangle face,glm::vec4 color, Bitmap & destination);
    
private:
    
    void processTriangle(const Triangle & triangle);

    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, const Triangle & triangle);
    
     void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, glm::vec4 color);
    
    void rasterizeTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    void extractTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    void drawScanLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient, uint id);
    
    void drawScanLineColor(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient, glm::vec4 color);
    
    bool clipPoligonAxis (std::vector<Vertex> & verticies, int component);
    
    void clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor);
    
    bool isInsideViewFrustrum (Vertex v);
    
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);
    float triangleAreaTexture(Vertex v1, Vertex v2, Vertex v3);

};




#endif /* ExtractionWorker_hpp */

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
/**
 * Worker class. Extracts texture information from a photographs.
 */
class ExtractionWorker{
private:
    /** mask to write information for extension step */
    Bitmap * mask = nullptr;
    
    /** target texture bitmap */
    Bitmap * texture = nullptr;
    
    /** target leveling texture */
    Bitmap * levelingTexture = nullptr;
    
    /** Source photograph for sampling */
    Bitmap * source = nullptr;
    
    /** current view */
    View * view = nullptr;
    
    /** current camera transformation */
    Transformation transformation;
    
    /** working mesh */
    const Mesh * mesh;
    
public:
    
    ExtractionWorker(const Mesh * mesh) : mesh(mesh){};
    
    ExtractionWorker() {};
    
    /**
     * Starts an extraction process for one trinagle
     * @param face target triangle
     * @param tex output texture
     * @param view source view
     */
    void extract(Triangle face,Bitmap & tex, View & view);
    
    /**
     * Starts an extension on a texture triangle
     * @param face target triangle
     * @param tex output texture
     */
    void extend(Triangle face,Bitmap & tex);
    
    /**
     * Applies a gradient to a texture triangle
     * @param face target triangle
     * @param dst output texture
     * @param src texture for initial sampling
     * @param color set of color adjustment gradients for every vertex.
     * @param output leveling texture
     * @param view source view
     */
    void applyGradient(Triangle face,Bitmap & src,Bitmap & dst, glm::vec4 color [3], Bitmap & levelingTexture);
    
    /**
     * Sets a new bloking mask
     * @param m new mask
     */
    void setMask (Bitmap * m) {mask = m;}
    
    /**
     * Sets a working mesh
     * @param m a mesh
     */
    void setMesh (const Mesh * m) {mesh = m;}
    
    
    /**
     * Fills a texture triangle with solid color
     * @param face target triangle
     * @param destination output texture
     * @param color a color to applye
     */
    void fillTextureTriangle(Triangle face,glm::vec4 color, Bitmap & destination);
    
private:
    
    /**
     * Perform an extension on a texture triangle
     * @param face target triangle
     */
    void extendTriangle(Triangle face);
    
    /**
     * Starts extraction process. Checks for clipping. And prepares verticies
     * @param triangle target triangle
     */
    void processTriangle(const Triangle & triangle);

    /**
     * Starts gradien application process. Checks for clipping. And prepares verticies
     * @param triangle target triangle
     */
    void proccesGradientTriangle(const Triangle & triangle, glm::vec4 color [3]);
    
    /**
     * Sets up a gradient and start scanline extraction of photo color data
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param handedness of a current tringle
     * @param triangle current triangel
     */
    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, const Triangle & triangle);
    
    /**
     * Sets up a gradient and start scanline application of solid color
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param handedness of a current tringle
     * @param color color to apply
     */
    void fillTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness, glm::vec4 color);
    
    /**
     * Sets up a gradient and start scanline application of gradent interpolation
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param handedness of a current tringle
     * @param triangle current triangle
     * @param color set of color adjustment gradients for every vertex.
     */
    void fillTriangleGradient(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness,const Triangle & triangle,glm::vec4 color [3]);
    
    /**
     * Sorts verticies and calculates their postions. calculates handedness
     * @param minYVert vertex with minimal Y position
     * @param maxYVert vertex with maximal Y position
     * @param midYVert vertex with middle Y position
     * @param triangle current triangle
     */
    void extractTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle);
    
    
    /**
     * Extracts texture scanline
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     */
    void drawScanLine(TextureEdge left, TextureEdge right, int y);
    
    /**
     * Adds gradient texture scanline
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     * @param gradient current scanline gradient
     */
    void addGradientLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient);
    
    /**
     * Fills scanline texture extension
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     * @param gradient current scanline gradient
     */
    void fillExtensionLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient);
    
    /**
     * Fills scanline solid color
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     * @param color color to fill
     */
    void drawScanLineColor(TextureEdge left, TextureEdge right, int y, glm::vec4 color);
    
    /**
     * Fills scanline solid color igonoring thr masking
     * @param left left edge
     * @param right right edge
     * @param y scnaline y position
     */
    void drawScanLineColorNoMask(TextureEdge left, TextureEdge right, int y, glm::vec4 color);
    
    /**
     * Performs window space clipping
     * @param verticies  source and taget set of verticies
     * @param componet (id of component to clip x,y)
     */
    bool clipPoligonAxis (std::vector<Vertex> & verticies, int component);
    
    /**
     * Performs window space clipping
     * @param verticies  source set of verticies
     * @param result target set of verticies
     * @param componet (id of component to clip x,y)
     * @param clipFactor clipping factor. Define clipping plane intersection
     */
    void clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor);
    
    /**
     * Calculates triangle area
     */
    float triangleArea(Vertex v1, Vertex v2, Vertex v3);
    
    /**
     * Calculates texture triangle area
     */
    float triangleAreaTexture(Vertex v1, Vertex v2, Vertex v3);

    /**
     * Fills scanline color sampling from one of the edges
     * @param left left edge
     * @param right right edge
     * @param leftIsSample if true sample from left, if false sample from right
     */
    void fillBetweenEdges(TextureEdge left, TextureEdge right, bool leftIsSample);
    
};




#endif /* ExtractionWorker_hpp */

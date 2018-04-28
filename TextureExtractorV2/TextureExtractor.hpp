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
#include "View.hpp"
#include "Transformation.hpp"
#include "Arguments.h"
#include "PatchQuality.h"
#include "ExtractionWorker.hpp"
#include "TexturePatch.hpp"
/**
 * Main class. Manages general extraction tasks
 */
class TextureExtractor {
    
    /** current texture mask. Used in texture expansion to keep expansion from overlapping with real texture. */
    Bitmap mask;
    
    /** Mesh  we are working on */
    Mesh mesh;
    
    /** Dictionarry of views that hold camera data and file references */
    std::map<uint,View> views;

    /** Dictionary of colors assigned to each photograph. Used for debuging */
    std::map<uint,glm::vec4> viewColors;
    
    /** Data costs structure. For each face(triangle) it holds a collection of patch qulity data nesessary to compute data costs */
    std::map<uint,std::map<uint,PatchQuality>> dataCosts;
    
    /** List of vertex color samples. Each vertex gets assigned a collection of view:color pairs */
    std::map<uint,std::map<uint,glm::vec4>> colorSamples;
    
    /** collection of color average for every vertex, avereged using sample list */
    std::map<uint,glm::vec4> colorAverages;
    
    /** Instance of extraction worke class. It extractes all the patches from the source photographs  */
    ExtractionWorker worker;
    
    /** Structure that is used to build texture adjacency for global leveling */
    TexturePatchDictionary patchDictionary;
    
public:

    ~TextureExtractor();
    
    /**
     * Reads and prepares views using the bundler file and camera list. Calculates all the nessecary matricies and parametrs
     */
    bool prepareViews();
    
    /**
     * Starts datacost calcualtation.
     */
    bool calculateDataCosts();
    
    /**
     * Start labeling process
     */
    bool selectViews();
    
    /**
     * sets a working mesh
     * @param M input mesh
     */
    void setMesh (const Mesh & m){mesh = m; worker.setMesh(&mesh);}
    
    /**
     * Using labeling starts to generate texture files for all the objects in the mesh
     */
    bool generateTexture();
    
    /**
     * Generates textures for a single object
     * @param object target object
     */
    bool generateTextureForObject(Object & object);
    
    /**
     * returns a number of views(source photographs)
     */
    uint numberOfViews(){return (uint)views.size();}
    
    /**
     * Reads and verifies precomputed labeling from file.
     */
    bool readLabelsFromFile();
    
    /**
     * Reads and verifies precomputed labeling from file.
     */
    bool readDataCostsFromFile();
    
    /**
     * Uses patch qulity to calculate datacosts and does color consistency check
     */
    void postprocessDataCosts();
    
    //DEBUG
    
    /**
     * DEBUG: prints out camera info
     */
    void checkCameraInfo();

    /**
     * DEBUG: prints out camera info for one camera only
     */
    void checkCameraInfo(uint viewId);
    
    /**
     * DEBUG: renders a view
     * @param bitmap image target.
     * @param viewID view to replicate
     */
    void renderView(Bitmap & bitmap,uint viewId);
    
    /**
     * DEBUG: renders a view and depth map
     * @param bitmap output image target.
     * @param bitmapDepth output depth image target.
     * @param viewID view to replicate
     */
    void renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,uint viewId);
    

    
private:
    
    /**
     * Generates texture patches for all the triangles of an object.
     * @param lableTexture texture to save the labeling into.
     * @param texture target texture bitmap
     * @param object target object
     */
    void extractAllFaces(Bitmap & labelTexture,Bitmap & texture,Object & object);
    
    /**
     * Extends the texture patches for all the triangles of an object.
     * @param texture target texture bitmap
     * @param object target object
     */
    void extendAllFaces(Bitmap & texture,Object & object);
    
    /**
     * Applies a gradient to all the triangles of an object.
     * @param textureCopy source texture to sample for initial color
     * @param levelingTexture leveling debug texture
     * @param texture target texture bitmap
     * @param object target object
     */
    void applyGradientAllFaces(Bitmap & textureCopy, Bitmap & levelingTexture, Bitmap & texture,Object & object);
    
    
    /**
     * initializes labeling generation and calculation usin the mapMAP
     */
    bool mapMapGetLabeling();
    
    /**
     * Adds new view to view dictionary
     * @param view new view
     */
    uint addView(const View & view);
    
    /**
     * Checks if view ID is valid
     * @param id id to check
     * @return true if ID is already in a dictionary (was already added)
     */
    bool isValidViewId(int id);
    
    /**
     * Checks if triange ID is valid
     * @param id id to check
     * @return true if ID is already in a mesh
     */
    bool isValidFaceId(int id);
    
    /**
     * Reads list of photos file to get image paths
     * @return True if sucessfull
     */
    bool extractPhotoList();
    
    /**
     * Reads camera oinformation from bundle file
     * @return True if sucessfull
     */
    bool extractCameraInfoCreateViews();
    
    /**
     * Reads camera information from bundle file
     * param readCOunter number of view to read
     * @return True if sucessfull
     */
    bool parseCameraInfo(std::ifstream & file,uint readCounter);
    
    /**
     * Parses labeling line
     * param line line to parse
     * @return True if sucessfull
     */
    bool parseLabelingLine(const std::string & line);
    
    /**
     * Reads line from a file and tries to parse  float tokens
     * param file source file
      * param tokens read tokens
     * @return True if sucessfull
     */
    bool get3Floats(std::vector<float> & tokens, std::ifstream & file);
    
    /**
     * Reads line from a file and tries to parse  int tokens
     * param file source file
     * param tokens read tokens
     * @return True if sucessfull
     */
    bool get2Ints(std::vector<int> & tokens,const std::string & line);
    
    /**
     * Reads precomputed data costs from file
     * param file source file
     * @return True if sucessfull
     */
    bool  parseFaceDataCost(std::ifstream & file);
    
    /**
     * Writes precomputed labeling into file
     */
    bool writeLabelingToFile();
    
    /**
     * Writes precomputed data costs into file
     */
    bool writeDataCostsToFile();
    
    /**
     * Calculates vertex color sample list from photographs
     */
    void getSampleList();
    
    /**
     * Updates vertex color sample list from photographs, to accomodate for global adjustements
     */
    void updateSampleList();
    
    /**
     * Constructs patch adjacency based on the labeling nd mesh structure
     */
    void preparePatchDictionary();
    
    /**
     * perform photo consistency check. Removes outliners
     */
    void photoConsistencyDataCosts();
    
};



#endif /* TextureExtractor_hpp */


//
//  TextureExtractor.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 15/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "TextureExtractor.hpp"
#include <glm/ext.hpp>
#include "Rasterizer.hpp"
#include "Utils.h"
#include <fstream>
#include "stb/stb_image.h"
#include "ExtractionWorker.hpp"
#include "DataCostExtractionManager.hpp"
#include <numeric>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstdlib>


void windowRender( Mesh mesh );

TextureExtractor::~TextureExtractor(){
    for(auto & v : views){
        v.second.releaseImage();
    }
}


bool TextureExtractor::isValidViewId(int id){
    return (id>=0) && (id<=views.size());
}


bool TextureExtractor::isValidFaceId(int id){
    return mesh.isValidFaceId(id);
}


bool TextureExtractor::prepareViews(){

    print("Reading the list of source Photos\n");
    adjPad(2);
    bool listOk;
    listOk = extractPhotoList();
    if(!listOk){
        printError("ERROR occured when getting photo list\n");
        return false;
    }
    adjPad(-2);
    print("Reading camera info (position, rotation, fov)\n");
    adjPad(2);
    bool cameraInfoOK;
    cameraInfoOK = extractCameraInfoCreateViews();
    if(!cameraInfoOK){
        printError("ERROR occured when getting camera info\n");
        return false;
    }
    adjPad(-2);
    for(auto v : views){
        float gs = (float)v.second.id / views.size();
        float rndR = ((float)rand())/RAND_MAX;
        float rndG = ((float)rand())/RAND_MAX;
        float rndB = ((float)rand())/RAND_MAX;
        viewColors[v.first] = glm::vec4(rndR,rndG,rndB,1);
//        viewColors[v.first] = glm::vec4(gs,gs,gs,1);
    }

    return true;
}

double vecSum(glm::vec4 v){
    return v[0]+v[1]+v[2];
}

double calcQuality(PatchQuality & patchQuality){
    if(patchQuality.sampleCount>0)
        return patchQuality.gradientMagnitudeSum;
    else
        return 0;
}


void TextureExtractor::photoConsistencyDataCosts(){
    int removed = 0;
    float outlinerPersentage = arguments.colorConsistencyThreshold;
    
    for(auto & f : dataCosts){
        std::vector<double> colorCombined;
        for(auto & v : f.second){
            colorCombined.push_back(vecSum(v.second.colorSum));
        }
        //getting mean color
        std::sort(colorCombined.begin(),colorCombined.end());
        double meanColor = colorCombined[floor(colorCombined.size()/2)];
        
        std::vector<uint> outlinerViews;
        
        for(auto & v : f.second){
            float percentColor = std::min(vecSum(v.second.colorSum),meanColor) / std::max(vecSum(v.second.colorSum),meanColor);
            
            if(percentColor<outlinerPersentage){
                outlinerViews.push_back(v.first);
            }
        } 
        if(outlinerViews.size()<f.second.size()){
            for(int t=0;t<outlinerViews.size();t++){
                f.second.erase(outlinerViews[t]);
                removed++;
            }
        }
    }
    print("Removed "+COLOR_BOLD+std::to_string(removed) + COLOR_RESET + " [face : view] pairs (color consistency)\n");
}


void TextureExtractor::postprocessDataCosts(){
    //normalisation and post;
    print("Post-processing datasets:\n");
    adjPad(2);
    for(auto & f : dataCosts){
        float faceMax = 0;
        std::vector<glm::vec4> colors;
        
        for(auto & v : f.second){
            v.second.colorSum[0] = v.second.colorSum[0]/v.second.sampleCount;
            v.second.colorSum[1] = v.second.colorSum[1]/v.second.sampleCount;
            v.second.colorSum[2] = v.second.colorSum[2]/v.second.sampleCount;
            v.second.colorSum[3] = v.second.colorSum[3]/v.second.sampleCount;
            
            colors.push_back(v.second.colorSum);
            
            v.second.quality = calcQuality(v.second);
            faceMax = std::max(faceMax,v.second.quality);
        }
        
        for(auto & v : f.second){
            if(faceMax > 0){
                v.second.quality = 1 - (v.second.quality/faceMax);
            }else{
                v.second.quality = 1 - v.second.quality;
            }
        }
    }
    
    if(arguments.doColorConsistency){
        photoConsistencyDataCosts();
    }
    adjPad(-2);
}


bool TextureExtractor::calculateDataCosts(){
    uint progressCounter=0;
    int threadCount = arguments.threadCount;
    
    print("Working Threads: "+std::to_string(arguments.threadCount)+"\n");
    std::mutex printMtx;
    uint viewsPerOneManager = (uint)views.size()/threadCount;
    
    std::vector<DataCostExtractionManager> managers(threadCount, mesh);
    std::vector<std::map<uint,std::map<uint,PatchQuality>>> managerDataSets(threadCount);
    
    for(int m = 0; m < managers.size(); m++){
        managers[m].dataCosts = &managerDataSets[m];
        managers[m].mtx = &printMtx;
        managers[m].totalViewCount = (uint)views.size();
        managers[m].progressCounter = &progressCounter;
    }
    
    int currentManager = 0;
    int currentManagerWorkLoad = 0;
    for(auto & v : views){
        managers[currentManager].viewsToDo.push_back(&v.second);
        currentManagerWorkLoad++;
        if((currentManagerWorkLoad==viewsPerOneManager) && (currentManager!=(threadCount-1))){
            currentManager++;
            currentManagerWorkLoad = 0;
        }
    }
    printOver(COLOR_TEAL+"["+fitPercent(0)+"%]"+COLOR_RESET + " Getting Data Costs");
    
    std::vector<std::thread> threads;
    for(int t = 0 ;t< threadCount; t++){
        threads.push_back( std::thread(&DataCostExtractionManager::doWork, &managers[t]));
    }
    
    for(int t = 0 ;t< threadCount; t++){
        threads[t].join();
    }
    print("\n");
    print("Merging datasets\n");
    //Merging manager datasets
    for(auto & ds : managerDataSets){
        for(auto & v : ds){
            uint viewId = v.first;
            for(auto entry : v.second){
                dataCosts[entry.first][viewId] = entry.second;
            }
        }
    }
 
    if(arguments.writeDataCostsToFile){
        writeDataCostsToFile();
    }
    
    postprocessDataCosts();

    return true;
}


bool TextureExtractor::readLabelsFromFile(){
    std::ifstream file;
    file.open(arguments.labelingFilePath.c_str());
    if(!file.is_open()){
        printError("Unable to open labeling file: " + arguments.labelingFilePath + "\n");
        return false;
    }
    
    std::string line;
    uint successCount = 0;
    while(file.good()){
        getline(file, line);
        if(line.size()>0){
            bool parseOk;
            parseOk = parseLabelingLine(line);
            if(parseOk)
                successCount++;
        }
    }
    file.close();
    if(successCount < mesh.triangles.size()){
        printError("Number of valid labels is less than number of faces: \n");
        printError("    labels = "+std::to_string(successCount)+"\n");
        printError("    faces = "+std::to_string(mesh.triangles.size())+"\n");
        return false;
    }
    return true;
}


bool TextureExtractor::readDataCostsFromFile(){
    std::ifstream file;
    file.open(arguments.dataCostsFilePath.c_str());
    if(!file.is_open()){
        printError("Unable to open data costs file: " + arguments.dataCostsFilePath+"\n");
        return false;
    }
    
    std::string line;
    getline(file,line);
    std::vector<std::string>tokens;
    tokens = splitString(line, ' ');
    uint faceCount = parseInt(tokens[0]);
    uint infoFaceCount =  parseInt(tokens[1]);
    
    if(faceCount != mesh.triangles.size()){
        printError("ERROR: Data cost file format doesn't match current mesh or is wrong\n");
        return false;
    }
    
    uint successCount = 0;
    while(file.good() && (successCount<infoFaceCount)){
        bool parseOk;
        parseOk = parseFaceDataCost(file);
        if(parseOk){
            successCount++;
        }else{
            printError("ERROR: Data cost file format doesn't match current mesh or is wrong\n");
            return false;
        }
    }
    file.close();
    if(successCount < infoFaceCount){
        printError("Number of valid dataCosts is less than promised number of faces: ");
        printError("    got = "+std::to_string(successCount)+"\n");
        printError("    expected = "+std::to_string(infoFaceCount)+"\n");
        return false;
    }
    return true;
}


bool  TextureExtractor::parseFaceDataCost(std::ifstream & file){
    std::string line;
    std::vector<std::string>tokens;
    
    getline(file,line);
    tokens = splitString(line, ' ');
    
    if(!file.good())
        return false;
    
    uint faceId = parseInt(tokens[0]);
    if(faceId > mesh.triangles.size() || faceId <= 0){
        printError("ERROR: Data cost file references wrong face ID\n");
        return false;
    }
    uint viewsCount = parseInt(tokens[1]);
    
    for(int v = 0; v< viewsCount; v++){
        getline(file,line);
        if(!file.good())
            return false;
        
        tokens = splitString(line, ' ');
        uint viewId = parseInt(tokens[0]);
        if(viewId <= 0 || viewId> views.size()){
            printError("ERROR: Data cost file references wrong view ID\n");
            return false;
        }

        dataCosts[faceId][viewId].gradientMagnitudeSum = parseFloat(tokens[1]);
        dataCosts[faceId][viewId].sampleCount = parseFloat(tokens[2]);
        dataCosts[faceId][viewId].colorSum[0] = parseFloat(tokens[3]);
        dataCosts[faceId][viewId].colorSum[1] = parseFloat(tokens[4]);
        dataCosts[faceId][viewId].colorSum[2] = parseFloat(tokens[5]);
        
        
        
    }
    
    getline(file,line);
    if(!file.good())
        return false;
    
    if(line.size() > 0){
        printError("ERROR: Data cost file format wrong. Newline between face infos missing\n");
        return false;
    }
    return true;
}


bool TextureExtractor::parseLabelingLine(const std::string & line){
    std::vector<int> tokens;
    bool got2int;
    got2int = get2Ints(tokens,line);
    if(!got2int)
        return false;
    if(!isValidViewId(tokens[1]) || !isValidFaceId(tokens[0]))
        return false;
    mesh.triangles[tokens[0]].viewId = tokens[1];
    return true;
}


bool TextureExtractor::selectViews(){
    mapMapGetLabeling();
    if(arguments.writeLabelingToFile){
        bool writingOk;
        writingOk = writeLabelingToFile();
        if(!writingOk)
            printWarning("WARNING: Writing of labels to a file failed\n");
    }
    return true;
}


void TextureExtractor::extractAllFaces(Bitmap & labelTexture, Bitmap & texture,Object & object){
    glm::vec4 defaultColor (0.37, 0.61, 0.62, 1);
    uint t = 0;
    for(auto & f : object.triangles){
        float progress = (100*((float)t/object.triangles.size()));
        printOver(COLOR_TEAL+"["+fitPercent(progress)+"%]"+COLOR_RESET + " Getting texture for faces");
        
        Triangle & face = mesh.triangles[f];
        if(face.viewId != 0){
            worker.extract(face, texture, views[face.viewId]);
            if(arguments.genLebelingTexture){
                worker.fillTextureTriangle(face,viewColors[face.viewId],labelTexture);
            }
        }else{
            worker.fillTextureTriangle(face,defaultColor,texture);
        }
        t++;
    }
    printOver(COLOR_TEAL+"["+fitPercent(100)+"%]"+COLOR_RESET + " Getting texture for faces\n");
}


void TextureExtractor::extendAllFaces(Bitmap & texture,Object & object){
    int t=0;
    for(auto & f : object.triangles){
        float progress = (100*((float)t/object.triangles.size()));
        printOver(COLOR_TEAL+"["+fitPercent(progress)+"%]"+COLOR_RESET + " Expanding faces");
        Triangle & face = mesh.triangles[f];
        if(face.viewId != 0){
            worker.extend(face, texture);
        }
        t++;
    }
    printOver(COLOR_TEAL+"["+fitPercent(100)+"%]"+COLOR_RESET + " Expanding faces\n");
}


void TextureExtractor::applyGradientAllFaces(Bitmap & textureCopy, Bitmap & levelingTexture,Bitmap & texture,Object & object){
    Bitmap globalCopy;
    int t=0;
    //global leveling
    if(arguments.doGloabalAdjustment){
        for(auto & f : object.triangles){
            float progress = (100*((float)t/object.triangles.size()));
            printOver(COLOR_TEAL+"["+fitPercent(progress)+"%]"+COLOR_RESET + " Applying global gradient to faces");
            Triangle & face = mesh.triangles[f];
            if(face.viewId != 0){
                uint texturePatchID = patchDictionary.triangleMembership[f];
                TexturePatch & texturePatch = patchDictionary.patches[texturePatchID];
                glm::vec4 colorGlobal[3];
                colorGlobal[0] = texturePatch.meanColorDiff;
                colorGlobal[1] = texturePatch.meanColorDiff;
                colorGlobal[2] = texturePatch.meanColorDiff;
                worker.applyGradient(face,textureCopy,texture,colorGlobal,levelingTexture);
            }
            t++;
        }
        printOver(COLOR_TEAL+"["+fitPercent(100)+"%]"+COLOR_RESET + " Applying global gradient to faces\n");
        globalCopy = texture;
        if(arguments.genGlobalTexture){
            adjPad(-2);
            print("Generating global adjustment texture\n");
            globalCopy.save(arguments.genGlobalTexturePath(object.name));
            adjPad(2);
        }
    }
    
    //seam leveling
    if(arguments.doSeamLeveling){
        t=0;
        for(auto & f : object.triangles){
            float progress = (100*((float)t/object.triangles.size()));
            printOver(COLOR_TEAL+"["+fitPercent(progress)+"%]"+COLOR_RESET + " Applying seam leveling to faces");
            Triangle & face = mesh.triangles[f];
            if(face.viewId != 0){
                glm::vec4 color[3];
                for(int t=0 ; t< 3; t++){
                    color[t] = colorAverages[face.verticies[t]] - colorSamples[face.verticies[t]][face.viewId];
                }
                if(arguments.doGloabalAdjustment){
                    worker.applyGradient(face,globalCopy,texture,color,levelingTexture);
                }else{
                    worker.applyGradient(face,textureCopy,texture,color,levelingTexture);
                }
            }
            t++;
        }
        printOver(COLOR_TEAL+"["+fitPercent(100)+"%]"+COLOR_RESET + " Applying seam leveling to faces\n");
    }
}

bool TextureExtractor::generateTexture(){
    adjPad(2);
    print("Getting verticies color sample list\n");
    getSampleList();
    if(arguments.doGloabalAdjustment){
        print("Building texture patch adjacency\n");
        preparePatchDictionary();
        print("Updating verticies sample list for global adjustments\n");
        updateSampleList();
    }
    for(auto & o : mesh.objects){
        print("Generating texture for object [ "+COLOR_BOLD+o.name+COLOR_RESET+" ] :\n");
        generateTextureForObject(o);
    }
    adjPad(-2);
    return true;
}

bool TextureExtractor::generateTextureForObject(Object & object){
    adjPad(2);
    Bitmap texture = Bitmap(arguments.textureWidth, arguments.textureHeight);
    mask = Bitmap(arguments.textureWidth, arguments.textureHeight);
    Bitmap labelTexture(arguments.textureWidth, arguments.textureHeight);
    Bitmap levelingTexture(arguments.textureWidth, arguments.textureHeight);
    
    worker.setMask(&mask);
    mask.clear(glm::vec4(0,0,0,1));
    texture.clear(glm::vec4(0.8, 0.8, 0.8, 1));
    levelingTexture.clear(glm::vec4(0.8, 0.8, 1, 1));
    labelTexture.clear(glm::vec4(0.8, 0.8, 1, 1));
    
    extractAllFaces(labelTexture,texture,object);

    print("Post-processing:\n");
    adjPad(2);
    Bitmap textureCopy;
    textureCopy = texture;
    applyGradientAllFaces(textureCopy,levelingTexture,texture,object);
    
    if(arguments.doTextureExtension)
        extendAllFaces(texture,object);
    
    adjPad(-2);
    if(arguments.genLevelingTexture && arguments.doSeamLeveling){
        print("Generating leveling texture\n");
        levelingTexture.save(arguments.genLevelingTexturePath(object.name));
    }
    if(arguments.genMaskTexture){
        print("Generating mask texture\n");
        mask.save(arguments.genMaskTexturePath(object.name));
    }
    if(arguments.genRawTexture){
        print("Generating raw texture\n");
        textureCopy.save(arguments.genRawTexturePath(object.name));
    }
    if(arguments.genLebelingTexture){
        print("Generating lable assignment texture\n");
        labelTexture.save(arguments.genLebelingTexturePath(object.name));
    }
    
    print("Writing texture to file\n");
    texture.save(arguments.genFinalTexturePath(object.name));
    adjPad(-2);
    return true;
}


void TextureExtractor::preparePatchDictionary(){
     for(auto & t : mesh.triangles){
         if(t.second.viewId==0)
            continue;
         if(patchDictionary.triangleMembership.find(t.first)!=patchDictionary.triangleMembership.end()) //face already belongs
             continue;
         
        TexturePatch & currentPatch = patchDictionary.patches[patchDictionary.genNewPatch()];
        currentPatch.viewID = t.second.viewId;
        currentPatch.myTriangles.insert(t.first);
        patchDictionary.triangleMembership[t.first] = currentPatch.patchID;
        //BFS
        std::set<uint> visited;
        std::queue<uint> queue;
    
        queue.push(t.first);
        visited.insert(t.first);
        while(!queue.empty()){
            uint currentTri = queue.front();
            queue.pop();
            for(auto n : mesh.adjacencyGraph.nodes[currentTri].neighbours){
                if(visited.find(n) == visited.end()){
                    visited.insert(n);
                    if(mesh.triangles[n].viewId == currentPatch.viewID){
                        patchDictionary.triangleMembership[n] = currentPatch.patchID;
                        currentPatch.myTriangles.insert(n); //same patch
                        queue.push(n); //add to queue
                    }else{
                        if(mesh.triangles[n].viewId != 0){
                            currentPatch.neighbourTriangles.insert(n);
                            for(int t=0;t<3;t++){
                                if(mesh.triangles[n].verticies[t]==mesh.triangles[currentTri].verticies[0] ||
                                   mesh.triangles[n].verticies[t]==mesh.triangles[currentTri].verticies[1] ||
                                   mesh.triangles[n].verticies[t]==mesh.triangles[currentTri].verticies[2]){
                                    //common vertex
                                    EdgeVertex edgeVertex;
                                    edgeVertex.commonVertex = mesh.triangles[n].verticies[t];
                                    edgeVertex.neighbourID = n;
                                    edgeVertex.colorDiff = colorSamples[edgeVertex.commonVertex][mesh.triangles[n].viewId] - colorSamples[edgeVertex.commonVertex][currentPatch.viewID];
                                    edgeVertex.colorDiff /= 2;
                                    currentPatch.edgeVerticies.push_back(edgeVertex);
                                    break;
                                }
                            }
                        }
                    }
                  }
                }
          }
         //get meanColorDiff
         glm::vec4 sumColorDiff;
         for(auto & ev : currentPatch.edgeVerticies){
             sumColorDiff += ev.colorDiff;
         }
         currentPatch.meanColorDiff = sumColorDiff/currentPatch.edgeVerticies.size();

     }
}


void TextureExtractor::getSampleList(){
    for(auto & f : mesh.triangles){
        if(f.second.viewId == 0)
            continue;
        View & v = views[f.second.viewId];
        if(!v.sourceImage){
            v.loadImage();
        }
        Transformation transformation;
        transformation.setCamera(v.camera);
        transformation.setAspectRatio(v.photoWidth, v.photoHeight);
        glm::mat4 cameraModelTransform = transformation.getViewProjection()* transformation.getModelMatrix();
        glm::mat4 screenSpaceTransform = transformation.getScreenTransform();
        
        
        for(int t=0;t<3;t++){
            Vertex vertex = mesh.verticies.at(f.second.verticies[t]);
            vertex = cameraModelTransform * vertex;
            vertex = transformation.doPerspectiveDevide(screenSpaceTransform * vertex);
            colorSamples[vertex.id][f.second.viewId] = v.sourceImage->at(vertex.x(), vertex.y());
        }
    }
    //getting averages
    for(auto & v : colorSamples){
        glm::vec4 sum = glm::vec4(0,0,0,0);
        for(auto & f : v.second){
            sum += f.second;
        }
        sum[0] /= v.second.size();
        sum[1] /= v.second.size();
        sum[2] /= v.second.size();
        sum[3] = 1;
        colorAverages[v.first] = sum;
    }

}


void TextureExtractor::updateSampleList(){
    for(auto & f : mesh.triangles){
        if(f.second.viewId == 0)
            continue;
        View & v = views[f.second.viewId];
        if(!v.sourceImage){
            v.loadImage();
        }
        Transformation transformation;
        transformation.setCamera(v.camera);
        transformation.setAspectRatio(v.photoWidth, v.photoHeight);
        glm::mat4 cameraModelTransform = transformation.getViewProjection()* transformation.getModelMatrix();
        glm::mat4 screenSpaceTransform = transformation.getScreenTransform();
        
        uint texturePatchID = patchDictionary.triangleMembership[f.first];
        TexturePatch & texturePatch = patchDictionary.patches[texturePatchID];
        
        for(int t=0;t<3;t++){
            Vertex vertex = mesh.verticies.at(f.second.verticies[t]);
            vertex = cameraModelTransform * vertex;
            vertex = transformation.doPerspectiveDevide(screenSpaceTransform * vertex);
            colorSamples[vertex.id][f.second.viewId] = v.sourceImage->at(vertex.x(), vertex.y()) + texturePatch.meanColorDiff;
        }
    }
    //getting averages
    for(auto & v : colorSamples){
        glm::vec4 sum = glm::vec4(0,0,0,0);
        for(auto & f : v.second){
            sum += f.second;
        }
        sum[0] /= v.second.size();
        sum[1] /= v.second.size();
        sum[2] /= v.second.size();
        sum[3] = 1;
        colorAverages[v.first] = sum;
    }
    
}




bool TextureExtractor::writeLabelingToFile(){
    print("Writing labeling into file\n");
    std::ofstream file;
    file.open(arguments.newLabelingFilePath.c_str());
    if(!file.is_open()){
        printWarning(s(2)+"Unable to open labeling file for writng: " + arguments.newLabelingFilePath+"\n");
        return false;
    }
    
    for(const auto & f : mesh.triangles){
        const Triangle & face = f.second;
        file << face.id <<" "<<face.viewId<<"\n";
    }
    file.close();
    if(!file.good()){
        printWarning(s(2)+"Something went wrong when writing to file\n");
        return false;
    }
   return true;
}


bool TextureExtractor::writeDataCostsToFile(){
    print("Writing data costs into file\n");
    std::ofstream file;
    file.open(arguments.newDataCostsFilePath.c_str());
    if(!file.is_open()){
         printWarning(s(2)+"Unable to open data cost file for writng: " + arguments.newLabelingFilePath+"\n");
        return false;
    }
    
    file<<mesh.triangles.size()<<" "<<dataCosts.size()<<"\n";
    for(const auto & f : dataCosts){
        file<<f.first<<" "<<f.second.size()<<"\n";
        for(const auto & v:f.second){
            file << v.first<<" "
                 << v.second.gradientMagnitudeSum<<" "
                 << v.second.sampleCount<<" "
                 << v.second.colorSum[0]<<" "
                 << v.second.colorSum[1]<<" "
                 << v.second.colorSum[2]<<"\n";
        }
        file<<"\n";
    }
    file.close();
    if(!file.good()){
        printWarning(s(2)+"Something went wrong when writing to file\n");
        return false;
    }
    return true;
}



bool TextureExtractor::extractPhotoList(){
    
    std::ifstream file;
    file.open(arguments.cameraListFilePath.c_str());
    
    std::string line;
    if(!file.is_open()){
        printError("Unable to open camera list file: " + arguments.cameraListFilePath+"\n");
        return false;
    }
    
    while(file.good()){
        getline(file, line);
        if(line.size()>0){
            if ( line[line.size()-1] == '\r' ) {
                line = line.substr(0, line.size() - 1);
            }
            View view;
            view.fileName = arguments.photoFolderPath +'/' +line;
            //original photo dimensions
            int width, height, comp;
            bool imageOk;
            imageOk = stbi_info(view.fileName.c_str(), &width, &height, &comp);
            if(!imageOk){
                printError("ERROR Wasnt able to read the file:" + view.fileName +"\n");
                return false;
            }
            view.photoWidth = width;
            view.photoHeight = height;
            addView(view);
        }
    }
    file.close();
    if(views.size()<=0){
        printError("File was empty or error occured\n");
        return false;
    }
    return true;
}


bool TextureExtractor::extractCameraInfoCreateViews(){
    std::ifstream file;
    file.open(arguments.cameraInfoPath.c_str());
    
    std::string line;
    if(!file.is_open()){
        printError("Unable to open camera info file: " + arguments.cameraInfoPath+"\n");
        return false;
    }
    
    print("Reading Bundler format\n");
    getline(file, line);
    if(line.at(0)!='#'){
        printError("Bundler format violated. First comment line missing (\"# ....\")\n");
        return false;
    }
    
    getline(file, line);
    std::vector<std::string> tokens;
    tokens = splitString(line, ' ');
    if(tokens.size()!=2){
        printError("Bundler format violated.\"camera_count point_count\" line missing\n");
        return false;
    }
    
    uint cameraCount = parseInt(tokens[0]);
    if(cameraCount<=0){
        printError("Number of cameras is invalid: "+std::to_string(cameraCount)+"\n");
        return false;
    }
    
    if(cameraCount<views.size()){
        printError("Number of cameras is less than photo list: \n");
        printError("    list_size = " + std::to_string(views.size())+ "\n");
        printError("    camera_info_size = "+ std::to_string(cameraCount) + "\n");
        return false;
    }
    
    uint readCounter = 0;
    while(file.good() && (readCounter<views.size())){
        bool parsingOK;
        parsingOK = parseCameraInfo(file,readCounter);
        if(!parsingOK)
            break;
        readCounter ++;
    }
    
    file.close();
    
    if(readCounter != views.size()){
        printError("ERROR camera info count missmatch\n");
        printError("      got: " + std::to_string(readCounter) +
                   " expected: " + std::to_string(views.size()) + "\n");
        return false;
    }
    return true;
}


bool TextureExtractor::parseCameraInfo(std::ifstream & file,uint readCounter){
    std::vector<float> tokens;
    uint viewID = readCounter + 1;
    View view = views[viewID];
    bool got3Float;
    
    //focal length;
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.focalLength = tokens[0];
    
    view.camera.fov = glm::atan(((float)std::max(view.photoHeight,view.photoWidth)), view.camera.focalLength);
    
    //rotation matrix;
        //line 1
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.rotationMatrix[0][0] =  tokens[0];
    view.camera.rotationMatrix[1][0] =  tokens[1];
    view.camera.rotationMatrix[2][0] =  tokens[2];
        //line 2
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.rotationMatrix[0][1] =  tokens[0];
    view.camera.rotationMatrix[1][1] =  tokens[1];
    view.camera.rotationMatrix[2][1] =  tokens[2];
        //line 3
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.rotationMatrix[0][2] =  tokens[0];
    view.camera.rotationMatrix[1][2] =  tokens[1];
    view.camera.rotationMatrix[2][2] =  tokens[2];
    
    view.camera.rotationMatrix[3][3] =  1;
    view.camera.rotationMatrix[0][3] =  0;
    view.camera.rotationMatrix[1][3] =  0;
    view.camera.rotationMatrix[2][3] =  0;
    view.camera.rotationMatrix[3][0] =  0;
    view.camera.rotationMatrix[3][1] =  0;
    view.camera.rotationMatrix[3][2] =  0;

    //translation;
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.translation[0] = tokens[0];
    view.camera.translation[1] = tokens[1];
    view.camera.translation[2] = tokens[2];
    
    
    
    views[viewID] = view;
    return true;
}


bool TextureExtractor::get3Floats(std::vector<float> & tokens, std::ifstream & file){
    std::string line;
    if(file.good())
        getline(file, line);
    else
        return false;
    tokens = splitFloatLine(line);
    if(tokens.size()!=3)
        return false;
    return true;
}

bool TextureExtractor::get2Ints(std::vector<int> & tokens,const std::string & line){
    tokens = splitIntLine(line);
    if(tokens.size()!=2)
        return false;
    return true;
}


uint TextureExtractor::addView(const View & view){
    uint id;
    View newView = view;
    id = (uint)views.size() + 1;
    newView.id = id;
    views[id] = newView;
    return id;
}

//----------------------------------------------------------------------------//
//DEBUG
void TextureExtractor::checkCameraInfo(){
    std::cout<<"Printing camera info\n";
    for(auto v : views){
        std::cout<<"focal length:" <<v.second.camera.focalLength<<"\n";
        std::cout<<"translation: ";
        std::cout<<v.second.camera.translation[0]<<" "<<
                   v.second.camera.translation[1]<<" "<<
                   v.second.camera.translation[2]<<"\n";
        std::cout<<"rotation:\n";
        printMatrix(v.second.camera.rotationMatrix);
        std::cout<<"\n";
    }
}

void TextureExtractor::checkCameraInfo(uint viewId){
    if(views.find(viewId)==views.end()){
        std::cout<<"No such viewId:"<<viewId<<"\n";
        return;
    }
    View  v = views[viewId];
    std::cout<<"Printing camera info\n";
    
    std::cout<<"focal length:" <<v.camera.focalLength<<"\n";
    std::cout<<"translation: ";
    std::cout<<v.camera.translation[0]<<" "<<
    v.camera.translation[1]<<" "<<
    v.camera.translation[2]<<"\n";
    std::cout<<"rotation:\n";
    printMatrix(v.camera.rotationMatrix);
    std::cout<<"\n";
    
}


void TextureExtractor::renderView(Bitmap & bitmap,uint viewId){
    if(views.find(viewId)==views.end()){
        std::cout<<"No such viewId:"<<viewId<<"\n";
        return;
    }
    bitmap.clear(glm::vec4(0.4,0.4,0.4,1));
    View  v = views[viewId];

    bitmap = Bitmap(v.photoWidth, v.photoHeight);
    
    bitmap.clear(glm::vec4(0.4,0.4,0.4,1));
    Rasterizer rasterizer(bitmap.width,bitmap.height);
    rasterizer.bindMesh(mesh);
    //here use new texturel
//    rasterizer.setTexture(texture);

    Transformation transform;
    transform.setCamera(v.camera);
    transform.setAspectRatio(bitmap.width, bitmap.height);

    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(&bitmap);
    rasterizer.drawMesh();
    views[viewId].visibleFaces = rasterizer.getVisibleFaces();
}


void TextureExtractor::renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,uint viewId){
    if(views.find(viewId)==views.end()){
        std::cout<<"No such viewId:"<<viewId<<"\n";
        return;
    }
    View  v = views[viewId];
    bitmap = Bitmap(v.photoWidth, v.photoHeight);
    bitmapDepth = Bitmap(v.photoWidth, v.photoHeight);
    
    bitmap.clear(glm::vec4(0.4,0.4,0.4,1));
    Rasterizer rasterizer(bitmap.width,bitmap.height);
    rasterizer.bindMesh(mesh);
    
    Transformation transform;
    transform.setCamera(v.camera);
    transform.setAspectRatio(bitmap.width, bitmap.height);
    
    glm::mat4 cameraModelTransform = transform.getViewProjection()*
    transform.getModelMatrix();
    
    printMatrix(cameraModelTransform);
    
    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(&bitmap);
    rasterizer.drawMesh();
    rasterizer._getDepthBitmap(bitmapDepth);
    views[viewId].visibleFaces = rasterizer.getVisibleFaces();
}



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

void windowRender( Mesh mesh );

TextureExtractor::~TextureExtractor(){
    for(auto & v : views){
        v.second.releaseImage();
    }
}

bool TextureExtractor::prepareViews(const std::string & cameraInfoPath, const std::string &  cameraListFilePath){

    std::vector<std::string> photoPaths;

    std::cout<<"Reading the list of source Photos\n";
    bool listOk;
    listOk = extractPhotoList(photoPaths,cameraListFilePath);
    if(!listOk){
        std::cout<<"ERROR occured when getting photo list\n";
        return false;
    }

    std::cout<<"Reading Camera info (position, rotation, fov)\n";
    bool cameraInfoOK;
    cameraInfoOK = extractCameraInfoCreateViews(photoPaths, cameraInfoPath);
    if(!cameraInfoOK){
        std::cout<<"ERROR occured when getting camera info\n";
        return false;
    }

    return true;
}


bool TextureExtractor::calculateDataCosts(){
    
    return true;
}


bool TextureExtractor::selectViews(){
    //FIXME: actual label selection
    for(auto & f : mesh.triangles){
        Triangle & face = f.second;
        for(auto & v : views){
            View & view = v.second;
            if(view.visibleFaces.count(face.id)>0){
                face.viewId = view.id;
                break;
            }
        }
    }
    
    return true;
}


bool TextureExtractor::generateTexture(const std::string & newTexturePath, int width, int height){
    texture = Bitmap(width, height);
    
    texture.clear(glm::vec4(0.8, 0.8, 0.8, 1));
    glm::vec4 defaultColor (0.37, 0.61, 0.62, 1);
    
    ExtractionWorker worker(mesh);
    for(auto & f : mesh.triangles){
        Triangle & face = f.second;
        if(face.viewId != 0){
            worker.extract(face, texture, views[face.viewId]);
        }else{
            worker.fillTextureTriangle(face,defaultColor,texture);
        }
    }
    texture.toPPM(newTexturePath);
    
    return true;
}


bool TextureExtractor::extractPhotoList(std::vector<std::string> & photoPaths,const std::string & cameraListFilePath){
    
    std::ifstream file;
    file.open(cameraListFilePath.c_str());
    
    std::string line;
    if(!file.is_open()){
        std::cerr << "Unable to open camera list file: " << cameraListFilePath << std::endl;
        return false;
    }
    
    while(file.good()){
        getline(file, line);
        if(line.size()>0){
            View view;
            view.fileName = photoFolderPath +'/' +line;
            //original photo dimensions
            int width, height, comp;
            bool imageOk;
            imageOk = stbi_info(view.fileName.c_str(), &width, &height, &comp);
            if(!imageOk){
                std::cout<<"ERROR Wasnt able to read the file:" << view.fileName <<"\n";
                return false;
            }
            view.photoWidth = width;
            view.photoHeight = height;
            addView(view);
        }
    }
    
    if(views.size()<=0){
        std::cout<< "File was empty or error occured\n";
        return false;
    }
    return true;
}


bool TextureExtractor::extractCameraInfoCreateViews(const std::vector<std::string> &photoPaths,const std::string & cameraInfoPath){
    std::ifstream file;
    file.open(cameraInfoPath.c_str());
    
    std::string line;
    if(!file.is_open()){
        std::cerr << "Unable to open camera info file: " << cameraInfoPath << std::endl;
        return false;
    }
    
    std::cout<<"Reading Bundler format\n";
    getline(file, line);
    if(line.at(0)!='#'){
        std::cout<<"Bundler format violated. First comment line missing (\"# ....\")\n";
        return false;
    }
    
    getline(file, line);
    std::vector<std::string> tokens;
    tokens = splitString(line, ' ');
    if(tokens.size()!=2){
        std::cout<<"Bundler format violated.\"camera_count point_count\" line missing\n";
        return false;
    }
    
    uint cameraCount = parseInt(tokens[0]);
    if(cameraCount<=0){
        std::cout<<"Number of cameras is invalid: "<<cameraCount<<"\n";
        return false;
    }
    
    if(cameraCount<views.size()){
        std::cout<<"Number of cameras is less than photo list: "<<
                   "    list_size = "<<views.size()<<"\n"<<
                   "    camera_info_size = "<<cameraCount<<"\n";
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
        std::cout << "ERROR camera info count missmatch\n"<<
                     "    got: "<<readCounter<<" expected: " << views.size()<<"\n";
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


void TextureExtractor::renderView(Bitmap & bitmap,const Bitmap & texture,uint viewId){
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
    rasterizer.setTexture(texture);

    Transformation transform;
    transform.setCamera(v.camera);
    transform.setAspectRatio(bitmap.width, bitmap.height);

    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(&bitmap);
    rasterizer.drawMesh();
    views[viewId].visibleFaces = rasterizer.getVisibleFaces();
}


void TextureExtractor::renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,const Bitmap & texture,uint viewId){
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
    rasterizer.setTexture(texture);
    
    Transformation transform;
    transform.setCamera(v.camera);
    transform.setAspectRatio(bitmap.width, bitmap.height);
    
    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(&bitmap);
    rasterizer.drawMesh();
    rasterizer._getDepthBitmap(bitmapDepth);
    views[viewId].visibleFaces = rasterizer.getVisibleFaces();
}


void TextureExtractor::windowRender( uint viewId ){
    if(views.find(viewId)==views.end()){
        std::cout<<"No such viewId:"<<viewId<<"\n";
        return;
    }
    View view = views[viewId];
    
    Bitmap texture("./resources/cs.png");
    int width = 960, height = 540;
    view.camera.fov = 0.88922719077834;
    
    Display *  display = new Display(width, height, "TEST123");
    
    Rasterizer rasterizer(width,height);
    rasterizer.bindMesh(mesh);
    rasterizer.setTexture(texture);
    
    Transformation transform;
    transform.setCamera(view.camera);
    transform.setAspectRatio(width, height);
    
    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(display);
    rasterizer.drawMesh();
    
    auto previousTime = std::chrono::high_resolution_clock::now();
    float counter = 0.0f;
    
    while (!(display->isColsed())){
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto delta = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime-previousTime).count()/1000000000;
        previousTime = currentTime;
        rasterizer.clearBuffer();
        
        //        transform.pos.x = sin(counter*0.5)*4;
        //        transform.rot.x = counter/2;
        //        transform.rot.z = counter/2;
        //        transform.rot.y = counter/2;
        
//        transform.rot.y = sin(counter*0.5);
        
//         transform.rot.x = sin(counter*0.5);
//         transform.rot.z = sin(counter*0.5);
        //        transform.rot.y = 1;
        //        transform.rot.x = 1;
        //        transform.pos.x = 2;
        
        rasterizer.setTransformation(transform);
        display->clear(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        rasterizer.drawMesh();
        display->update();
        counter += delta*0.5;
    }
    delete display;
    std::cout<<"Display closed!\n";
}


bool TextureExtractor::_old_test_render(){
    //    TODO: set martix to 0
    Bitmap * bitmap = new Bitmap(4032,3024);
    bitmap->clear(glm::vec4(0.4,0.4,0.4,1));
    View v;
    v.camera = Camera(true);
    v.camera.fov = 0.88922719077834;
    v.camera.rotationMatrix[0][0] =  -0.272774424923  ;
    v.camera.rotationMatrix[1][0] =  -0.33027662254  ;
    v.camera.rotationMatrix[2][0] =  -0.90361192824  ;
    
    v.camera.rotationMatrix[0][1] =  0.884159272993  ;
    v.camera.rotationMatrix[1][1] =  -0.456338040783  ;
    v.camera.rotationMatrix[2][1] =  -0.1001074003  ;
    
    v.camera.rotationMatrix[0][2] =  -0.379288518971  ;
    v.camera.rotationMatrix[1][2] =  -0.826241714125  ;
    v.camera.rotationMatrix[2][2] =  0.416493523273  ;
    
    //   -0.272774424923 -0.33027662254 -0.90361192824
    //    -0.884159272993 0.456338040783 0.1001074003
    //    0.379288518971 0.826241714125 -0.416493523273
    
    //real
//    -0.272774424923 -0.33027662254 -0.90361192824
//    0.884159272993 -0.456338040783 -0.1001074003
//    -0.379288518971 -0.826241714125 0.416493523273
    
    v.camera.translation = glm::vec3(-0.461685315155,-0.533333463975,-1.43486184825);
    
    Bitmap texture("./resources/cs.png");
    Rasterizer rasterizer(4032,3024);
    rasterizer.bindMesh(mesh);
    rasterizer.setTexture(texture);
    
    Transformation transform;
    transform.setCamera(v.camera);
    transform.setAspectRatio(4032, 3024);
    
    rasterizer.setTransformation(transform);
    rasterizer.clearBuffer();
    rasterizer.setRenderContext(bitmap);
    rasterizer.drawMesh();
    bitmap->toPPM("./resources/render_test.ppm");
    //    windowRender(mesh);
    return true;
}


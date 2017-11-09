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

void windowRender( Mesh mesh );

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


bool TextureExtractor::selectViews(){
    //TODO:
    
    return true;
}


bool TextureExtractor::generateTexture(const std::string & newTexturePath, int width, int height){
    //TODO:
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
            view.fileName = line;
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
    
    if(cameraCount!=views.size()){
        std::cout<<"Number of cameras doesent match the photo list: "<<
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
    View view = views[readCounter];
    bool got3Float;
    
    //focal length;
    got3Float = get3Floats(tokens, file);
    if(!got3Float)
        return false;
    view.camera.focalLength = tokens[0];
    
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
    
    
    views[readCounter] = view;
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
    id = (uint)views.size();
    newView.id = id;
    views[id] = newView;
    return id;
}

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
    //FIXME:accual fov;
    v.camera.fov = 0.88922719077834;

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
}


void TextureExtractor::renderViewAndDepth(Bitmap & bitmap,Bitmap & bitmapDepth,const Bitmap & texture,uint viewId){
    if(views.find(viewId)==views.end()){
        std::cout<<"No such viewId:"<<viewId<<"\n";
        return;
    }
    bitmap.clear(glm::vec4(0.4,0.4,0.4,1));
    View  v = views[viewId];
    //FIXME:accual fov;
    v.camera.fov = 0.88922719077834;
    
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



//void TextureExtractor::extractTexture(Bitmap * source, std::vector<std::pair<uint,float>> * scoreTable){
//    this->source = source;
//    this->sourceScoreTable = scoreTable;
//    for(auto triangle : mesh.triangles){
//        processTriangle(triangle.second);
//    }
//}
//
//void TextureExtractor::processTriangle(const Triangle & triangle){
//    Vertex vOne = mesh.verticies.at(triangle.verticies[0]);
//    Vertex vTwo = mesh.verticies.at(triangle.verticies[1]);
//    Vertex vThree = mesh.verticies.at(triangle.verticies[2]);
//
//    vOne.texCoord = mesh.texCoords.at(triangle.texCoords.at(vOne.id)).coord;
//    vTwo.texCoord = mesh.texCoords.at(triangle.texCoords.at(vTwo.id)).coord;
//    vThree.texCoord = mesh.texCoords.at(triangle.texCoords.at(vThree.id)).coord;
//
//    glm::mat4 cameraModelTransform = transformation.getViewProjection()*
//    transformation.getModelMatrix();
//
//    vOne =   cameraModelTransform * vOne;
//    vTwo =   cameraModelTransform * vTwo;
//    vThree = cameraModelTransform * vThree;
//
//    if(isInsideViewFrustrum(vOne) && isInsideViewFrustrum(vTwo) && isInsideViewFrustrum(vThree)){
//        extractTriangle(vOne, vTwo, vThree, triangle);
//        return;
//    }
//
//    if(!isInsideViewFrustrum(vOne) && !isInsideViewFrustrum(vTwo) && !isInsideViewFrustrum(vThree)){
//        return;
//    }
//
//    std::vector<Vertex> verticies;
//    verticies.push_back(vOne);
//    verticies.push_back(vTwo);
//    verticies.push_back(vThree);
//
//    if(clipPoligonAxis(verticies, 0) &&
//       clipPoligonAxis(verticies, 1) &&
//       clipPoligonAxis(verticies, 2)){
//
//        Vertex initVertex = *verticies.begin();
//        for(int i = 1; i<(verticies.size()-1); i++ ){
//            extractTriangle(initVertex, verticies.at(i), verticies.at(i+1), triangle);
//        }
//
//    }
//}
//
//void TextureExtractor::extractTriangle(Vertex  minYVert, Vertex midYVert, Vertex maxYVert,const Triangle & triangle){
//    glm::mat4 screenSpaceTransform = transformation.getScreenTransform();
//
//    minYVert = transformation.doPerspectiveDevide(screenSpaceTransform * minYVert);
//    midYVert = transformation.doPerspectiveDevide(screenSpaceTransform * midYVert);
//    maxYVert = transformation.doPerspectiveDevide(screenSpaceTransform * maxYVert);
//
//    if (triangleArea(minYVert, maxYVert, midYVert) <= 0){
//        return; //BackFace culling
//    }
//
//    if(minYVert.texCoord.y>midYVert.texCoord.y)
//        std::swap(minYVert, midYVert);
//    if(midYVert.texCoord.y>maxYVert.texCoord.y)
//        std::swap(maxYVert, midYVert);
//    if(minYVert.texCoord.y>midYVert.texCoord.y)
//        std::swap(minYVert, midYVert);
//
//
//    minYVert.texCoord.x = minYVert.texCoord.x*(texture.width - 1);
//    minYVert.texCoord.y = minYVert.texCoord.y*(texture.height - 1);
//
//    midYVert.texCoord.x = midYVert.texCoord.x*(texture.width - 1);
//    midYVert.texCoord.y = midYVert.texCoord.y*(texture.height - 1);
//
//    maxYVert.texCoord.x = maxYVert.texCoord.x*(texture.width - 1);
//    maxYVert.texCoord.y = maxYVert.texCoord.y*(texture.height - 1);
//
//    float area = triangleAreaTexture(minYVert, maxYVert, midYVert);
//
//    bool handedness = (area >= 0);
//
//    fillTriangle(minYVert, midYVert, maxYVert, handedness, triangle);
//}
//
//
//void TextureExtractor::fillTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert ,bool handedness,const Triangle & triangle){
//
//    TextureGradient gradient(minYVert,midYVert,maxYVert);
//
//    TextureEdge topToBottom(minYVert, maxYVert, gradient, 0);
//    TextureEdge topToMiddle(minYVert, midYVert, gradient, 0);
//    TextureEdge middleToBottom(midYVert, maxYVert, gradient, 1);
//
//
//    TextureEdge * left = &topToBottom;
//    TextureEdge * right = &topToMiddle;
//
//    if(handedness)
//        std::swap(left,right);
//
//    int yStart = topToMiddle.yStart;
//    int yEnd   = topToMiddle.yEnd;
//
//    for(int j = yStart; j< yEnd; j++){
//        drawScanLine(*left,*right,j,gradient,triangle.id);
//        left->Step();
//        right->Step();
//    }
//
//    left = & topToBottom;
//    right = & middleToBottom;
//
//    if(handedness)
//        std::swap(left,right);
//
//    yStart =  middleToBottom.yStart;
//    yEnd   =  middleToBottom.yEnd;
//
//    for(int j = yStart; j<yEnd; j++){
//        drawScanLine(*left,*right,j,gradient,triangle.id);
//        left->Step();
//        right->Step();
//    }
//
//}
//
//
//void TextureExtractor::drawScanLine(TextureEdge left, TextureEdge right, int y, TextureGradient & gradient, uint id){
//    int xMin = (int)ceil(left.currentX);
//    int xMax = (int)ceil(right.currentX);
//    float xPrestep = xMin - left.currentX;
//    float xDist = right.currentX - left.currentX;
//
//    //texture
//    float photoCoordXXStep = (right.photoCoord.x - left.photoCoord.x)/xDist;
//    float photoCoordYXStep = (right.photoCoord.y - left.photoCoord.y)/xDist;
//    glm::vec2 photoCoord;
//    photoCoord.x = left.photoCoord.x + photoCoordXXStep*xPrestep;
//    photoCoord.y = left.photoCoord.y + photoCoordYXStep*xPrestep;
//
//    //overZ
//    float oneOverZXStep = (right.oneOverZ - left.oneOverZ)/xDist;
//    float oneOverZ = left.oneOverZ + oneOverZXStep*xPrestep;
//
//    for(int x = xMin; x<xMax ; x++){
//
//        float z = 1/oneOverZ;
//        int photoX = (int)((photoCoord.x * z) + 0.5f);
//        int photoY = (int)((photoCoord.y * z) + 0.5f);
//        //
//        //        int photoX = (int)((photoCoord.x) + 0.5f);
//        //        int photoY = (int)((photoCoord.y) + 0.5f);
//        //
//        int index = photoX + photoY*source->width;
//        int myScoreIndex = x + y*width;
//
//        if(true){
////        if((*sourceScoreTable)[index].first == id){
//            if(scoreTable[myScoreIndex] < (*sourceScoreTable)[index].second ){
//                texture.putPixel(x, y, source->at(photoX, photoY));
//                scoreTable[myScoreIndex] = (*sourceScoreTable)[index].second;
//            }
//        }else{
//            //            std::cout<<scoreTable[index].first<<"\n";
//        }
//        photoCoord.x += photoCoordXXStep;
//        photoCoord.y += photoCoordYXStep;
//        oneOverZ   += oneOverZXStep;
//
//    }
//}
//
//
//bool TextureExtractor::clipPoligonAxis (std::vector<Vertex> & verticies, int component){
//    std::vector<Vertex> result;
//    clipPoligonComponent(verticies, result, component, 1);
//    verticies.clear();
//    if(result.empty()){
//        return false;
//    }
//    clipPoligonComponent(result, verticies, component, -1);
//    return !verticies.empty();
//}
//
//void TextureExtractor::clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor){
//
//    Vertex prevVertex = verticies.back();
//    float prevComponent = prevVertex.get(component) * clipFactor;
//    bool prevInside = prevComponent <= prevVertex.w();
//
//    for(auto currentVertex = verticies.begin() ; currentVertex< verticies.end();currentVertex++){
//        float currentComponent = currentVertex->get(component) * clipFactor;
//        bool currentInside = currentComponent <= currentVertex->w();
//
//        if(currentInside ^ prevInside){
//            float lerpAmmount = (prevVertex.w() - prevComponent) /
//            ((prevVertex.w() - prevComponent) - ( currentVertex->w() - currentComponent));
//
//            result.push_back(prevVertex.lerp(*currentVertex, lerpAmmount));
//        }
//        if(currentInside){
//            result.push_back(*currentVertex);
//        }
//        prevVertex = *currentVertex;
//        prevComponent = currentComponent;
//        prevInside = currentInside;
//    }
//
//}
//
//
//bool TextureExtractor::isInsideViewFrustrum (Vertex v){
//    return (abs(v.x()) <= abs(v.w())) &&
//    (abs(v.y()) <= abs(v.w())) &&
//    (abs(v.z()) <= abs(v.w()));
//}
//
//
//float TextureExtractor::triangleArea(Vertex v1, Vertex v2, Vertex v3){
//    glm::vec4 a = v1.coord;
//    glm::vec4 b = v2.coord;
//    glm::vec4 c = v3.coord;
//
//    glm::vec4 p1;
//    p1.x = b.x - a.x;
//    p1.y = b.y - a.y;
//
//    glm::vec4 p2;
//    p2.x = c.x - a.x;
//    p2.y = c.y - a.y;
//
//    return ( p1.x * p2.y - p2.x * p1.y);
//}
//
//
//float TextureExtractor::triangleAreaTexture(Vertex v1, Vertex v2, Vertex v3){
//    glm::vec2 a = v1.texCoord;
//    glm::vec2 b = v2.texCoord;
//    glm::vec2 c = v3.texCoord;
//
//    glm::vec4 p1;
//    p1.x = b.x - a.x;
//    p1.y = b.y - a.y;
//
//    glm::vec4 p2;
//    p2.x = c.x - a.x;
//    p2.y = c.y - a.y;
//
//    return ( p1.x * p2.y - p2.x * p1.y);
//}


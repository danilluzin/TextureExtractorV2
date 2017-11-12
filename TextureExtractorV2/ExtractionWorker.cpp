//
//  ExtractionWorker.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 12/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "ExtractionWorker.hpp"











//----------------------------------------------------------------------------//







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


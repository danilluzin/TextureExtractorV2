//
//  ExtractionWorker.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 12/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "ExtractionWorker.hpp"
#include <glm/ext.hpp>
#include "Utils.h"

bool ExtractionWorker::clipPoligonAxis (std::vector<Vertex> & verticies, int component){
    std::vector<Vertex> result;
    clipPoligonComponent(verticies, result, component, 1);
    verticies.clear();
    if(result.empty()){
        return false;
    }
    clipPoligonComponent(result, verticies, component, -1);
    return !verticies.empty();
}

void ExtractionWorker::clipPoligonComponent (std::vector<Vertex> & verticies, std::vector<Vertex> & result , int component, int clipFactor){
    Vertex prevVertex = verticies.back();
    float prevComponent = prevVertex.get(component) * clipFactor;
    bool prevInside = prevComponent <= prevVertex.w();

    for(auto currentVertex = verticies.begin() ; currentVertex< verticies.end();currentVertex++){
        float currentComponent = currentVertex->get(component) * clipFactor;
        bool currentInside = currentComponent <= currentVertex->w();

        if(currentInside ^ prevInside){
            float lerpAmmount = (prevVertex.w() - prevComponent) /
            ((prevVertex.w() - prevComponent) - ( currentVertex->w() - currentComponent));

            result.push_back(prevVertex.lerp(*currentVertex, lerpAmmount));
        }
        if(currentInside){
            result.push_back(*currentVertex);
        }
        prevVertex = *currentVertex;
        prevComponent = currentComponent;
        prevInside = currentInside;
    }

}


float ExtractionWorker::triangleArea(Vertex v1, Vertex v2, Vertex v3){
    glm::vec4 a = v1.coord;
    glm::vec4 b = v2.coord;
    glm::vec4 c = v3.coord;

    glm::vec4 p1;
    p1.x = b.x - a.x;
    p1.y = b.y - a.y;

    glm::vec4 p2;
    p2.x = c.x - a.x;
    p2.y = c.y - a.y;

    return ( p1.x * p2.y - p2.x * p1.y);
}


float ExtractionWorker::triangleAreaTexture(Vertex v1, Vertex v2, Vertex v3){
    glm::vec2 a = v1.texCoord;
    glm::vec2 b = v2.texCoord;
    glm::vec2 c = v3.texCoord;

    glm::vec4 p1;
    p1.x = b.x - a.x;
    p1.y = b.y - a.y;

    glm::vec4 p2;
    p2.x = c.x - a.x;
    p2.y = c.y - a.y;

    return ( p1.x * p2.y - p2.x * p1.y);
}



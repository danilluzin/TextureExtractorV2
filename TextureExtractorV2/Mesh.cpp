//
//  Mesh.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Mesh.hpp"
#include "Utils.h"
#include "Arguments.h"

bool Mesh::initialize(const std::string & filename){
    bool fileLoaded = loadFromFile(filename);
    if(!fileLoaded){
        return false;
    }
    buildAdjacencyGraph();
    return true;
}

Direction getOptimalSeparation(const BoundingBox & boundinBox){
    float xLength = abs( boundinBox.maxVec.x - boundinBox.minVec.x);
    float yLength = abs( boundinBox.maxVec.y - boundinBox.minVec.y);
    float zLength = abs( boundinBox.maxVec.z - boundinBox.minVec.z);
    
    if(xLength >= yLength && xLength >= zLength  )
        return X;
    if(yLength >= xLength && yLength >= zLength  )
        return Y;
    return Z;
}


void Mesh::buildAdjacencyGraph(){
    for(auto t : triangles){
        Triangle face = t.second;
        uint vert [3];
        vert[0] = face.verticies[0];
        vert[1] = face.verticies[1];
        vert[2] = face.verticies[2];
        
        std::vector<uint> edge1AdjacentFaces = getEdgeAdjacentFaces(vert[0],vert[1]);
        std::vector<uint> edge2AdjacentFaces = getEdgeAdjacentFaces(vert[1],vert[2]);
        std::vector<uint> edge3AdjacentFaces = getEdgeAdjacentFaces(vert[2],vert[0]);
        
        std::set<uint> adjacents;
        adjacents.insert(edge1AdjacentFaces.begin(),edge1AdjacentFaces.end());
        adjacents.insert(edge2AdjacentFaces.begin(),edge2AdjacentFaces.end());
        adjacents.insert(edge3AdjacentFaces.begin(),edge3AdjacentFaces.end());
        
        std::set<uint> neighbours;
        for(auto f : adjacents){
            if(f != face.id){
                neighbours.insert(f);
            }
            adjacencyGraph.addNode(face.id, neighbours);
        }
    }
}


std::vector<uint> Mesh::getEdgeAdjacentFaces(uint vert1, uint vert2){
    
    std::set<uint> vert1Faces = facesVertexBelongsTo[vert1];
    std::set<uint> vert2Faces = facesVertexBelongsTo[vert2];
    uint maxSize = (uint)std::max(vert2Faces.size(),vert1Faces.size());
    
    std::vector<uint> intersection(maxSize);
    std::vector<uint>::iterator it;
    
    it = std::set_intersection (vert1Faces.begin(), vert1Faces.end(), vert2Faces.begin(), vert2Faces.end(), intersection.begin());
    intersection.resize(it-intersection.begin());
    
    return intersection;
}


Vertex operator * (const glm::mat4 & matrix , const Vertex & vertex){
    Vertex res(matrix*vertex.coord);
    res.id = vertex.id;
    res.texCoord = vertex.texCoord;
    return res;
}

float Vertex::get(int component){
    switch (component) {
        case 0:
            return coord.x;
            break;
        case 1:
            return coord.y;
            break;
        case 2:
            return coord.z;
            break;
        case 3:
            return coord.w;
            break;
        default:
            return -1;
            break;
    }
}

Mesh & Mesh::addVertex(Vertex vertex){
    vertex.id = (uint)verticies.size() + 1;
    verticies[vertex.id] = vertex;
    return *this;
}



Mesh & Mesh::addTexCoord(TexCoord texCoord){
    texCoord.id = (uint)texCoords.size() + 1;
    texCoords[texCoord.id] = texCoord;
    return *this;

}

Mesh & Mesh::addNormal(Normal normal){
    normal.id = (uint)normals.size() + 1;
    normals[normal.id] = normal;
    return *this;
}

uint Mesh::addTriangle(Triangle triangle){
    triangle.id = (uint)triangles.size() + 1;
    triangles[triangle.id] = triangle;
    return triangle.id;
}

bool Mesh::isValidFaceId(int id){
    return (id >= 1) && (id<=triangles.size());
}

Vertex Vertex::lerp(const Vertex & other, float ammount){
    Vertex res;
    res.coord = glm::mix(this->coord, other.coord, ammount);
    res.texCoord = glm::mix(this->texCoord, other.texCoord, ammount);
    return res;
}


bool Mesh::loadFromFile(const std::string &filename){
    std::ifstream file;
    file.open(filename.c_str());
    
    std::string line;
    if(!file.is_open()){
        printError("Unable to load mesh: "+filename+"\n");
        return false;
    }
    bool canAdd = false;
    Object newObject;
    newObject.name = "default";
    while(file.good()){
        getline(file, line);
        
        unsigned int lineLength = (uint)line.length();
        
        if(lineLength < 2)
            continue;
        
        const char* lineCStr = line.c_str();
        
        switch(lineCStr[0]){
            case 'v':{
                if(lineCStr[1] == 't'){
                    //texcoord
                    parseTexCoord(splitString(line, ' '));
                }
                else if(lineCStr[1] == 'n'){
                    //normal
                    parseNormal(splitString(line, ' '));
                }
                else if(lineCStr[1] == ' ' || lineCStr[1] == '\t'){
                    //vertex
                    parseVertex(splitString(line, ' '));
                }
                break;
            }
            case 'f':{
                //triangle,plane
                std::vector<std::string> tokens = splitString(line, ' ');
                if(tokens.size()==4){
                    std::vector<std::string> one = splitString(tokens[1], '/');
                    std::vector<std::string> two = splitString(tokens[2], '/');
                    std::vector<std::string> three = splitString(tokens[3], '/');
                    parseTriangle(one,two,three,newObject);
                }
                if(tokens.size()==5)
                    parsePlane(tokens,newObject);
                break;
            }
            case 'o':{
                if(!canAdd){
                    objectNames.push_back(line.substr(2,line.size()));
                    newObject.name = line.substr(2,line.size());
                    canAdd = true;
                }else{
                    objects.push_back(newObject);
                    newObject = Object();
                    objectNames.push_back(line.substr(2,line.size()));
                    newObject.name = line.substr(2,line.size());
                }
            }
            default: break;
        };
    }
    objects.push_back(newObject);
    file.close();

    preparePartition();
    return true;
}

void Mesh::preparePartition(){
    for(auto & o : objects){
        o.partitionRoot.triangles = o.triangles;
        o.partitionRoot.boundingBox = o.boundingBox;
        constructNode(&o.partitionRoot);
    }
}

void Mesh::constructNode(PartitionNode * node){

    if(node->triangles.size() <= arguments.BVHMinNode)
        return;
    node->direction = getOptimalSeparation(node->boundingBox);
    node->leftNode = new PartitionNode();
    node->rightNode = new PartitionNode();
    node->leftNode->parent = node->rightNode->parent = node;
    switch (node->direction) {
        case X:
            node->separator = ( node->boundingBox.maxVec.x + node->boundingBox.minVec.x)/2;
            for(auto t : node->triangles){
                if(triangles[t].boundingBox.maxVec.x < node->separator){
                    node->leftNode->addTriangle(triangles[t]);
                }else{
                    node->rightNode->addTriangle(triangles[t]);
                }
            }
            break;
        case Y:
            node->separator = ( node->boundingBox.maxVec.y + node->boundingBox.minVec.y)/2;
            for(auto & t : node->triangles){
                if(triangles[t].boundingBox.maxVec.y < node->separator){
                    node->leftNode->addTriangle(triangles[t]);
                }else{
                    node->rightNode->addTriangle(triangles[t]);
                }
            }
            break;
        case Z:
            node->separator = ( node->boundingBox.maxVec.z + node->boundingBox.minVec.z)/2;
            for(auto & t : node->triangles){
                if(triangles[t].boundingBox.maxVec.z < node->separator){
                    node->leftNode->addTriangle(triangles[t]);
                }else{
                    node->rightNode->addTriangle(triangles[t]);
                }
            }
            break;
        default:
            break;
    }
    if(node->leftNode->triangles.size() == 0 || node->rightNode->triangles.size() == 0){
        //edge case where cant devide
        return;
    }
    constructNode(node->leftNode);
    constructNode(node->rightNode);
}


void Mesh::parseVertex(std::vector<std::string> tokens){
    float x = parseFloat(tokens[1]);
    float y = parseFloat(tokens[2]);
    float z = parseFloat(tokens[3]);
    addVertex(Vertex(glm::vec4(x,y,z,1)));
}

void Mesh::parseTexCoord(std::vector<std::string> tokens){
    float x = parseFloat(tokens[1]);
    float y = parseFloat(tokens[2]);
    addTexCoord(TexCoord(x, y));
}

void Mesh::parseNormal(std::vector<std::string> tokens){
    float x = parseFloat(tokens[1]);
    float y = parseFloat(tokens[2]);
    float z = parseFloat(tokens[3]);
    addNormal(Normal(glm::vec4(x,y,z,1)));
}

void Mesh::parseTriangle (std::vector<std::string> one,
                          std::vector<std::string> two,
                          std::vector<std::string> three,
                          Object & object){

    uint vert [3];
    vert[0] = parseInt(one[0]);
    vert[1] = parseInt(two[0]);
    vert[2] = parseInt(three[0]);

    Triangle triangle;

    triangle.verticies[0] = vert[0];
    triangle.verticies[1] = vert[1];
    triangle.verticies[2] = vert[2];
    
    triangle.boundingBox.addVertex(verticies[vert[0]]);
    triangle.boundingBox.addVertex(verticies[vert[1]]);
    triangle.boundingBox.addVertex(verticies[vert[2]]);

    triangle.texCoords[ vert[0] ] = std::max(parseInt(one[1]),1);
    triangle.texCoords[ vert[1] ] = std::max(parseInt(two[1]),1);
    triangle.texCoords[ vert[2] ] = std::max(parseInt(three[1]),1);

    triangle.normalVecs[ vert[0] ] = parseInt(one[2]);
    triangle.normalVecs[ vert[1] ] = parseInt(two[2]);
    triangle.normalVecs[ vert[2] ] = parseInt(three[2]);

    uint triangleId = addTriangle(triangle);
    object.triangles.push_back(triangleId);
    object.boundingBox.addBoundingBox(triangle.boundingBox);
    
    facesVertexBelongsTo[vert[0]].insert(triangleId);
    facesVertexBelongsTo[vert[1]].insert(triangleId);
    facesVertexBelongsTo[vert[2]].insert(triangleId);
}


void Mesh::parsePlane(std::vector<std::string> tokens,
                      Object & object){
    std::vector<std::string> one = splitString(tokens[1], '/');
    std::vector<std::string> two = splitString(tokens[2], '/');
    std::vector<std::string> three = splitString(tokens[3], '/');
    std::vector<std::string> four = splitString(tokens[4], '/');

    parseTriangle(one, two, three,object);
    parseTriangle(one, three, four,object);
}

bool isInsideViewFrustrum(const Vertex & v){
    return (abs(v.x()) <= abs(v.w())) &&
    (abs(v.y()) <= abs(v.w())) &&
    (abs(v.z()) <= abs(v.w()));
}


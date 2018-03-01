//
//  Mesh.hpp
//  TextureExtractor
//
//  Created by Danil Luzin on 07/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <iostream>
#include <fstream>


class BoundingBox;

struct Vertex{
    Vertex (glm::vec4 coord = glm::vec4(0,0,0,0)){
        this->coord = coord;
    }
    
    Vertex (float x, float y, float z){
        this->coord = glm::vec4(x,y,z,1);
    }
    
    Vertex operator*(const glm::mat4 & matrix){
        return Vertex(matrix*coord);
    }
    
    Vertex lerp(const Vertex & other, float ammount);
    
    float x() const { return coord.x;}
    float y() const { return coord.y;}
    float z() const { return coord.z;}
    float w() const { return coord.w;}
    
    float get(int component);
    
    glm::vec4 coord;
    glm::vec2 texCoord;
    uint id;
};

struct BoundingBox{
    
    glm::vec4 minVec;
    
    glm::vec4 maxVec;
    
    BoundingBox(){
        minVec.x = minVec.y = minVec.z = std::numeric_limits<float>::max();
        maxVec.x = maxVec.y = maxVec.z = std::numeric_limits<float>::lowest();
        minVec.w = maxVec.w = 1;
    }
    
    void addVertex(const Vertex & v ){
        minVec.x = std::min(minVec.x , v.x());
        minVec.y = std::min(minVec.y , v.y());
        minVec.z = std::min(minVec.z , v.z());
        
        maxVec.x = std::max(maxVec.x , v.x());
        maxVec.y = std::max(maxVec.y , v.y());
        maxVec.z = std::max(maxVec.z , v.z());
    }
    
    void addBoundingBox(const BoundingBox & boundingBox){
        addVertex(Vertex(boundingBox.minVec));
        addVertex(Vertex(boundingBox.maxVec));
    }
};

struct Triangle {
    Triangle(const uint verticies[3]){
        this->verticies[0] = verticies[0];
        this->verticies[1] = verticies[1];
        this->verticies[2] = verticies[2];
    }
    Triangle(){};
    uint verticies [3];
    std::map<uint,uint> texCoords;
    std::map<uint,uint> normalVecs;
    uint id;
    uint viewId = 0;
    BoundingBox boundingBox;
};

struct TexCoord{
    TexCoord(float x, float y){
        coord.x = x;
        coord.y = y;
    }
    TexCoord(){}
    glm::vec2 coord;
    uint id;
    
};


struct Normal{
    Normal(glm::vec4 coord){
        normal = coord;
    }
    Normal(){}
    glm::vec4 normal;
    uint id;
};

class AdjacencyGraph{
    class Node{
    public:
        Node(uint faceId, const std::set<uint> & neighbours){
            this->faceId = faceId;
            this->neighbours = neighbours;
        }
        Node(){}
        std::set<uint> neighbours;
        uint faceId;
    };
public:
    void addNode(uint faceId,const std::set<uint> & neighbours){
        nodes[faceId] = Node(faceId, neighbours);
    }
    std::map<uint,Node> nodes;
};

enum Direction{
    X,Y,Z
};

struct PartitionNode{
    PartitionNode * leftNode = nullptr;
    PartitionNode * rightNode = nullptr;
    PartitionNode * parent = nullptr;
    std::vector<uint> triangles;
    BoundingBox boundingBox;
    float separator;
    Direction direction;
    void addTriangle(Triangle & triangle){
        triangles.push_back(triangle.id);
        boundingBox.addBoundingBox(triangle.boundingBox);
    }
};

struct Object{
    BoundingBox boundingBox;
    std::string name;
    std::vector<uint> triangles;
    PartitionNode partitionRoot;
};

class Mesh {
public:
    bool initialize(const std::string & filename);

    std::map<uint, Vertex>    verticies;
    
    std::map<uint, TexCoord> texCoords;
    
    std::map<uint, Normal> normals;
    
    std::map<uint,Triangle> triangles;
    
    std::map<uint,std::set<uint>> facesVertexBelongsTo;
    
    std::vector<Object> objects;
    
    bool isValidFaceId(int id);
    
    AdjacencyGraph adjacencyGraph;
private:
    
    std::vector<uint> getEdgeAdjacentFaces(uint vert1, uint vert2);
        
    void buildAdjacencyGraph();
    
    void preparePartition();
    
    bool loadFromFile(const std::string & filename);
    
    uint addTriangle(Triangle triangle);
    
    Mesh & addVertex(Vertex vertex);
    
    Mesh & addTexCoord(TexCoord texCoord);
    
    Mesh & addNormal(Normal normal);

    void parseVertex(std::vector<std::string>);
    
    void parseTexCoord(std::vector<std::string>);
    
    void parseNormal(std::vector<std::string>);

    void parseTriangle(std::vector<std::string> one,
                       std::vector<std::string> two,
                       std::vector<std::string> three,
                       Object & object);

    void parsePlane(std::vector<std::string> tokens, Object & object);
    
    std::vector<std::string> objectNames;
};


Vertex operator * (const glm::mat4 & matrix , const Vertex & vertex);

#endif /* Mesh_hpp */

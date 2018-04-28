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

/**
 * Vertex class. Hold coordinates postion and texture coordinates
 */
struct Vertex{
    /**
     * Constructs a new vertex using existing coords
     * @param coord new vertex coordinate
     */
    Vertex (glm::vec4 coord = glm::vec4(0,0,0,0)){
        this->coord = coord;
    }
    
    /**
     * Constructs a new vertex using existing coords
     * @param x x postion of new vertex coordinate
     * @param y y postion of new vertex coordinate
     * @param z z postion of new vertex coordinate
     */
    Vertex (float x, float y, float z){
        this->coord = glm::vec4(x,y,z,1);
    }
    
    /**
     * Traditional matrix x vertex multiplication
     * @param matrix matrix to multiply with
     * @return resulting vertex of multiplying matrix*coord
     */
    Vertex operator*(const glm::mat4 & matrix){
        return Vertex(matrix*coord);
    }
    
    /**
     * Calculates new vertex as linear interpolation of current and other one. Both position and texture coordinate get interpolated.
     * @param other other vertex
     * @return ammount lerp factor
     */
    Vertex lerp(const Vertex & other, float ammount);
    
    float x() const { return coord.x;}
    float y() const { return coord.y;}
    float z() const { return coord.z;}
    float w() const { return coord.w;}
    
    /**
     * Returns x,y,z or w component for the  component number
     * @param component component number. 0 : x, 1 : y, 2 : z, 3 : w
     * @return x, y, z or w component
     */
    float get(int component);
    
    /** Mesh 3D coordinate of the vertex*/
    glm::vec4 coord;
    
    /** Texture coordinate of the vertex*/
    glm::vec2 texCoord;
    
    /** ID of the vertex. Uniq in the mesh*/
    uint id;
};

/**
 * Structure for describing Bounding boxes
 */
struct BoundingBox{
    
    /** collection of minimal position values of the box minVec = (x_min, y_min, z_min, w_min)*/
    glm::vec4 minVec;
    
    /** collection of maximal position values of the box maxVec = (x_max, y_max, z_max, w_max)*/
    glm::vec4 maxVec;
    
    /**
     * Constructor. Creates the biggest boundig box possible.
     */
    BoundingBox(){
        minVec.x = minVec.y = minVec.z = std::numeric_limits<float>::max();
        maxVec.x = maxVec.y = maxVec.z = std::numeric_limits<float>::lowest();
        minVec.w = maxVec.w = 1;
    }
    
    /**
     * Adds a new vertex to existing bounding box. Box is expanded if necessary.
     * @param v vertex to add
     */
    void addVertex(const Vertex & v ){
        minVec.x = std::min(minVec.x , v.x());
        minVec.y = std::min(minVec.y , v.y());
        minVec.z = std::min(minVec.z , v.z());
        
        maxVec.x = std::max(maxVec.x , v.x());
        maxVec.y = std::max(maxVec.y , v.y());
        maxVec.z = std::max(maxVec.z , v.z());
    }
    
    /**
     * Adds one bounding box to another. Destination box gets expanded if needed
     * @param boundingBox box to add
     */
    void addBoundingBox(const BoundingBox & boundingBox){
        addVertex(Vertex(boundingBox.minVec));
        addVertex(Vertex(boundingBox.maxVec));
    }
};

/**
 * Structure describing basic triangle
 */
struct Triangle {
    
    Triangle(const uint verticies[3]){
        this->verticies[0] = verticies[0];
        this->verticies[1] = verticies[1];
        this->verticies[2] = verticies[2];
    }
    
    Triangle(){};
    
    /** set of three verticies */
    uint verticies [3];
    
    /** set of texture coordinates assigned to each verte in a triangle */
    std::map<uint,uint> texCoords;
    
    /** set of normals assigned to each verte in a triangle (not used in the tool)*/
    std::map<uint,uint> normalVecs;
    
    /** ID of a triangle */
    uint id;
    
    /** ID of a view assigned to a polygon */
    uint viewId = 0;
    
    /** Bounding box of the triangle */
    BoundingBox boundingBox;
};

/**
 * Structure that holds texture coordinate
 */
struct TexCoord{
    TexCoord(float x, float y){
        coord.x = x;
        coord.y = y;
    }
    TexCoord(){}
    
    /** coordinate u and v */
    glm::vec2 coord;
    
    /** coord ID */
    uint id;
    
};

/**
 * Structure that holds normal (not used in final tool)
 */
struct Normal{
    Normal(glm::vec4 coord){
        normal = coord;
    }
    Normal(){}
    /** normal vector*/
    glm::vec4 normal;
    /** normal ID */
    uint id;
};

/**
 * Class representing polygon adjacency in the mesh. Polygons are considered adjacent if they share one common edge.
 */
class AdjacencyGraph{
    /**
     * Class that represents one adjacency graph node.
     */
    class Node{
    public:
        Node(uint faceId, const std::set<uint> & neighbours){
            this->faceId = faceId;
            this->neighbours = neighbours;
        }
        Node(){}
        /** Set of adjacent polygons referenced by ID */
        std::set<uint> neighbours;
        
        /** ID of the node and face simultaniously */
        uint faceId;
    };
public:

    /**
     * Add a new node to a graph
     * @param faceId ID of new node and face
     * @param neighbours neighbours of the new node
     */
    void addNode(uint faceId,const std::set<uint> & neighbours){
        nodes[faceId] = Node(faceId, neighbours);
    }
    /** Collection of all nodes referenced by their ID.  */
    std::map<uint,Node> nodes;
};

/**
 * Direction enum. Used to identify the axis of bounding box separation.
 */
enum Direction{
    X,Y,Z,NONE
};

/**
 * Bounding volume hierarhy node, contains list of triangles that belong to a node and a bounding box
 */
struct PartitionNode{
    /** left child node of the current node */
    PartitionNode * leftNode  = nullptr;
    
    /** right child node of the current node */
    PartitionNode * rightNode = nullptr;
    
    /** parent node of the current node */
    PartitionNode * parent    = nullptr;
    
    /** set of triangles that belong to the current node. Referenced by the ID */
    std::vector<uint> triangles;
    
    /** Bounding box of the current node */
    BoundingBox boundingBox;
    
    /** Delta separator of the current node. Determines how triangles are separated into child nodes */
    float separator;
    
    /** Direction of the bounding box separation*/
    Direction direction = NONE;
    
    /** Adds triangle to a node. Updates bounding box as well. */
    void addTriangle(Triangle & triangle){
        triangles.push_back(triangle.id);
        boundingBox.addBoundingBox(triangle.boundingBox);
    }
};

/**
 * Structure that represents the mesh object
 */
struct Object{
    /** bounding box of the object */
    BoundingBox boundingBox;
    
    /** name of the object */
    std::string name;
    
    /** set of triangle that belong to the mesh. Referenced by ID */
    std::vector<uint> triangles;
    
    /** BVH root for the object */
    PartitionNode partitionRoot;
};


/**
 * Main mesh class. Manages all element dictionaries
 */
class Mesh {
public:
    /**
     * Reads and initializes the mesh from the file. Starts face adjacentcy graph
     * @param filename path to OBJ file
     */
    bool initialize(const std::string & filename);

    /** Dictionary of verticies*/
    std::map<uint, Vertex>    verticies;
    
    /** Dictionary of texture verticies*/
    std::map<uint, TexCoord> texCoords;
    
    /** Dictionary of the normals*/
    std::map<uint, Normal> normals;
    
    /** Dictionary of the trinagles*/
    std::map<uint,Triangle> triangles;
    
    /** For each vertex we store a set of triangles that it belongs to*/
    std::map<uint,std::set<uint>> facesVertexBelongsTo;
    
    /** Collection of the objects in the mesh*/
    std::vector<Object> objects;
    
    /** Checks if ID is a valid face ID*/
    bool isValidFaceId(int id);
    
    /** Triangle adjacency graph for the mesh*/
    AdjacencyGraph adjacencyGraph;
private:
    
    /**
     * Scans thorough facesVertexBelongsTo set to get the Faces that belong to the same edge
     * @param vert1 first vertex that makes up an edge
     * @param vert2 second vertex that meks up an edge
     */
    std::vector<uint> getEdgeAdjacentFaces(uint vert1, uint vert2);
    
    /**
     * Builds an adjacency graph of triangles
     */
    void buildAdjacencyGraph();
    
    /**
     * Build a BVH partion
     */
    void preparePartition();
    
    /**
     * Constructs a single partioion node. Starts recursive construction.
     * @param node node to construct.
     */
    void constructNode(PartitionNode * node);
    
    /**
     * Reads and parses the OBJ file
     * @param filename path to the OBJ file
     */
    bool loadFromFile(const std::string & filename);
    
    /**
     * Adds a triangle to a mesh. Assigns it an ID.
     * @param triangle triangle to add
     */
    uint addTriangle(Triangle triangle);
    
    /**
     * Adds a vertex to a mesh. Assigns it an ID.
     * @param vertex vertex to add
     */
    Mesh & addVertex(Vertex vertex);
    
    /**
     * Adds a texture coordinate to a mesh. Assigns it an ID.
     * @param texCoord texture coordinate to add
     */
    Mesh & addTexCoord(TexCoord texCoord);
    
    /**
     * Adds a normal to a mesh. Assigns it an ID (not used in tool).
     * @param normal normal to add
     */
    Mesh & addNormal(Normal normal);

    /**
     * Parses the vertex OBJ file line and adds a vertex to a mesh
     */
    void parseVertex(std::vector<std::string>);
    
    /**
     * Parses the texture coordinate OBJ file line and adds a texture coordinate to a mesh
     */
    void parseTexCoord(std::vector<std::string>);
    
    /**
     * Parses the normal OBJ file line and adds a normal to a mesh
     */
    void parseNormal(std::vector<std::string>);

    /**
     * Parses the three int strings from the OBJ file and adds new triangle to an object
     * @param one Int string of vertex IDs
     * @param two Int string of texture IDs
     * @param three Int string of normal IDs (not used)
     * @param object target object to add a parsed triangle
     */
    void parseTriangle(std::vector<std::string> one,
                       std::vector<std::string> two,
                       std::vector<std::string> three,
                       Object & object);

    /**
     * Parses the set of string tokens to get a new plane
     */
    void parsePlane(std::vector<std::string> tokens, Object & object);
    
    /** Object names */
    std::vector<std::string> objectNames;
};

/**
 * Traditional matrix x vertex multiplication
 * @param matrix matrix to multiply with
 * @return resulting vertex of multiplying matrix*coord
 */
Vertex operator * (const glm::mat4 & matrix , const Vertex & vertex);

/**
 * View frustum check in clipping space. Checks agains the w
 */
bool isInsideViewFrustrum(const Vertex & v);

#endif /* Mesh_hpp */

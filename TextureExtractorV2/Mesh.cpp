//
//  Mesh.cpp
//  TextureExtractor
//
//  Created by Danil Luzin on 08/10/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Mesh.hpp"

std::vector<std::string> splitString(const std::string &s, char delim);

float parseFloat(std::string txt);

float parseInt(std::string txt);

Vertex operator * (const glm::mat4 & matrix , const Vertex & vertex){
    Vertex res(matrix*vertex.coord,vertex.color);
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

Mesh & Mesh::addTriangle(Triangle triangle){
    triangle.id = (uint)triangles.size() + 18;
    triangles[triangle.id] = triangle;
    return *this;
}


Vertex Vertex::lerp(const Vertex & other, float ammount){
    Vertex res;
    res.coord = glm::mix(this->coord, other.coord, ammount);
    res.texCoord = glm::mix(this->texCoord, other.texCoord, ammount);
    return res;
}

Mesh::Mesh(const std::string & filename){
    
    std::ifstream file;
    file.open(filename.c_str());
    
    std::string line;
    if(!file.is_open()){
        std::cerr << "Unable to load mesh: " << filename << std::endl;
    }
 
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
                    parseTriangle(one,two,three);
                }
                if(tokens.size()==5)
                    parsePlane(tokens);
                break;
            }
            default: break;
        };
    }
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
                          std::vector<std::string> three){
    
    uint vert [3];
    vert[0] = parseInt(one[0]);
    vert[1] = parseInt(two[0]);
    vert[2] = parseInt(three[0]);
    
    Triangle triangle;
    
    triangle.verticies[0] = vert[0];
    triangle.verticies[1] = vert[1];
    triangle.verticies[2] = vert[2];
    
    triangle.texCoords[ vert[0] ] = parseInt(one[1]);
    triangle.texCoords[ vert[1] ] = parseInt(two[1]);
    triangle.texCoords[ vert[2] ] = parseInt(three[1]);
    
    triangle.normalVecs[ vert[0] ] = parseInt(one[2]);
    triangle.normalVecs[ vert[1] ] = parseInt(two[2]);
    triangle.normalVecs[ vert[2] ] = parseInt(three[2]);
    
    addTriangle(triangle);
}


void Mesh::parsePlane(std::vector<std::string> tokens){
    std::vector<std::string> one = splitString(tokens[1], '/');
    std::vector<std::string> two = splitString(tokens[2], '/');
    std::vector<std::string> three = splitString(tokens[3], '/');
    std::vector<std::string> four = splitString(tokens[4], '/');

    parseTriangle(one, two, three);
    parseTriangle(one, three, four);
}

float parseFloat(std::string txt){
    return std::stof (txt);
}

float parseInt(std::string txt){
    return std::stoi (txt);
}


std::vector<std::string> splitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    
    const char* cstr = s.c_str();
    uint strLength = (uint)s.length();
    uint start = 0;
    uint end = 0;
    
    while(end <= strLength){
        while(end <= strLength){
            if(cstr[end] == delim)
                break;
            end++;
        }
        
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
    
    return elems;
}

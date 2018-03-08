//
//  Utils.h
//  TextureExtractorV2
//
//  Created by Danil Luzin on 06/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Misc.h"
#include <sstream>
#include <iomanip>

static bool verbose = true;
extern int pad;



class Coord{
public:
    int x;
    int y;
    Coord(int x,int y):x(x),y(y){}
    bool operator < (const Coord & other) const{
       return x<other.x?true:y<other.y;
    }
    Coord operator + (const Coord & other) const{
        return Coord(x+other.x, y+other.y);
    }
    
};

static const Coord dir[8] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

inline std::string fitPercent(float p){
    std::stringstream ss;
    ss.precision(2);
    ss << std::fixed << std::setw(6) << std::setfill(' ') << p;
    return ss.str();
}

inline std::string s(int c){
    std::string str;
    str.insert(0, c,' ');
    return str;
}


inline void print(const std::string & text){
    if(verbose)
        std::cout <<s(pad)<< text << COLOR_RESET;
}

inline void printOver(const std::string & text){
    if(verbose)
        std::cout << "\r"<< s(pad) << text << COLOR_RESET << std::flush;
}

inline void printError(const std::string & text){
    std::cout << COLOR_RED << s(pad) << text << COLOR_RESET;
}

inline void printWarning(const std::string & text){
    std::cout << COLOR_YELLOW << s(pad) << text << COLOR_RESET;
}

inline void printBold(const std::string & text){
    if(verbose)
        std::cout << s(pad) << text;
//    std::cout << bold_on << text << bold_off;
}


inline float parseFloat(std::string txt){
    if(txt.size()<=0)
        return -1;
    return std::stof (txt);
}


inline int parseInt(std::string txt){
    if(txt.size()<=0)
        return 0;
    return std::stoi (txt);
}


inline std::vector<std::string> splitString(const std::string &s, char delim){
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

inline std::vector<float> splitFloatLine(const std::string & line){
    std::vector<std::string> stringTokens;
    std::vector<float> floatTokens;
    stringTokens = splitString(line,' ');
    for(auto s : stringTokens ){
        floatTokens.push_back(parseFloat(s));
    }
    return floatTokens;
}

inline std::vector<int> splitIntLine(const std::string & line){
    std::vector<std::string> stringTokens;
    std::vector<int> intTokens;
    stringTokens = splitString(line,' ');
    for(auto s : stringTokens ){
        intTokens.push_back(parseInt(s));
    }
    return intTokens;
}


inline void printMatrix(const glm::mat4 & m){
    for(int y=0;y<4;y++){
        for(int x=0;x<4; x++){
            std::cout<<m[x][y]<<" ";
        }
        std::cout<<"\n";
    }
}

inline void clampRGBA(glm::vec4 & color){
    if(color.r < 0)
        color.r = 0;
    
    if(color.g < 0)
        color.g = 0;
    
    if(color.b < 0)
        color.b = 0;
    
    if(color.a < 0)
        color.a = 0;
    
    if(color.r > 1)
        color.r = 1;
    
    if(color.g > 1)
        color.g = 1;
    
    if(color.b > 1)
        color.b = 1;
    
    if(color.a > 1)
        color.a = 1;
}

inline void adjPad(int t){
    pad += t;
}

#endif /* Utils_h */

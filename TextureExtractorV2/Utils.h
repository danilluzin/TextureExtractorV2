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


inline std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}


inline std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}


inline void print(const std::string & text){
    std::cout << text;
}


inline void printBold(const std::string & text){
//    std::cout << bold_on << text << bold_off;
    std::cout << text;
}


inline float parseFloat(std::string txt){
    if(txt.size()<=0)
        return -1;
    return std::stof (txt);
}


inline float parseInt(std::string txt){
    if(txt.size()<=0)
        return -1;
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


#endif /* Utils_h */

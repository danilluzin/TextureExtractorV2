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

/** is output verbose*/
extern bool verbose;

/** winite space pad size */
extern int pad;

/**
 *  Fits percent putput to a certain string length
 * @param p string size
 * @return foramated percent string
 */
inline std::string fitPercent(float p){
    std::stringstream ss;
    ss.precision(2);
    ss << std::fixed << std::setw(6) << std::setfill(' ') << p;
    return ss.str();
}

/**
 * Pads a string with ceratain number of whitespaces
 * @param c padd size
 * @return string
 */
inline std::string s(int c){
    std::string str;
    str.insert(0, c,' ');
    return str;
}


/**
 * Outputs the normal  text to console in formated way
 * @param text text to putput
 */
inline void print(const std::string & text){
    if(verbose)
        std::cout <<s(pad)<< text << COLOR_RESET;
}

/**
 * Write over the existing text on console
 * @param text text to putput
 */
inline void printOver(const std::string & text){
    if(verbose)
        std::cout << "\r"<< s(pad) << text << COLOR_RESET << std::flush;
}

/**
 * Outputs the error text to console in formated way
 * @param text text to putput
 */
inline void printError(const std::string & text){
    std::cout << COLOR_RED << s(pad) << text << COLOR_RESET;
}

/**
 * Outputs the warning text to console in formated way
 * @param text text to putput
 */
inline void printWarning(const std::string & text){
    std::cout << COLOR_YELLOW << s(pad) << text << COLOR_RESET;
}

/**
 * Outputs the bold text to console in formated way
 * @param text text to putput
 */
inline void printBold(const std::string & text){
    if(verbose)
        std::cout << s(pad) << text;
//        std::cout << bold_on << text << bold_off;
}


/**
 * Prses string containing float
 * @param txt input string with float number
 * @return parsed float
 */
inline float parseFloat(std::string txt){
    if(txt.size()<=0)
        return -1;
    return std::stof (txt);
}


/**
 * Prses string containing int
 * @param txt input string with int number
 * @return parsed float
 */
inline int parseInt(std::string txt){
    if(txt.size()<=0)
        return 0;
    return std::stoi (txt);
}


/**
 * Splits a string
 * @param delim a delimiter used for splitting a string
 * @return vector of tokens
 */
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

/**
 * Splits a string a parses floats
 * @param line input string
 * @return vector of floats
 */
inline std::vector<float> splitFloatLine(const std::string & line){
    std::vector<std::string> stringTokens;
    std::vector<float> floatTokens;
    stringTokens = splitString(line,' ');
    for(auto s : stringTokens ){
        floatTokens.push_back(parseFloat(s));
    }
    return floatTokens;
}

/**
 * Splits a string a parses int
 * @param line input string
 * @return vector of ints
 */
inline std::vector<int> splitIntLine(const std::string & line){
    std::vector<std::string> stringTokens;
    std::vector<int> intTokens;
    stringTokens = splitString(line,' ');
    for(auto s : stringTokens ){
        intTokens.push_back(parseInt(s));
    }
    return intTokens;
}

/**
 * DEBUG: prints out a matrix
 */
inline void printMatrix(const glm::mat4 & m){
    for(int y=0;y<4;y++){
        for(int x=0;x<4; x++){
            std::cout<<m[x][y]<<" ";
        }
        std::cout<<"\n";
    }
}

/**
 * Clamps RGB into a vlid range 1 - 0
 * @param color input color
 */
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

//
//  Timer.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 05/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Timer.hpp"
#include <sstream>
#include <math.h>

void Timer::start(){
    CPUstart = std::clock();
    WallStart = std::chrono::high_resolution_clock::now();
}


std::string Timer::stopGetResults(){
    auto CPUend = std::clock();
    auto WallEnd = std::chrono::high_resolution_clock::now();
    std::stringstream stringStream;
    auto milliC = 1000.0 * (CPUend-CPUstart) / CLOCKS_PER_SEC;
    auto secC = milliC / 1000;
    auto minutesC = secC / 60;
    
    auto milliW = std::chrono::duration<double, std::milli>(WallEnd-WallStart).count();
    auto secW = milliW / 1000;
    auto minutesW = secW / 60;
    
//    stringStream << std::fixed << std::setprecision(2) << "[ CPU: ";
    stringStream << std::fixed << "[ CPU: ";

    stringStream<< (int)minutesC<<"m:";
    stringStream<< (int)fmod(secC,60)<<"s:";
    stringStream<< (int)fmod(milliC,1000)<<" | ";
    stringStream<< "Wall:";
    stringStream<< (int)minutesW<<"m:";
    stringStream<< (int)fmod(secW,60)<<"s:";
    stringStream<< (int)fmod(milliW,1000)<<" ]\n";
    
    return stringStream.str();
}


std::string Timer::stopGetResults(const std::string & messagePrefix){
    return messagePrefix+stopGetResults();
}

//
//  Timer.cpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 05/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Timer.hpp"
#include <sstream>


void Timer::start(){
    CPUstart = std::clock();
    WallStart = std::chrono::high_resolution_clock::now();
}


std::string Timer::stopGetResults(){
    auto CPUend = std::clock();
    auto WallEnd = std::chrono::high_resolution_clock::now();
    std::stringstream stringStream;
    stringStream << std::fixed << std::setprecision(2) << "[ CPU: "
        << 1000.0 * (CPUend-CPUstart) / CLOCKS_PER_SEC << " ms | "
        << "Wall: "
        << std::chrono::duration<double, std::milli>(WallEnd-WallStart).count()
        << " ms ]\n";
    return stringStream.str();
}


std::string Timer::stopGetResults(const std::string & messagePrefix){
    return messagePrefix+stopGetResults();
}

//
//  Timer.hpp
//  TextureExtractorV2
//
//  Created by Danil Luzin on 05/11/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

class Timer{
public:
    void start();
    
    std::string stopGetResults();
    
    std::string stopGetResults(const std::string & messagePrefix);
    
private:
    std::clock_t CPUstart;
    
    std::chrono::time_point<std::chrono::high_resolution_clock> WallStart;
    
};

#endif /* Timer_hpp */

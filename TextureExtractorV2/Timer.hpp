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

/**
 * Timer class keeps track of Wall and CPU time
 */
class Timer{
public:
    /**
     * Starts a timer by saving current timestamp
     */
    void start();
    /**
     * Stops the timer and calculates elapsed time
     * @return Strign containing the timer output
     */
    std::string stopGetResults();
    
    /**
     * Stops timer and calculates elapsed time
     * @param messagePrefix string to add before the output
     * @return Strign containing the timer output with prefix before it
     */
    std::string stopGetResults(const std::string & messagePrefix);
    
private:
    /**
     * CPU time stat timestamp
     */
    std::clock_t CPUstart;
    /**
     * Wall time stat timestamp
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> WallStart;

    
};

#endif /* Timer_hpp */

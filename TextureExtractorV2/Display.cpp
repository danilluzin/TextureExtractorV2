//
//  Display.cpp
//  gltest
//
//  Created by Danil Luzin on 28/09/2017.
//  Copyright © 2017 Danil Luzin. All rights reserved.
//

#include "Display.hpp"


Display::Display( int width, int height, const std::string & title) :RenderContext(width,height){
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    isClosed = false;
}

Display::~Display(){
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void Display::update(){
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            isClosed = true;
        }
    }
}

void Display::putPixel(int x, int y,glm::vec4 color){
    if(x<0 || x>=width || y<0 || y>=height)
        return;
        
    SDL_SetRenderDrawColor(renderer, (color[0] * 255),
                                     (color[1] * 255),
                                     (color[2] * 255),
                                     (color[3] * 255));
    SDL_RenderDrawPoint(renderer, x, y);
}


void Display::clear(glm::vec4 color){
    SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
    SDL_RenderClear(renderer);
}

void Display::loadBitmap(const Bitmap & bitmap ){
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            putPixel(x, y, bitmap.at(x, y));
        }
    }
}

bool Display::isColsed(){
    return isClosed;
}




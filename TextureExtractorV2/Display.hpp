#ifndef Display_hpp
#define Display_hpp


#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Bitmap.hpp"
#include "RenderContext.h"
 
class Display : public RenderContext {
    
    SDL_Renderer * renderer;
    
    SDL_Window * window;
    
    bool isClosed;
    
    
public:
    Display( int width, int height, const std::string & title);
    
   ~Display();
    
    void update();
    
    bool isColsed();
    
    void clear(glm::vec4 color);
    
    void putPixel(int x, int y, glm::vec4 color);
        
    void loadBitmap(const Bitmap & bitmap );
    
    SDL_Renderer * getRenderer(){return renderer;}
    
private:
 
};

#endif /* Display_hpp */

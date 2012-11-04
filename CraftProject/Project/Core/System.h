#ifndef SYSTEM_H
#define SYSTEM_H

//#include <windows.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Graphics.h"
#include "Resources.h"
#include "Input.h"
#include "Sound.h"

#define FLIP_BUFFER SDL_GL_SwapBuffers()

#ifdef DOUBLE_PRES
    #define glTranslate glVertex
#endif

#ifdef FLOAT_PRES
    #define glTranslate glVertex
#endif

class Graphics;
class Input;

class System
{
private:
    bool fullScreen;
    int width, height, bpp;
    char title[128];
    SDL_Surface* backBuffer;
    bool running;

    Graphics *graphics;
    Input input;
    ResourceManager mgr;
public:
    System(){};
    ~System(){};

    bool init(char* aTitle, int aWidth, int aHeight, bool aFullScreen = false, int aBpp = 32);
    void kill();
    void update();
    void setFullScreen(bool full);

    void delay(int ticks);
    long getTicks();

    bool isRunning(){return running;};
    bool isFullScreen(){return fullScreen;};

    Graphics* getGraphics(){return graphics;};
    Input* getInput(){return &input;};

    unsigned char* loadImageData(char fileName[],int* aWidth = NULL, int* aHeight = NULL, int* dim = NULL);
    ResourceManager* getResMgr(){return &mgr;};

    int getClientWidth(){return width;};
    int getClientHeight(){return height;};
};

#endif

#include "System.h"
#include <SDL/SDL.h>
#include <string>

bool System::init(char* aTitle, int aWidth, int aHeight, bool aFullScreen, int aBpp)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return 0;

    width = aWidth;
    height = aHeight;
    bpp = aBpp;
    strcpy(title, aTitle);

    fullScreen = aFullScreen;

    if(!fullScreen)
        backBuffer = SDL_SetVideoMode(width,height,bpp, SDL_OPENGL);
    else
        backBuffer = SDL_SetVideoMode(width,height,bpp, SDL_OPENGL | SDL_FULLSCREEN);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);


    SDL_WM_SetCaption(title,NULL);
    SDL_Flip(backBuffer);
    running = true;

    graphics = new Graphics();
    graphics->init(width,height);

    input.init();
    Sound::init();

    return true;
}

void System::kill()
{
    mgr.freeAll();

    Sound::kill();
    delete graphics;

    input.free();

    SDL_Quit();
}

void System::update()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }

    input.update();
}

void System::setFullScreen(bool full)
{
    fullScreen = full;

    if(fullScreen)
        backBuffer = SDL_SetVideoMode(width,height,bpp, SDL_OPENGL);
    else
        backBuffer = SDL_SetVideoMode(width,height,bpp, SDL_OPENGL | SDL_FULLSCREEN);
}

void System::delay(int ticks)
{
    SDL_Delay(ticks);
}

long System::getTicks()
{
    return SDL_GetTicks();
}

unsigned char* System::loadImageData(char* fileName, int* aWidth, int* aHeight, int* dim)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* image = NULL;
    loadedImage = SDL_LoadBMP(fileName);

    if(loadedImage != NULL)
    {
        image = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

        if(image != NULL)
        {
            Uint32 colorKey = SDL_MapRGB(image->format, 0xFF,0,0xFF);
            SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorKey);
        }
        else
            return NULL;
    }
    else
        return NULL;

    if(aWidth != NULL)
        (*aWidth) = image->w;

    if(aHeight != NULL)
        (*aHeight) = image->h;

    int width = image->w;
    int height = image->h;

    int texSize = 2;

    while(width > texSize || height > texSize)
    {
       texSize = texSize*2;
    }

    if(dim != NULL)
        (*dim) = texSize;

    unsigned char* data;

    data = new unsigned char[texSize*texSize*4];

    for(int i = 0; i < texSize*texSize*4; i++)
    {
        data[i] = 0;
    }

    if(SDL_MUSTLOCK(image))
       SDL_LockSurface(image);

    unsigned char* imgData; //= new unsigned char[width*height*4];
    imgData = (unsigned char*) image->pixels;

    for(int y = 0; y <height; y++)
        for(int x = 0; x <width; x++)
        {
            int i = (x+y*width)*4;
            int o = (x+y*texSize)*4;
            data[o] = imgData[i];
            data[1+o] = imgData[i+1];
            data[2+o] = imgData[i+2];
            data[3+o] = imgData[i+3];
        }

    if(SDL_MUSTLOCK(image))
       SDL_UnlockSurface(image);

    for(int y = 0; y <texSize; y++)
    for(int x = 0; x <texSize; x++)
    {
        int i = x+y*texSize;
        int byte1 = i*4;
        int byte2 = byte1+1;
        int byte3 = byte2+1;
        int byte4 = byte3+1;

        char swap = data[byte1];
        data[byte1] = data[byte3];
        data[byte3] = swap;

        if(data[byte1] == 255 && data[byte2] == 0 && data[byte3] == 255)
            data[byte4] = 0;
        else
            data[byte4] = 255;

    }
    /*
    glGenTextures(1,&imageTag);
    glBindTexture(GL_TEXTURE_2D, imageTag);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST_MIPMAP_NEAREST);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);//image->pixels);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texSize, texSize, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSize,texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    */
    SDL_FreeSurface(image);

    return data;
}

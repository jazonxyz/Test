#include "Input.h"

Input::Input()
{
    keys = NULL;
    prevKeys = NULL;

    for(int i = 0; i < 3; i++)
    {
        mouse[i] = 0;
        prevMouse[i];
    }

    mouseX = mouseY = 0;
    numKeys = 0;
}

void Input::init()
{
    SDL_GetKeyState(&numKeys);
    keys = new bool[numKeys];
    prevKeys = new bool[numKeys];
}

void Input::free()
{
    delete keys;
    delete prevKeys;
}

void Input::showMouse(bool show)
{
    if(show)
        SDL_ShowCursor(SDL_ENABLE);
    else
        SDL_ShowCursor(SDL_DISABLE);

}

bool Input::mouseVisible()
{
    if(SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
        return true;

    return false;
}

void Input::update()
{
    SDL_PumpEvents();//Handle Input Here

    for(int i = 0; i < numKeys; i++)
        prevKeys[i] = keys[i];

    for(int i = 0; i < 3; i++)
        prevMouse[i] = mouse[i];

    Uint8* keyboard = SDL_GetKeyState(NULL);


    for(int i = 0; i < numKeys; i++)
        keys[i] = keyboard[i];

    mouse[0] = (SDL_GetMouseState(&mouseX, &mouseY)&SDL_BUTTON(1));
    mouse[1] = (SDL_GetMouseState(&mouseX, &mouseY)&SDL_BUTTON(2));
    mouse[2] = (SDL_GetMouseState(&mouseX, &mouseY)&SDL_BUTTON(3));
}

bool Input::keyDown(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return keys[key];
}

bool Input::keyHit(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return (!prevKeys[key] && keys[key]);
}

bool Input::keyUp(int key)
{
    if(key < 0 || key > numKeys)
        return false;

    return (prevKeys[key] && !keys[key]);
}

bool Input::mouseDown(int key)
{
    if(key < 0 || key > 2)
        return false;

    return mouse[key];
}

bool Input::mouseUp(int key)
{
    if(key < 0 || key > 2)
        return false;

    return (!mouse[key] && prevMouse[key]);
}

bool Input::mouseHit(int key)
{
    if(key < 0 || key > 2)
        return false;

    return (!prevMouse[key] && mouse[key]);
}

int Input::getMouseX()
{
    return mouseX;
}

int Input::getMouseY()
{
    return mouseY;
}

void Input::setMouse(int x, int y)
{
    SDL_WarpMouse((Uint16)x,(Uint16)y);
}


#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>


enum{MOUSE_LEFT = 0, MOUSE_MIDDLE, MOUSE_RIGHT};

class Input
{
private:
    bool *keys;
    bool *prevKeys;
    bool mouse[3];
    bool prevMouse[3];
    int mouseX, mouseY;
    int numKeys;
public:
    Input();
    ~Input(){};
    void init();
    void free();
    void showMouse(bool show);
    bool mouseVisible();
    void update();
    bool keyDown(int key);
    bool keyHit(int key);
    bool keyUp(int key);
    bool mouseDown(int mouse);
    bool mouseUp(int mouse);
    bool mouseHit(int mouse);
    int getMouseX();
    int getMouseY();
    void setMouse(int x, int y);
};

#endif

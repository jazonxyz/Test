#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include "System.h"
#include "Resources.h"
#include "Math.h"

#ifdef DOUBLE_PRES
    #define glMaterialv glMaterialdv
    #define glLightv   glLightdv
    #define GL_SCALAR   GL_DOUBLE
#endif

class System;
class Graphics;
class Image;
class Color;

class Camera : public Rect //add stuff to zoom in
{
private:
    //Vector pos;
    //Scalar rot;
    Scalar zoomIn;
    void translateCam();
public:
    Camera(){setPos(0,0);setSize(800,600);zoomIn = 1.0;};
    ~Camera(){};
    void moveX(Scalar d);
    void moveY(Scalar d);
    void rotate(Scalar a);
    void setZoom(Scalar z);
    Scalar getZoom(){return zoomIn;};
    void zoom(Scalar z);

    Scalar getTotalX(){return getX()/zoomIn;};
    Scalar getTotalY(){return getY()/zoomIn;};
    Scalar getTotalX2(){return getTotalX() + getTotalWidth() - 1;};
    Scalar getTotalY2(){return getTotalY() + getTotalHeight() - 1;};

    void setTotalX(Scalar x){setX(x*zoomIn);};
    void setTotalY(Scalar y){setY(y*zoomIn);};
    void setTotalX2(Scalar x){setTotalX(x-getTotalWidth()+1);};
    void setTotalY2(Scalar y){setTotalY(y-getTotalHeight()+1);};

    Scalar getTotalWidth(){return getWidth()/zoomIn;};
    Scalar getTotalHeight(){return getHeight()/zoomIn;};

    void centerAt(Scalar x, Scalar y);
    bool overlapsAll(Rect r){return r.overlaps(Rect(getTotalX(),getTotalY(),getTotalWidth(),getTotalHeight()));};
    int getCamY(){return getY();};
};

class Graphics
{
private:
    int width, height;
    //Scalar Width2D;
    //Scalar Height2D;
    Camera cam;
    bool isCamEnabled;
public:
    void init(int aWidth, int aHeight);
    void resize(int aWidth, int aHeight);
    void debugDraw();
    void clear(Scalar r, Scalar g, Scalar b, Scalar a);
    void Perspective(Scalar fov, Scalar anear, Scalar afar);
    void flip();
    void drawVec(Vector V);
    void drawPoint(int x, int y, Color c);                          //Optimize for many primitives
    void drawLine(Scalar x, Scalar y, Scalar x2, Scalar y2, Color c);           //This too
    void drawRect(Rect r, Color c);                                 //And this
    void drawRect(int x, int y, int width, int height, Color c);
    void fillRect(Rect r, Color c);                                 //Yep, this too
    void fillRect(int x, int y, int width, int height, Color c);
    void fillRect(Rect r, Color c1, Color c2, Color c3, Color c4);  //And this haha
    void enableCam(bool c = true);
    bool camEnabled(){return isCamEnabled;};
    Scalar getWidth2D(){return width;};
    Scalar getHeight2D(){return height;};
    Camera* getCamera(){return &cam;};
};

class Color
{
private:
    Scalar r,g,b,a;
public:
    Color(){a=g=b=a=1;};
    Color(Scalar ar,Scalar ag,Scalar ab,Scalar aa = 255){r = ar; g = ag; b = ab; a = aa;};
    //Color(int ar, int ag, int ab, int aa = 255){r = 1/((double)ar); g = 1/((double)ag);b = 1/((double)ab);a = 1/((double)aa);};
    ~Color(){};
    void Set(Scalar ar,Scalar ag,Scalar ab,Scalar aa = 255){r = ar; g = ag; b = ab; a = aa;};
    void SetR(Scalar n){r = n;};
    void SetG(Scalar n){g = n;};
    void SetB(Scalar n){b = n;};
    void SetA(Scalar n){a = n;};
    Scalar GetR(){return r;};
    Scalar GetG(){return g;};
    Scalar GetB(){return b;};
    Scalar GetA(){return a;};
};

class Image : public Resource                //add simple Draw(x,y,scalex,scaley,rot,color c)
{
private:
    int width;
    int height;
    int texSize;
    int frameWidth;
    int frameHeight;
    System* system;

    //int imageTag;
    char imageName[512];
    GLuint imageTag;
public:
    Image();
    Image(char Name[], char FileName[], System* sys);
    Image(char Name[], char FileName[], System* sys, int aFrameWidth, int aFrameHeight);
    ~Image(){};
    void load(char fileName[], System* sys);
    void load(char fileName[], System* sys, int aFrameWidth, int aFrameHeight);

    void setFrameSize(int aFrameWidth,int aFrameHeight);
    /*void draw(int x, int y, Color c = Color());
    void draw(int x, int y, int x1, int y1, int w, int h, Color c = Color());
    void draw(int x, int y, int frame, Color c = Color());*/
    Vector tex(Scalar u, Scalar v);
    void draw(int x, int y, Color c = Color(1,1,1));//Add arg Cam c at the end
    void draw(int x, int y, Scalar ScaleX =1 , Scalar ScaleY =1 , Scalar Rot = 0, Color c = Color (1,1,1));//add arg cam c
    void draw(int x, int y, int x1, int y1, int w, int h, Scalar ScaleX, Scalar ScaleY, Scalar Rot, Color c);//Rect for x1,y1,w,h
    void draw(int x, int y, int frame, Scalar ScaleX, Scalar ScaleY, Scalar Rot, Color c);//Add cam c

    int getWidth();
    int getHeight();
    int getFrameWidth();
    int getFrameHeight();

    void free();
};

class Font : public Resource
{
private:
    Image image;
    int charWidth;
    int charHeight;
    System* sys;
public:
    Font();
    Font(char Name[], char FileName[], System* sys);
    ~Font(){};
    void load(char fileName[], System* system);
    //void draw(char message[], int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    //void draw(char message[],Rectangle rect, int r = 255, int g = 255, int b = 255, int a = 255);
    //void drawCentered(char message[], Rectangle rect, int r = 255, int g = 255, int b = 255, int a = 255);
    void draw(char message[],int x, int y, Scalar ScaleX, Scalar ScaleY ,Scalar rot, Color c);
    void draw(char message[],int x, int y, Color c);
    void free();
    int getCharWidth(){return charWidth;};
    int getCharHeight(){return charHeight;};
};

class Map : public Resource
{
private:
    Image image;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<int> layer1;
    std::vector<int> layer2;
    std::vector<int> solidLayer;
    System* system;
public:
    Map(){width = height = tileWidth = tileHeight = 0;};
    Map(char name[], char fileName[], System *sys);
    ~Map(){};

    void create(char imageName[], System* sys, int aWidth, int aHeight, int aTileWidth, int aTileHeight);
    bool load(char fileName[], System* sys);
    void free();

    void setTile1(int tile, int x, int y);
    void setTile2(int tile, int x, int y);
    void setSolid(int tile, int x, int y);

    void draw1(int x, int y);
    void draw2(int x, int y);

    int getTile1(int x, int y);
    int getTile2(int x, int y);
    int getSolid(int x, int y);
    Image* getImage(){return &image;};

    int getWidth();
    int getHeight();
    int getTileWidth();
    int getTileHeight();
    int getTotalWidth(){return width*tileWidth;};
    int getTotalHeight(){return height*tileHeight;};

    bool rectCollides(int code, Rect r);
    bool rectOverlaps(int code, Rect r);
};

#endif

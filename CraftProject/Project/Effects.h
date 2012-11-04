#ifndef EFFECTS_H
#define EFFECTS_H

#include "Core/World.h"

class Effect : public WorldObject
{
private:
    int life;
public:
    Effect(int x, int y, int life);
    ~Effect(){};
    void update();
    void render();
};

class SparkEffect : public Effect
{
private:
    Image* image;
public:
    SparkEffect(Image* i, int x, int y);
    ~SparkEffect(){};
    void render();
};

class BloodEffect : public Effect
{
private:
    int frame;
    Image* image;
    int xVel, yVel;
public:
    BloodEffect(Image* i, int x, int y);
    ~BloodEffect(){};
    void update();
    void render();
};

#endif

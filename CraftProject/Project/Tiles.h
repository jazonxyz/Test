#ifndef TILES_H
#define TILES_H

#include "Core/World.h"
#include "Entity.h"
#include "EntityFactory.h"

class EntityFactory;

enum{TILE_GRASS = 1, TILE_WATER, TILE_SAND, TILE_WOOD};

class Spawn : public WorldObject
{
private:
    static int id;
public:
    Spawn(int x, int y);
    ~Spawn(){};
    void update();
    void render();

    static int getID(){return id;};
};

class Tree : public Entity
{
private:
    Image* image;
    EntityFactory* factory;
public:
    Tree(Image* i, EntityFactory* f, int x, int y);
    ~Tree(){};
    void destroy();
    void update();
    void render();
};

class Rock : public Entity
{
private:
    Image* image;
    EntityFactory* factory;
    bool hasMultiplied;
    int frame;
public:
    Rock(Image* i, EntityFactory* f, int x, int y);
    ~Rock(){};
    void update();
    void render();
    void setFrame(int f){frame = f;};
};

#endif

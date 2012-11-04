#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "Core/System.h"
#include "Core/World.h"
#include "Core/Resources.h"
#include "Tiles.h"
#include "EntityFactory.h"

class EntityFactory;

class LevelGen
{
private:
    std::vector<std::vector<int> > objects;
public:
    World* generate(int width, int height, EntityFactory *factory, System* sys);
    void generateWater(World* world);
    void generateTrees(World* world);
    void generateRocks(World* world);
    void generateSpawns(World* world);
    void placeObjects(World* world, EntityFactory* factory);
    int calcFrame(int x, int y, int key);
};

#endif

#ifndef LEVEL_H
#define LEVEL_H

#include "Core/World.h"
#include "Core/Resources.h"
#include "LevelGen.h"
#include "Core/Sound.h"
#include "Player.h"
#include "Creatures.h"
#include "Tiles.h"
#include "EntityFactory.h"

class LevelSorter : public RenderSorter
{
private:
public:
    LevelSorter(){};
    ~LevelSorter(){};
    std::vector<WorldObject*> sort(std::vector<WorldObject*> items);
};

class Level
{
private:
    World *world;
    std::vector<Spawn*> spawnList;
    EntityFactory factory;
    ItemFactory itemFactory;
    LevelSorter sorter;
    Music music;
    Input* in;
public:
    void init(System* sys, Player* player);
    void update();
    void render();
    void free();

    void generateSpawnList();

    Spawn* getRandomSpawn();

    World* getWorld(){return world;};
};

#endif

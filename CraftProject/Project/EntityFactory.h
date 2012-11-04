#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "Core/Resources.h"
#include "Entity.h"
#include "Tiles.h"
#include "Creatures.h"
#include "Player.h"
#include "Effects.h"
#include "Item.h"

class PlayerObject;

class EntityFactory
{
private:
    ResourceManager resMgr;
    ItemFactory* itemFactory;
public:
    void init(System* sys, ItemFactory* f);
    void free();

    PlayerObject* makePlayer(Input* in, Inventory* inv, int x, int y);

    Entity* makePig(int x, int y);
    Entity* makeRock(int x, int y);
    Entity* makeTree(int x, int y);

    Effect* makeSpark(int x, int y);
    Effect* makeBlood(int x, int y);

    SolidItem* makeItem(int id, int x, int y);
};


#endif

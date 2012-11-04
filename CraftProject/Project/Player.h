#ifndef PLAYER_H
#define PLAYER_H

#include "Core/Graphics.h"
#include "Core/World.h"
#include "LevelGen.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Item.h"

class EntityFactory;

class PlayerObject : public Entity
{
private:
    static int id;
    Image* image;
    Input* input;
    Inventory* inventory;
    EntityFactory* factory;
    int range;
public:
    PlayerObject(Image* i, Input* in, EntityFactory* f, Inventory* inv, int x, int y);
    void update();
    void render();
    Rect getFrontRect(int size);

    int getID(){return id;};
    EntityFactory* getFactory(){return factory;};
    Inventory* getInventory(){return inventory;};
};

class Player
{
private:
    Inventory inventory;
    PlayerObject* object;
public:
    Player();
    ~Player(){};
    void setObject(PlayerObject* o){object = o;};
    Inventory* getInventory(){return &inventory;};
    PlayerObject* getObject(){return object;};
};

#endif

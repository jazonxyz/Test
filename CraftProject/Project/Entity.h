#ifndef ENTITY_H
#define ENTITY_H

#include "Core/World.h"


enum{DIR_DOWN = 0, DIR_UP, DIR_RIGHT, DIR_LEFT};

enum{ENTITY_ITEM, ENTITY_CREATURE, ENTITY_OBJECT, ENTITY_PLAYER};

class Entity : public WorldObject
{
private:
    static int id;

    int health;
    int defense;
    int speed;
    int type;
protected:
    int direction;
    int frame;
public:
    Entity(int x, int y, int t);
    ~Entity(){};
    virtual void hurt(int damage);
    virtual void interact();
    virtual void destroy();

    int getSpeed(){return speed;};
    void setSpeed(int s){speed = s;};

    void setHealth(int h){health = h; if(health > 100) health = 100;};
    int getHealth(){return health;};

    int getDefense(){return defense;};
    void setDefense(int d){defense = d;};

    static int getID(){return id;};
    int getType(){return type;};
};

#endif

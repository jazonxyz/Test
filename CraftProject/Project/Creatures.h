#ifndef CREATURES_H
#define CREATURES_H

#include "Entity.h"
#include "EntityFactory.h"

class EntityFactory;

class Pig : public Entity
{
private:
    static int id;
    int frame;
    int direction;
    int action;
    int actionCounter;
    Image* image;
    EntityFactory* factory;
public:
    Pig(Image* i, EntityFactory* f, int x, int y);
    ~Pig(){};

    void hurt(int damage);
    void update();
    void render();

    static int getID(){return id;};
};

#endif

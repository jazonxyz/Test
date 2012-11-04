#include "Tiles.h"

int Spawn::id = WorldObject::generateType();

Spawn::Spawn(int x, int y) : WorldObject(x,y,id)
{
    setSolid(false);
    setSize(25,25);
}

void Spawn::update(){};
void Spawn::render(){};

Tree::Tree(Image* i, EntityFactory *f, int x, int y) : Entity(x, y, ENTITY_CREATURE)
{
    factory = f;
    setDefense(0);
    setSpeed(0);
    setSize(25,25);
    setSolid(true);
    image = i;
    activate(false);
    frame = 0;
}

void Tree::destroy()
{
    getWorld()->addObject(factory->makeItem(ITEM_WOOD, getX(), getY()));
}

void Tree::update()
{

}

void Tree::render()
{
    image->draw(getX(), getY(), frame , 1, 1, 0, Color(255,255,255));
}

Rock::Rock(Image* i, EntityFactory* f, int x, int y) : Entity(x, y, ENTITY_CREATURE)
{
    setSize(25,25);
    image = i;
    hasMultiplied = false;
    setSolid(true);
    setDefense(5);
    setSpeed(0);
    factory = f;
    activate(false);
    frame = 0;
}

void Rock::update()
{

}

void Rock::render()
{
    image->draw(getX(), getY(), frame, 1, 1, 0, Color(255,255,255));
}

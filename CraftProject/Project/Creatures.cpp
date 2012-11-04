#include "Creatures.h"

int Pig::id = WorldObject::generateType();

Pig::Pig(Image* i, EntityFactory* f, int x, int y) : Entity(x, y, ENTITY_CREATURE)
{
    setSize(25,25);
    setSolid(true);
    direction = DIR_DOWN;
    frame = 0;
    action = 0;
    actionCounter = rand()%250;
    image = i;
    setSpeed(3);
    factory = f;
}

void Pig::hurt(int damage)
{
    Entity::hurt(damage);

    for(int i = 0; i < 2; i++)
        getWorld()->addObject(factory->makeBlood(getCenterX(), getCenterY()));
}

void Pig::update()
{
    int speed = getSpeed();
    if(action == 1)
    {
        int oldX = getX();
        int oldY = getY();

        if(direction == DIR_UP)
            move(0,-speed);
        if(direction == DIR_DOWN)
            move(0,speed);
        if(direction == DIR_LEFT)
            move(-speed,0);
        if(direction == DIR_RIGHT)
            move(speed, 0);

        if(oldX == getX() && oldY == getY())
            direction = rand()%4;
    }
    else
    {
        frame = 0;
    }

    actionCounter--;

    if(actionCounter <= 0)
    {
        action = rand()%2;
        actionCounter = rand()%250;
        direction = rand()%4;
    }
}

void Pig::render()
{
    int frameRow = direction * 2;
    image->draw(getX(), getY(), frameRow + (frame/6), 1, 1, 0, Color(255,255,255));
    frame++;

    if(frame >= 12)
        frame = 0;
}





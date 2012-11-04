#include "Entity.h"

int Entity::id = WorldObject::generateType();

Entity::Entity(int x, int y, int t) : WorldObject(x,y,id)
{
    type = t;
    health = 100;
    defense = 0;
    speed = 0;
}

void Entity::hurt(int damage)
{
    int totalDamage = damage-defense;

    if(totalDamage <= 0)
        health--;
    else
        health-= totalDamage;

    if(health < 0)
    {
        destroy();
        remove();
    }
}

void Entity::interact()
{

}

void Entity::destroy()
{

}


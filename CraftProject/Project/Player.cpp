#include "Player.h"

int PlayerObject::id = WorldObject::generateType();

Player::Player()
{
    object = NULL;
}

PlayerObject::PlayerObject(Image* i, Input* in, EntityFactory* f, Inventory* inv, int x, int y) : Entity(x,y, ENTITY_PLAYER)
{
    setSize(25,25);
    setSolid(true);
    image = i;
    input = in;
    range = 10;
    direction = DIR_DOWN;
    frame = 0;
    setSpeed(3);
    setDefense(5);
    factory = f;
    inventory = inv;
}

void PlayerObject::update()
{
    int movex = 0;
    int movey = 0;

    int speed = getSpeed();

    int oldDirection = direction;

    if(input->keyDown(SDLK_UP))
    {
        movey -= speed;
        direction = DIR_UP;
    }

    if(input->keyDown(SDLK_DOWN))
    {
        movey += speed;
        direction = DIR_DOWN;
    }

    if(input->keyDown(SDLK_RIGHT))
    {
        movex += speed;
        direction = DIR_RIGHT;
    }

    if(input->keyDown(SDLK_LEFT))
    {
        movex -= speed;
        direction = DIR_LEFT;
    }

    bool animate = false;

    if(input->keyDown(SDLK_SPACE))
    {
        direction = oldDirection;
        Rect frontRect = getFrontRect(range);

        std::vector<WorldObject*> targets = getWorld()->getOverlapping(frontRect, this);

        for(int i = 0; i < (int)targets.size(); i++)
        {
            if(targets[i]->getType() == Entity::getID())
            {
                Entity* target = ((Entity*)targets[i]);

                if(target->getType() == ENTITY_CREATURE)
                {
                    target->hurt(5);
                    getWorld()->addObject(factory->makeSpark(getFrontRect(25).getCenterX(), getFrontRect(25).getCenterY()));
                }

                if(target->getType() == ENTITY_ITEM)
                {
                    if(!inventory->isFull())
                    {
                        inventory->addItem(((Item*)target)->getID());
                        target->remove();
                    }
                }

                animate = true;

            }
        }
    }

    if(movex == 0 && movey == 0 && !animate)
        frame = 0;


    if(input->keyHit(SDLK_p))
        for(int i = 0; i < 100; i++)
            getWorld()->addObject(factory->makePig(getFrontRect(25).getX(), getFrontRect(25).getY()));

    if(input->keyDown(SDLK_r))
    {
        if(getWorld()->getOverlapping(getFrontRect(25),this).empty())
            getWorld()->addObject(factory->makeRock(((int)getFrontRect(25).getCenterX()/25)*25, ((int)getFrontRect(25).getCenterY()/25)*25));
    }

    move(movex, movey);
}

Rect PlayerObject::getFrontRect(int size)
{
    switch(direction)
    {
    case DIR_UP:
        return getRectUp(size);
    case DIR_DOWN:
        return getRectDown(size);
    case DIR_LEFT:
        return getRectLeft(size);
    case DIR_RIGHT:
        return getRectRight(size);
    default:
        return getRectDown(size);
    }
}

void PlayerObject::render()
{
    int frameRow = direction * 4;
    image->draw(getX(), getY(), frameRow + (frame/8), 1, 1, 0, Color(255,255,255));
    frame++;

    if(frame >= 32)
        frame = 0;
}


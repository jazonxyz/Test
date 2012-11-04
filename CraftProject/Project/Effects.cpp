#include "Effects.h"

Effect::Effect(int x, int y, int life) : WorldObject(x, y)
{
    this->life = life;
    setSolid(false);
}

void Effect::update()
{
    life--;
    if(life < 0)
        remove();
}

void Effect::render()
{

}

SparkEffect::SparkEffect(Image* i, int x, int y) : Effect(x, y, 10)
{
    setSize(25,25);

    setCenterX(x);
    setCenterY(y);

    image = i;

    int xOffset = -5 + rand()%10;
    int yOffset = -5 + rand()%10;

    setPos(getX()+xOffset, getY()+yOffset);
}

void SparkEffect::render()
{
    image->draw(getX(), getY(), Color(255,255,255));
}

BloodEffect::BloodEffect(Image* i, int x, int y) : Effect(x,y,10)
{
    setSize(4,4);
    setCenterX(x);
    setCenterY(y);
    image = i;
    xVel = -3+rand()%6;
    yVel = -3+rand()%6;

    if(xVel == 0)
        xVel = 1;

    if(yVel == 0)
        yVel = 1;
}

void BloodEffect::update()
{
    Effect::update();
    move(xVel,yVel);
}

void BloodEffect::render()
{
    image->draw(getX(), getY(), rand()%4, 1, 1, 0, Color(255,255,255));
}

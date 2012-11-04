#include "Math.h"
#include <Math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool Math::randInit = false;

int Math::randInt(int x, int y)
{
    if(!randInit)
    {
        srand((unsigned)time(NULL));
        randInit = true;
    }

    if(x > y)
    {
        int temp = x;
        x = y;
        y = temp;
    }

    int diff = y-x;

    /*if(diff < 0)
        diff*=-1;*/
    if(diff == 0)
        return x;

    return x+(rand()%(diff+1));
}

Scalar Math::randScalar()
{
    if(!randInit)
    {
        srand((unsigned)time(NULL));
        randInit = true;
    }
    return rand()/(Scalar(RAND_MAX)+1);
}

void Vector::multQ(Scalar x1, Scalar y1, Scalar z1, Scalar w1, Scalar x2, Scalar y2, Scalar z2, Scalar w2, Scalar* rx, Scalar* ry, Scalar* rz, Scalar* rw)
{
    if(!rx || !ry || !rz || !rw)
        return;

    (*rx) = w1*x2 + x1*w2 + y1*z2 - z1*y2;
    (*ry) = w1*y2 - x1*z2 + y1*w2 + z1*x2;
    (*rz) = w1*z2 + x1*y2 - y1*x2 + z1*w2;
    (*rw) = w1*w2 - x1*x2 - y1*y2 - z1*z2;
}

Scalar Vector::length()
{
    return sqrt(x*x + y*y);
}

void Vector::setLength(Scalar l)
{
    normalize();
    x*=l;
    y*=l;
}

void Vector::normalize()
{
    Scalar len = length();
    if(len == 0.0f)
        return;

    x/= len;
    y/= len;
}

void Vector::negate()
{
    x = -x;
    y = -y;
}

Vector Vector::operator+(Vector v)
{
    return Vector(x+v.getX(), y+v.getY());
}

Vector Vector::operator-(Vector v)
{
    return Vector(x-v.getX(), y-v.getY());
}

Vector Vector::operator*(Scalar d)
{
    return Vector(x*d,y*d);
}

Vector Vector::operator/(Scalar d)
{
    if(d == 0.0f)
        return Vector(0,0);
    return Vector(x/d,y/d);
}

void Vector::operator=(Vector v)
{
    //Set(v.GetX(),v.GetY(),v.GetZ());
    x = v.getX();
    y = v.getY();
}

Scalar Vector::dot(Vector v)
{
    return (x*v.getX()+ y*v.getY());
}

/*Vector Vector::cross(Vector v)
{
    return Vector(y*v.getZ()-z*v.getY(), z*v.getX()-x*v.getZ(), x*v.getY()-y*v.getX());
}*/

Scalar Vector::angle(Vector v)
{
    Vector v1(x,y);
    v1.normalize();
    v.normalize();

    if(v1.length() == 0 || v.length() == 0)
        return 0;

    Scalar ret = acosf(v1.dot(v))*180/PI;

    return ret;
}

Scalar Vector::angle()
{
    if(x == 0)
    {
        if(y > 0)
            return 270;

        if(y < 0)
            return 90;

        return 0;
    }

    Scalar ret = atanf(y/x)*180/PI;

    if(x < 0)
        ret+=180;

    return -ret;
}

/*void Vector::rotate(Scalar rx, Scalar ry, Scalar rz)
{
    rotX(rx);
    rotY(ry);
    rotZ(rz);
}*/

void Vector::setDir(Scalar dx, Scalar dy, Scalar dz)
{
    x = cos(dx*PI/180);
    y = sin(dx*PI/180);//Finish later
    z = -cos(dx*PI/180);
}

void Vector::rotate(Scalar angle)
{
    Scalar newX;
    Scalar newY;

    angle*=-PI/180;

    newX = x*cos(angle)-y*sin(angle);
    newY = x*sin(angle)+y*cos(angle);

    x = newX;
    y = newY;
}

//////////////////////////////////////////////////////////////////////////////
//Rect////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

bool Rect::overlaps(Rect r)
{
    return (r.getX2() >= getX() && r.getX() <= getX2() && r.getY2() >= getY() && r.getY() <= getY2());
}

bool Rect::collides(Rect r)
{
    return (r.getX2() >= getX()-1 && r.getX() <= getX2()+1 && r.getY2() >= getY()-1 && r.getY() <= getY2()+1);
}

bool Rect::contains(Rect r)
{
    return (r.getX() > getX() && r.getY() > getY() && r.getX2() < getX2() && r.getY2() < getY2());
}

bool Rect::contains(Scalar x, Scalar y)
{
    return (x>= getX() && x <= getX2() && y >= getY() && y <= getY2());
}

bool Rect::lineOverlaps(Scalar x1, Scalar y1, Scalar x2, Scalar y2)
{
    /*if(x1 > x2)
    {
        int swap = x1;
        x1 = x2;
        x2 = swap;
    }

    if(y1 > y2)
    {
        int swap = y1;
        y1 = y2;
        y2 = swap;
    }*/

    Scalar rx = x1;
    Scalar width = x2-x1+1;
    if(width < 0)
    {
        rx = x2;
        width = x1-x2+1;
    }

    Scalar ry = y1;
    Scalar height = y2-y1+1;
    if(height < 0)
    {
        ry = y2;
        height = y1-y2+1;
    }

    if(!overlaps(Rect(rx,ry,width,height)))
        return false;

    if(x2-x1 == 0)
        return true;

    Scalar slope = (y2-y1)/(x2-x1);
    Scalar b = y1-slope*x1;

    if(slope == 0)
        return true;

    //if(x2<x1)
    //{
      //  x1=x2;
        //y1=y2;
    //}

    //Check top line by finding the XValue of the line corresponding to the line in the top
    Scalar result = (getY()-b)/slope;
    if(result >= getX() && result <= getX2())
        return true;

    result = (getY2()-b)/slope;
    if(result >= getX() && result <= getX2())
        return true;

    result = (slope*getX()+b);
    if(result >= getY() && result <= getY2())
        return true;

    result = (slope*getX2()+b);
    if(result >= getY() && result <= getY2())
        return true;

    //if(contains(Rect(x1,y1,x2-x1+1,y2-y1+1)))
      //  return true;

    return false;
}

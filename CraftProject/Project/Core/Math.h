#ifndef MATH_H
#define MATH_H

#include <Math.h>

#define PI 3.14159265

#define DOUBLE_PRES
//#define FLOAT_PRES

#ifdef DOUBLE_PRES
    #define Scalar double
#endif

#ifdef FLOAT_PRES
    #define Scalar float
#endif

//Remember 1 Degree = PI/180
//         1 Radian = 180/PI
class Math
{
private:
    static bool randInit;
public:
    static int randInt(int x, int y);
    static Scalar randScalar();
    static Scalar getSin(Scalar a){return sin(a*PI/180);};
    static Scalar getCos(Scalar a){return cos(a*PI/180);};
};

class Vector//rewrite for 2d later
{
private:
    Scalar x,y,z;
    void multQ(Scalar x1, Scalar y1, Scalar z1, Scalar w1, Scalar x2, Scalar y2, Scalar z2, Scalar w2, Scalar* rx, Scalar* ry, Scalar* rz, Scalar* rw);
public:
    Vector(){x=y=x=0;};
    Vector(Scalar ax, Scalar ay, Scalar az = 0){x = ax; y = ay; z = az;};
    ~Vector(){};
    void setX(Scalar a){x = a;};
    void setY(Scalar a){y = a;};
    void set(Scalar ax, Scalar ay){x = ax; y = ay;};
    void set(Vector v){x = v.getX(); y = v.getY();};
    Scalar getX(){return x;};
    Scalar getY(){return y;};
    Scalar length();
    void setLength(Scalar l);
    void normalize();
    void negate();
    Vector operator+(Vector v);
    Vector operator-(Vector v);
    Vector operator*(Scalar d);
    Vector operator/(Scalar d);
    void   operator=(Vector v);
    Scalar dot(Vector v);
    Vector cross(Vector v);
    Scalar angle(Vector v);
    Scalar angle();
    //void rotate(Scalar rx, Scalar ry = 0, Scalar rz = 0);//fix for 2d
    void rotate(Scalar theta, Vector v);
    void rotate(Scalar angle);
    void setDir(Scalar dx, Scalar dy, Scalar dz = 0);
    void rotX(Scalar t);
    void rotY(Scalar t);
    void rotZ(Scalar t);
};

class Rect
{
private:
    Scalar x,y,width,height,rot;
public:
    Rect(){x=y=width=height=rot=0;};
    Rect(Scalar ax, Scalar ay, Scalar aw, Scalar ah, Scalar ar=0){x=ax; y=ay; width=aw; height=ah; rot = ar;};
    ~Rect(){};

    Scalar getX(){return x;};
    Scalar getY(){return y;};
    Scalar getWidth(){return width;};
    Scalar getHeight(){return height;};
    Scalar getRot(){return rot;};
    Scalar getX2(){return x+width-1;};
    Scalar getY2(){return y+height-1;};
    Scalar getCenterX(){return getX()+(width-1)/2;};
    Scalar getCenterY(){return getY()+(height-1)/2;};

    void setRot(Scalar r){rot = r;};

    void setX(Scalar ax){x = ax;};
    void setY(Scalar ay){y = ay;};
    void setX2(Scalar ax){x=ax-width+1;};
    void setY2(Scalar ay){y=ay-height+1;};
    void setCenterX(Scalar ax){x = ax-(width-1)/2;};
    void setCenterY(Scalar ay){y = ay-(height-1)/2;};
    void setWidth(Scalar aw){width = aw;};
    void setHeight(Scalar ah){height = ah;};
    void setPos(Scalar ax, Scalar ay){x = ax; y = ay;};
    void setSize(Scalar aw, Scalar ah){width = aw; height = ah;};
    void scale(Scalar sx, Scalar sy){x*=sx; y*=sy;};
    void scaleX(Scalar sx){x*=sx;};
    void scaleY(Scalar sy){y*=sy;};

    bool contains(Rect r);
    bool overlaps(Rect r);  //Rects actually overlap each other Axis Aligned
    bool collides(Rect r);  //The edges of rects touch Axis Aligned
    bool contains(Scalar x, Scalar y); //Axis aligned
    bool lineOverlaps(Scalar x1, Scalar y1, Scalar x2, Scalar y2);

    Rect getRectUp(int size = 1){return Rect(x,y-size,width,size);};
    Rect getRectDown(int size = 1){return Rect(x,y+height,width,size);};
    Rect getRectLeft(int size = 1){return Rect(x-size,y,size,height);};
    Rect getRectRight(int size = 1){return Rect(x+width,y,size,height);};
};

#endif

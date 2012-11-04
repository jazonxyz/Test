#ifndef WORLD_H
#define WORLD_H

#include "Graphics.h"
#include "Math.h"
#include "Resources.h"

#include <list>

class World;
class WorldObject;
class QuadTree;

class RenderSorter
{
private:
public:
    RenderSorter(){};
    ~RenderSorter(){};
    virtual std::vector<WorldObject*> sort(std::vector<WorldObject*> items){return items;};
};

class QuadTreeNode : public Rect
{
private:
    //std::vector<WorldObject*> objects;
    std::list<WorldObject*> objects;

    QuadTreeNode* nodes[4];
    QuadTreeNode* parent;
public:
    QuadTreeNode(QuadTreeNode* p, Scalar x, Scalar y, Scalar w, Scalar h);
    ~QuadTreeNode(){};
    QuadTreeNode* getParent(){return parent;};
    void create(int layers);
    void sort(WorldObject* obj);
    void remove(WorldObject* obj);
    void deleteChildren();

    bool checkOverlapping(Rect r, Rect* except);
    Scalar getLowestXIn(Scalar result, bool started, Rect r, Rect* except);
    Scalar getHighestXIn(Scalar result, bool started, Rect r, Rect* except);
    Scalar getLowestYIn(Scalar result, bool started, Rect r, Rect* except);
    Scalar getHighestYIn(Scalar result, bool started, Rect r, Rect* except);

    void getCollisions(QuadTree* w, Rect r, Rect* except = NULL);
    void getOverlapping(QuadTree* w, Rect r, Rect* except = NULL);
    void getSolidOverlapping(QuadTree* w, Rect r, Rect* except = NULL);
    void getSolidCollisions(QuadTree* w, Rect r, Rect* except = NULL);

    void render(System* sys);
};

class QuadTree
{
private:
    QuadTreeNode* root;
    std::vector<WorldObject*> results;
public:
    QuadTree();
    ~QuadTree(){};
    void create(Scalar width, Scalar height, int layers);
    void free();
    void addObject(WorldObject* obj);
    void remove(WorldObject* obj);
    bool checkOverlapping(Rect r, Rect* except);

    void addToRes(WorldObject* o){results.push_back(o);};

    Scalar getLowestXIn(Rect r, Rect* except);
    Scalar getHighestXIn(Rect r, Rect* except);
    Scalar getLowestYIn(Rect r, Rect* except);
    Scalar getHighestYIn(Rect r, Rect* except);

    std::vector<WorldObject*> getCollisions(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getOverlapping(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getSolidOverlapping(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getSolidCollisions(Rect r, Rect* except = NULL);

    void render(System* sys);

};

class WorldObject : public Rect
{
private:
    bool solid;
    int hashID;
    int type;
    bool toRemove;
    bool isActive;
    World* world;
    static int typeGen;
    QuadTreeNode* node;
public:
    static int generateType();
    WorldObject(int x, int y);
    WorldObject(int x, int y, int aType);
    ~WorldObject(){};

    void move(Scalar x, Scalar y);
    void remove(){toRemove = true; activate(true);};
    bool isRemoved(){return toRemove;};//Only to be used by World
    void updatePos();//Only to be used by world
    void setWorld(World* w){world = w;}; //only to be used by world
    int getType(){return type;};
    bool isSolid(){return solid;};
    void setSolid(bool s){solid = s;};

    void setNode(QuadTreeNode* n){node = n;};
    QuadTreeNode* getNode(){return node;};

    World* getWorld(){return world;};
    Map* getMap();

    std::vector<WorldObject*> getCollisions();
    std::vector<WorldObject*> getSolidCollisions();
    std::vector<WorldObject*> getCollisionsUp(Scalar dist);
    std::vector<WorldObject*> getCollisionsDown(Scalar dist);
    std::vector<WorldObject*> getCollisionsLeft(Scalar dist);
    std::vector<WorldObject*> getCollisionsRight(Scalar dist);

    std::vector<WorldObject*> getSolidCollisionsUp(Scalar dist);
    std::vector<WorldObject*> getSolidCollisionsDown(Scalar dist);
    std::vector<WorldObject*> getSolidCollisionsLeft(Scalar dist);
    std::vector<WorldObject*> getSolidCollisionsRight(Scalar dist);

    std::vector<WorldObject*> getOverlapping();
    std::vector<WorldObject*> getOverlappingUp(Scalar dist);
    std::vector<WorldObject*> getOverlappingDown(Scalar dist);
    std::vector<WorldObject*> getOverlappingLeft(Scalar dist);
    std::vector<WorldObject*> getOverlappingRight(Scalar dist);

    bool overlapsMap(int num = 1);
    bool overlapsMapDown(int num = 1);
    bool overlapsMapUp(int num = 1);
    bool overlapsMapRight(int num = 1);
    bool overlapsMapLeft(int num = 1);

    virtual void update(){move(1,1);};
    virtual void render();
    virtual void kill(){};

    void activate(bool active = true);
    bool active(){return isActive;};

    void setHashID(int id){hashID = id;};
    int getHashID(){return hashID;};
};

class World : public Resource
{
private:
    Map map;
    //std::vector<WorldObject*> objects;
    //std::list<WorldObject*> objects;
    std::vector<std::list<WorldObject*> > objectLists;
    std::vector<int> objectLengths;

    std::list<WorldObject*> activeObjects;
    System* system;
    QuadTree tree;
    bool mapLoaded;
    int width, height;
public:
    World();
    World(char name[], char fileName[], System* sys);
    ~World(){};

    System* getSystem(){return system;};

    void loadMap(char fileName[], System* sys);
    void create(int width, int height, int depth, System* sys);

    WorldObject* addObject(WorldObject* o);

    std::vector<WorldObject*> getObjects();
    //void updateObject(WorldObject* o);
    void removeObject(WorldObject* o);
    void clearObjects();
    int getObjectCount();//{return (int)objects.size();};

    void enableMapDrawing(bool b){mapLoaded = b;};

    void update();
    void render(RenderSorter* sorter = NULL);
    void free();

    std::vector<WorldObject*> getCollisions(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getOverlapping(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getSolidOverlapping(Rect r, Rect* except = NULL);
    std::vector<WorldObject*> getSolidCollisions(Rect r, Rect* except = NULL);

    bool checkOverlapping(Rect r, Rect* except);
    Scalar getLowestXIn(Rect r, Rect* except);
    Scalar getHighestXIn(Rect r, Rect* except);
    Scalar getLowestYIn(Rect r, Rect* except);
    Scalar getHighestYIn(Rect r, Rect* except);

    int getWidth(){return width;};
    int getHeight(){return height;};

    Map* getMap(){return &map;};

    void activateObject(WorldObject* object);
};

#endif

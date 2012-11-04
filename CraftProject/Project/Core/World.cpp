#include "World.h"

World::World() : Resource("None")
{
    system = NULL;
    tree.free();
    mapLoaded = false;
    width = height = -1;

    std::list<WorldObject*> list;

    objectLists.push_back(list);
    objectLengths.push_back(0);
}

World::World(char name[], char fileName[], System* sys) : Resource(name)
{
    system = sys;
    loadMap(fileName, sys);

    width = map.getWidth()*map.getTileWidth();
    height = map.getHeight()*map.getTileHeight();
    tree.create(width, height, map.getTileWidth());

    std::list<WorldObject*> list;

    objectLists.push_back(list);
    objectLengths.push_back(0);
}

void World::create(int width, int height, int depth, System *sys)
{
    system = sys;
    this->width = width;
    this->height = height;
    tree.create(width,height, 50);
}

void World::loadMap(char fileName[], System* sys)
{
    setFileName(fileName);
    map.load(fileName, sys);
    mapLoaded = true;
}

WorldObject* World::addObject(WorldObject* o)
{
    o->setWorld(this);

    if(objectLengths[objectLengths.size()-1] >= 1000)
    {
        std::list<WorldObject*> list;
        objectLists.push_back(list);
        objectLengths.push_back(0);
    }

    objectLists[objectLists.size()-1].push_back(o);
    objectLengths[objectLists.size()-1]++;
    o->setHashID(objectLists.size()-1);

    tree.addObject(o);

    if(o->active())
        activeObjects.push_back(o);

    return o;
}

std::vector<WorldObject*> World::getObjects()
{
    std::vector<WorldObject*> ret;

    for(int i = 0; i < (int)objectLists.size(); i++)
    {
        std::list<WorldObject*> objects = objectLists[i];

        for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
        {
            ret.push_back(((WorldObject*)(*it)));
        }
    }

    return ret;
}

//void updateObject(WorldObject* o);
void World::removeObject(WorldObject* o)
{
    if(o->active())
        activeObjects.remove(o);

    o->kill();

    tree.remove(o);
    objectLengths[o->getHashID()]--;
    objectLists[o->getHashID()].remove(o);
    //objects.remove(o);
    delete o;
}

void World::clearObjects()
{
    for(int i = 0; i < (int)objectLists.size(); i++)
    {
        std::list<WorldObject*> objects = objectLists[i];
        for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
        {
            WorldObject* obj = (WorldObject*)(*it);

            if(obj!=NULL)
            {
                obj->kill();
                tree.remove(obj);
                delete obj;
            }
        }
    }

    objectLists.clear();
    objectLengths.clear();
}

int World::getObjectCount()
{
    int count = 0;

    for(int i = 0; i < (int)objectLengths.size(); i++)
    {
        count+=objectLengths[i];
    }

    return count;
}

void World::update()
{
    /*for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);

        if(obj != NULL)
            obj->update();
    }*/

    std::vector<WorldObject*> inactiveList;
    std::vector<WorldObject*> removeList;

    for(std::list<WorldObject*>::iterator it = activeObjects.begin(); it != activeObjects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);

        if(obj != NULL)
        {
            if(!obj->isRemoved())
            {
                obj->update();
            }
            else
            {
                removeList.push_back(obj);
            }

            if(!obj->active())
                inactiveList.push_back(obj);
        }
    }

    for(int i = 0; i < (int)inactiveList.size(); i++)
    {
        activeObjects.remove(inactiveList[i]);
    }

    //std::vector<WorldObject*> removeList;

    /*for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);

        if(obj == NULL || obj->isRemoved())
        {
            removeList.push_back(obj);
        }
    }*/

    for(int i = 0; i < (int)removeList.size(); i++)
    {
        removeObject(removeList[i]);
    }
}

void World::render(RenderSorter* sorter)
{
    Camera* cam = system->getGraphics()->getCamera();

    system->getGraphics()->enableCam();

     if(mapLoaded)
        map.draw1(0-cam->getTotalX(),0-cam->getTotalY());

    std::vector<WorldObject*> renderTargets = getOverlapping(Rect(cam->getTotalX(), cam->getTotalY(), cam->getTotalWidth(), cam->getTotalHeight()));

    if(sorter != NULL)
        renderTargets = sorter->sort(renderTargets);

    for(int i = 0; i < renderTargets.size(); i++)
    {
        renderTargets[i]->render();
    }

    /*for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != NULL)
            if(cam->overlapsAll(Rect(obj->getX(),obj->getY(),obj->getWidth(),obj->getHeight())))
                obj->render();

    }*/

    if(mapLoaded)
        map.draw2(0-cam->getTotalX(),0-cam->getTotalY());
    //tree.render(system);
     system->getGraphics()->enableCam(false);
}

void World::free()
{
    map.free();
    mapLoaded = false;
    clearObjects();

    width = height = -1;
}

std::vector<WorldObject*> World::getCollisions(Rect r, Rect* except)
{
    return tree.getCollisions(r,except);
}

std::vector<WorldObject*> World::getOverlapping(Rect r, Rect* except)
{
    return tree.getOverlapping(r,except);
}

std::vector<WorldObject*> World::getSolidOverlapping(Rect r, Rect* except)
{
    return tree.getSolidOverlapping(r,except);
}

std::vector<WorldObject*> World::getSolidCollisions(Rect r, Rect* except)
{
    //return getSolidOverlapping(Rect(r.getX()-1,r.getY()-1, r.getWidth()+2, r.getHeight()+2),except);
    return tree.getSolidCollisions(r,except);
}

bool World::checkOverlapping(Rect r, Rect* except)
{
    return tree.checkOverlapping(r,except);
}

Scalar World::getLowestXIn(Rect r, Rect* except)
{
    return tree.getLowestXIn(r,except);
}

Scalar World::getHighestXIn(Rect r, Rect* except)
{
    return tree.getHighestXIn(r,except);
}

Scalar World::getLowestYIn(Rect r, Rect* except)
{
    return tree.getLowestYIn(r,except);
}
Scalar World::getHighestYIn(Rect r, Rect* except)
{
    return tree.getHighestYIn(r,except);
}

void World::activateObject(WorldObject* object)
{
    activeObjects.push_back(object);
}

int WorldObject::typeGen = 1;

int WorldObject::generateType()
{
    return typeGen++;
}

WorldObject::WorldObject(int x, int y) : Rect(x,y,50,50)
{
    solid = true;
    type = 0;
    toRemove = false;
    world = NULL;
    node = NULL;
    isActive = true;
}

WorldObject::WorldObject(int x, int y, int aType) : Rect(x,y,50,50)
{
    type = aType;
    solid = true;
    toRemove = false;
    world = NULL;
    node = NULL;
    isActive = true;
}

void WorldObject::move(Scalar x, Scalar y)
{
    //Map Collision
    if(solid && getMap() != NULL)
    {
        //std::vector<WorldObject*> objects;//Moving Right
        if(x > 0)
        {
            if(getMap()->rectOverlaps(1,getRectRight(x)))
            {
                setX2(((int)(getX2()/getMap()->getTileWidth()) + 1)*getMap()->getTileWidth() - 1);
            }
            else if(world->checkOverlapping(getRectRight(x),this))
            {
                Scalar moveX = world->getLowestXIn(getRectRight(x),this);
                setX2(moveX-1);
            }
            else
            {
                setX(getX()+x);
            }
        }
        else if(x < 0)
        {
            if(getMap()->rectOverlaps(1,getRectLeft(x*-1)))
            {
                setX(((int)(getX()/getMap()->getTileWidth()))*getMap()->getTileWidth());
            }
            else if(world->checkOverlapping(getRectLeft(x*-1),this))
            {
                Scalar moveX = world->getHighestXIn(getRectLeft(x*-1),this);
                setX(moveX+1);
            }
            else
            {
                setX(getX()+x);
            }
        }

        if(y > 0)
        {
            //objects = world->getSolidOverlapping(getRectDown(y), this);
            if(getMap()->rectOverlaps(1,getRectDown(y)))
            {
                setY2(((int)(getY2()/getMap()->getTileHeight()) + 1)*getMap()->getTileHeight() - 1);
            }
            //else if(!objects.empty())
            else if(world->checkOverlapping(getRectDown(y),this))
            {
                /*Scalar moveY = objects[0]->getY();

                for(int i = 1; i < (int)objects.size(); i++)
                {
                     if(moveY > objects[i]->getY())
                         moveY = objects[i]->getY();
                }*/
                Scalar moveY = world->getLowestYIn(getRectDown(y), this);

                setY2(moveY-1);
            }
            else
            {
                setY(getY()+y);
            }
        }
        else if(y < 0)
        {
            //objects = world->getSolidOverlapping(getRectUp(y*-1), this);
            if(getMap()->rectOverlaps(1,getRectUp(y*-1)))
            {
                setY(((int)(getY()/getMap()->getTileHeight()))*getMap()->getTileHeight());
            }
            //else if(!objects.empty())
            else if(world->checkOverlapping(getRectUp(y*-1),this))
            {
                /*Scalar moveY = objects[0]->getY2();

                for(int i = 1; i < (int)objects.size(); i++)
                {
                     if(moveY > objects[i]->getY2())
                         moveY = objects[i]->getY2();
                }*/
                Scalar moveY = world->getHighestYIn(getRectUp(y*-1),this);

                setY(moveY+1);
            }
            else
            {
                setY(getY()+y);
            }
        }
    }
    else
    {
        setX(getX()+x);
        setY(getY()+y);
    }

    if((x != 0 || y != 0) && node != NULL)//Update in tree
    {
        QuadTreeNode* parent = node->getParent();
        node->remove(this);
        if(parent != NULL)
            parent->sort(this);
        else
            node->sort(this);
    }
}

void WorldObject::updatePos()
{
    //Stub
}

Map* WorldObject::getMap()
{
    if(world != NULL)
        return world->getMap();

    return NULL;
}

void WorldObject::render()
{

    if(world == NULL)
        return;

    if(world->getSystem() == NULL)
        return;

    world->getSystem()->getGraphics()->fillRect(Rect(getX(),getY(),getWidth(),getHeight()),Color(1,0,0,0.5));
}

std::vector<WorldObject*> WorldObject::getCollisions()
{
    return world->getCollisions(Rect(getX(),getY(),getWidth(),getHeight()),this);
}

std::vector<WorldObject*> WorldObject::getSolidCollisions()
{
    return world->getSolidCollisions(Rect(getX(),getY(),getWidth(),getHeight()),this);
}

std::vector<WorldObject*> WorldObject::getCollisionsUp(Scalar dist)
{
    return world->getCollisions(getRectUp(dist),this);
}

std::vector<WorldObject*> WorldObject::getCollisionsDown(Scalar dist)
{
    return world->getCollisions(getRectDown(dist),this);
}

std::vector<WorldObject*> WorldObject::getCollisionsLeft(Scalar dist)
{
    return world->getCollisions(getRectLeft(dist),this);
}

std::vector<WorldObject*> WorldObject::getCollisionsRight(Scalar dist)
{
    return world->getCollisions(getRectRight(dist),this);
}

std::vector<WorldObject*> WorldObject::getSolidCollisionsUp(Scalar dist)
{
    return world->getSolidCollisions(getRectUp(dist),this);
}

std::vector<WorldObject*> WorldObject::getSolidCollisionsDown(Scalar dist)
{
    return world->getSolidCollisions(getRectDown(dist),this);
}

std::vector<WorldObject*> WorldObject::getSolidCollisionsLeft(Scalar dist)
{
    return world->getSolidCollisions(getRectLeft(dist),this);
}

std::vector<WorldObject*> WorldObject::getSolidCollisionsRight(Scalar dist)
{
    return world->getSolidCollisions(getRectRight(dist),this);
}

std::vector<WorldObject*> WorldObject::getOverlapping()
{
    return world->getOverlapping(Rect(getX(),getY(),getWidth(),getHeight()),this);
}

std::vector<WorldObject*> WorldObject::getOverlappingUp(Scalar dist)
{
    return world->getOverlapping(getRectUp(dist),this);
}

std::vector<WorldObject*> WorldObject::getOverlappingDown(Scalar dist)
{
    return world->getOverlapping(getRectDown(dist),this);
}

std::vector<WorldObject*> WorldObject::getOverlappingLeft(Scalar dist)
{
    return world->getOverlapping(getRectLeft(dist),this);
}

std::vector<WorldObject*> WorldObject::getOverlappingRight(Scalar dist)
{
    return world->getOverlapping(getRectRight(dist),this);
}

bool WorldObject::overlapsMap(int num)
{
    return world->getMap()->rectOverlaps(num, Rect(getX(),getY(),getWidth(),getHeight()));
}

void WorldObject::activate(bool active)
{
    if(isActive == active)
        return;

    isActive = active;
    if(getWorld() != NULL)
        getWorld()->activateObject(this);
}

//////////////////////////////////////////////////////////////////////////////
//QuadTreeNode////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

QuadTreeNode::QuadTreeNode(QuadTreeNode* p, Scalar x, Scalar y, Scalar w, Scalar h) : Rect(x,y,w,h)
{
    for(int i = 0; i < 4; i++)
        nodes[i] = NULL;

    parent = p;
}

void QuadTreeNode::create(int layers)
{
    if(getWidth() > layers)
    {
        nodes[0] = new QuadTreeNode(this,getX(),getY(),getWidth()/2,getHeight()/2);
        nodes[1] = new QuadTreeNode(this,getX()+getWidth()/2, getY(),getWidth()/2,getHeight()/2);
        nodes[2] = new QuadTreeNode(this,getX()+getWidth()/2, getY()+getHeight()/2, getWidth()/2, getHeight()/2);
        nodes[3] = new QuadTreeNode(this,getX(),getY()+getHeight()/2, getWidth()/2,getHeight()/2);

        layers--;

        for(int i = 0; i < 4; i++)
        {
            nodes[i]->create(layers);
        }
    }
}

void QuadTreeNode::sort(WorldObject* obj)
{
    if(nodes[0] == NULL)
    {
        obj->setNode(this);
        objects.push_back(obj);
        return;
    }

    for(int i = 0; i < 4; i++)
    {
        if(nodes[i]->contains(Rect(obj->getX(),obj->getY(),obj->getWidth(),obj->getHeight())))
        {
            nodes[i]->sort(obj);
            return;
        }
    }

    obj->setNode(this);
    objects.push_back(obj);
}

void QuadTreeNode::remove(WorldObject* obj)
{
    objects.remove(obj);
}

void QuadTreeNode::deleteChildren()
{
    if(nodes[0] == NULL)
        return;

    for(int i = 0; i < 4; i++)
    {
        nodes[i]->deleteChildren();
        delete nodes[i];
        nodes[i] = NULL;
    }
}

bool QuadTreeNode::checkOverlapping(Rect r, Rect* except)
{
    /*for(int i = 0; i < (unsigned)objects.size(); i++)
    {
        if(objects[i] != except && objects[i]->isSolid() && objects[i]->overlaps(r))
            return true;
    }*/

    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        if(*it != except && (WorldObject*)(*it)->isSolid() && (WorldObject*)(*it)->overlaps(r))
            return true;
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                if(nodes[i]->checkOverlapping(r,except))
                    return true;
            }
        }
    }

    return false;
}

Scalar QuadTreeNode::getLowestXIn(Scalar result, bool started, Rect r, Rect* except)
{
    /*for(int i = 0; i < (unsigned)objects.size(); i++)
    {
        if(objects[i] != except && objects[i]->isSolid() &&  objects[i]->overlaps(r))
        {
            if(!started)
            {
                result = objects[i]->getX();
                started = true;
            }
            else if(result > objects[i]->getX())
            {
                result = objects[i]->getX();
            }
        }
    }*/
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() &&  obj->overlaps(r))
        {
            if(!started)
            {
                result = obj->getX();
                started = true;
            }
            else if(result > obj->getX())
            {
                result = obj->getX();
            }
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                result = nodes[i]->getLowestXIn(result,started,r,except);
            }
        }
    }

    return result;
}

Scalar QuadTreeNode::getHighestXIn(Scalar result, bool started, Rect r, Rect* except)
{
    /*for(int i = 0; i < (unsigned)objects.size(); i++)
    {
        if(objects[i] != except && objects[i]->isSolid() &&  objects[i]->overlaps(r))
        {
            if(!started)
            {
                result = objects[i]->getX2();
                started = true;
            }
            else if(result < objects[i]->getX2())
            {
                result = objects[i]->getX2();
            }
        }
    }*/
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() &&  obj->overlaps(r))
        {
            if(!started)
            {
                result = obj->getX2();
                started = true;
            }
            else if(result < obj->getX2())
            {
                result = obj->getX2();
            }
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                result = nodes[i]->getHighestXIn(result,started,r,except);
            }
        }
    }

    return result;

}

Scalar QuadTreeNode::getLowestYIn(Scalar result, bool started, Rect r, Rect* except)
{
    /*for(int i = 0; i < (unsigned)objects.size(); i++)
    {
        if(objects[i] != except && objects[i]->isSolid() &&  objects[i]->overlaps(r))
        {
            if(!started)
            {
                result = objects[i]->getY();
                started = true;
            }
            else if(result > objects[i]->getY())
            {
                result = objects[i]->getY();
            }
        }
    }*/
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() &&  obj->overlaps(r))
        {
            if(!started)
            {
                result = obj->getY();
                started = true;
            }
            else if(result > obj->getY())
            {
                result = obj->getY();
            }
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                result = nodes[i]->getLowestYIn(result,started,r,except);
            }
        }
    }

    return result;
}

Scalar QuadTreeNode::getHighestYIn(Scalar result, bool started, Rect r, Rect* except)
{
    /*for(int i = 0; i < (unsigned)objects.size(); i++)
    {
        if(objects[i] != except && objects[i]->isSolid() &&  objects[i]->overlaps(r))
        {
            if(!started)
            {
                result = objects[i]->getY2();
                started = true;
            }
            else if(result > objects[i]->getY2())
            {
                result = objects[i]->getY2();
            }
        }
    }*/

    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() &&  obj->overlaps(r))
        {
            if(!started)
            {
                result = obj->getY2();
                started = true;
            }
            else if(result < obj->getY2())
            {
                result = obj->getY2();
            }
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                result = nodes[i]->getHighestYIn(result,started,r,except);
            }
        }
    }

    return result;

}

void QuadTreeNode::getCollisions(QuadTree* w, Rect r, Rect* except)
{
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except &&  obj->collides(r))
        {
            w->addToRes(obj);
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->collides(r))
            {
                nodes[i]->getCollisions(w,r,except);
            }
        }
    }
}

void QuadTreeNode::getOverlapping(QuadTree* w, Rect r, Rect* except)
{
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except &&  obj->overlaps(r))
        {
            w->addToRes(obj);
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                nodes[i]->getOverlapping(w,r,except);
            }
        }
    }
}

void QuadTreeNode::getSolidOverlapping(QuadTree* w, Rect r, Rect* except)
{
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() && obj->overlaps(r))
        {
            w->addToRes(obj);
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->overlaps(r))
            {
                nodes[i]->getSolidOverlapping(w,r,except);
            }
        }
    }
}

void QuadTreeNode::getSolidCollisions(QuadTree* w, Rect r, Rect* except )
{
    for(std::list<WorldObject*>::iterator it = objects.begin(); it != objects.end(); it++)
    {
        WorldObject* obj = (WorldObject*)(*it);
        if(obj != except && obj->isSolid() && obj->collides(r))
        {
            w->addToRes(obj);
        }
    }

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            if(nodes[i]->collides(r))
            {
                nodes[i]->getSolidCollisions(w,r,except);
            }
        }
    }
}

void QuadTreeNode::render(System* sys)
{
    sys->getGraphics()->drawRect(Rect(getX(),getY(),getWidth(),getHeight()),Color(1,0,0,.2));

    if(nodes[0] != NULL)
    {
        for(int i = 0; i < 4; i++)
        {
            nodes[i]->render(sys);
        }
    }
}

QuadTree::QuadTree()
{
    root = NULL;
}

void QuadTree::create(Scalar width, Scalar height, int layers)
{
    free();
    root = new QuadTreeNode(NULL,0,0,width,height);

    root->create(layers);
}

void QuadTree::free()
{
    if(root == NULL)
        return;

    root->deleteChildren();
    delete root;
    root = NULL;
}

void QuadTree::addObject(WorldObject* obj)
{
    if(root != NULL)
        root->sort(obj);
}

void QuadTree::remove(WorldObject* obj)
{
    obj->getNode()->remove(obj);
}

bool QuadTree::checkOverlapping(Rect r, Rect* except)
{
    return root->checkOverlapping(r,except);
}

Scalar QuadTree::getLowestXIn(Rect r, Rect* except)
{
    return root->getLowestXIn(0,0,r,except);
}

Scalar QuadTree::getHighestXIn(Rect r, Rect* except)
{
    return root->getHighestXIn(0,0,r,except);
}

Scalar QuadTree::getLowestYIn(Rect r, Rect* except)
{
    return root->getLowestYIn(0,0,r,except);
}

Scalar QuadTree::getHighestYIn(Rect r, Rect* except)
{
    return root->getHighestYIn(0,0,r,except);
}

std::vector<WorldObject*> QuadTree::getCollisions(Rect r, Rect* except)
{
    results.clear();
    root->getCollisions(this,r,except);
    return results;
}

std::vector<WorldObject*> QuadTree::getOverlapping(Rect r, Rect* except)
{
    results.clear();
    root->getOverlapping(this,r,except);
    return results;
}

std::vector<WorldObject*> QuadTree::getSolidOverlapping(Rect r, Rect* except)
{
    results.clear();
    root->getSolidOverlapping(this,r,except);
    return results;
}

std::vector<WorldObject*> QuadTree::getSolidCollisions(Rect r, Rect* except)
{
    results.clear();
    root->getSolidCollisions(this,r,except);
    return results;
}

void QuadTree::render(System* sys)
{
    root->render(sys);
}

#include "EntityFactory.h"

void EntityFactory::init(System* sys, ItemFactory *f)
{
    itemFactory = f;
    resMgr.add(new Image("treeImage", "tree.bmp", sys, 25, 25));
    resMgr.add(new Image("rockImage", "rock.bmp", sys, 25, 25));
    resMgr.add(new Image("playerImage", "player.bmp", sys, 25, 25));
    resMgr.add(new Image("pigImage", "pig.bmp", sys, 25, 25));
    resMgr.add(new Image("sparkImage", "spark.bmp", sys));
    resMgr.add(new Image("bloodImage", "blood.bmp", sys, 4, 4));
}

void EntityFactory::free()
{
    resMgr.freeAll();
}

PlayerObject* EntityFactory::makePlayer(Input* in, Inventory* inv,int x, int y)
{
    return new PlayerObject((Image*)resMgr.get("playerImage"), in, this, inv, x, y);
}

Entity* EntityFactory::makePig(int x, int y)
{
    return new Pig((Image*)resMgr.get("pigImage"), this, x, y);
}

Entity* EntityFactory::makeRock(int x, int y)
{
    return new Rock((Image*)resMgr.get("rockImage"), this, x, y);
}

Entity* EntityFactory::makeTree(int x, int y)
{
    return new Tree((Image*)resMgr.get("treeImage"), this, x, y);
}

Effect* EntityFactory::makeSpark(int x, int y)
{
    return new SparkEffect((Image*)resMgr.get("sparkImage"), x, y);
}

Effect* EntityFactory::makeBlood(int x, int y)
{
    return new BloodEffect((Image*)resMgr.get("bloodImage"), x, y);
}

SolidItem* EntityFactory::makeItem(int id, int x, int y)
{
    return new SolidItem(itemFactory->getItemImage(id), x, y, id);
}

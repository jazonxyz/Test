#include "Level.h"

std::vector<WorldObject*> LevelSorter::sort(std::vector<WorldObject*> items)
{
    std::vector<WorldObject*> itemObjects;
    std::vector<WorldObject*> miscObjects;

    for(int i = 0; i < items.size(); i++)
    {
            if(items[i]->getType() == Entity::getID())
            {
                if(((Entity*)items[i])->getType() == ENTITY_ITEM)
                    itemObjects.push_back(items[i]);
                else
                {
                    miscObjects.push_back(items[i]);
                }
            }
            else
            {
                miscObjects.push_back(items[i]);
            }
    }

    std::vector<WorldObject*> result;

    for(int i = 0; i < (int)itemObjects.size(); i++)
        result.push_back(itemObjects[i]);

    for(int i = 0; i < (int)miscObjects.size(); i++)
        result.push_back(miscObjects[i]);

    return result;
}

void Level::init(System* sys, Player* player)
{
    int width = 1000;
    int height = 1000;

    itemFactory.init(sys);
    factory.init(sys, &itemFactory);

    music.load("HappySong.mp3");

    LevelGen generator;
    int time = sys->getTicks();
    world = generator.generate(width,height, &factory, sys);

    generateSpawnList();

    Spawn* spawn = getRandomSpawn();


    PlayerObject* playerObject;

    playerObject = factory.makePlayer(sys->getInput(), player->getInventory(), spawn->getX(), spawn->getY());
    in = sys->getInput();
    world->addObject(playerObject);

    player->setObject(playerObject);

    player->getInventory()->setFactory(&itemFactory);
}

void Level::free()
{
    world->clearObjects();
    delete world;
    //resMgr.freeAll();
    factory.free();
    itemFactory.free();
    music.free();
}

void Level::update()
{
    if(!music.isPlaying())
        music.play();

    world->update();
}

void Level::render()
{
    world->render(&sorter);
}

void Level::generateSpawnList()
{
    std::vector<WorldObject*> objects = world->getObjects();

    for(int i = 0; i < (int)objects.size(); i++)
    {
        if(objects[i]->getType() == Spawn::getID())
            spawnList.push_back((Spawn*)objects[i]);
    }
}

Spawn* Level::getRandomSpawn()
{
    return spawnList[rand()%spawnList.size()];
}

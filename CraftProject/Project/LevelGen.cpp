#include "LevelGen.h"

enum{OBJECT_TREE=1, OBJECT_ROCK};

World* LevelGen::generate(int width, int height, EntityFactory* factory, System* sys)
{
    int totalWidth = width*25;
    int totalHeight = height*25;

    World* world = new World();
    world->create(totalWidth,totalHeight, 10, sys);
    world->getMap()->create("tiles.bmp", sys, width, height, 25, 25);
    world->enableMapDrawing(true);

    for(int i = 0; i < width; i++)
    {
        std::vector<int> list;

        for(int l = 0; l < height; l++)
        {
            list.push_back(0);
        }

        objects.push_back(list);
    }

    Map* map = world->getMap();

    //Generate Grass
    for(int y = 0; y < height; y++)
        for(int x = 0; x < width; x++)
        {
            world->getMap()->setTile1(TILE_GRASS, x, y);
        }

    //Generate Water
    generateWater(world);

    //Add Trees
    generateTrees(world);

    //Generate Rocks
    generateRocks(world);

    //Generate Spawns
    generateSpawns(world);

    //Place objects
    placeObjects(world, factory);

    return world;
}

void LevelGen::generateWater(World* world)
{
    //Generate pond sources
    int width = world->getMap()->getWidth();
    int height = world->getMap()->getHeight();

    for(int i = 0; i < width*height/500; i++)
    {
        world->getMap()->setTile1(TILE_WATER, rand()%width, rand()%height);
    }

    //Proliferate ponds
    for(int i = 0; i < 5; i++)
        for(int y = 1; y < height-1; y++)
            for(int x = 1; x < width-1; x++)
            {
                if(world->getMap()->getTile1(x,y) == TILE_WATER)
                {
                    if(rand()%2 == 0)
                        world->getMap()->setTile1(TILE_WATER, x+1, y);
                    if(rand()%2 == 0)
                        world->getMap()->setTile1(TILE_WATER, x-1, y);
                    if(rand()%2 == 0)
                        world->getMap()->setTile1(TILE_WATER, x, y-1);
                    if(rand()%2 == 0)
                        world->getMap()->setTile1(TILE_WATER, x, y+1);
            }
        }

        //make water solid

        for(int y = 0; y < height; y++)
            for(int x = 0; x < width; x++)
            {
                if(world->getMap()->getTile1(x,y) == TILE_WATER)
                {
                    world->getMap()->setSolid(1, x, y);
                }
            }


    //Generate Sand
    for(int i = 0; i < 5; i++)
        for(int y = 1; y < height-1; y++)
            for(int x = 1; x < width-1; x++)
            {
                if(world->getMap()->getTile1(x,y) == TILE_GRASS)
                {
                    if(world->getMap()->getTile1(x+1, y) == TILE_WATER ||
                       world->getMap()->getTile1(x-1, y) == TILE_WATER ||
                       world->getMap()->getTile1(x, y-1) == TILE_WATER ||
                       world->getMap()->getTile1(x, y+1) == TILE_WATER)
                       {
                           world->getMap()->setTile1(TILE_SAND, x, y);
                       }
            }
        }
}

void LevelGen::generateTrees(World* world)
{
    int width = world->getMap()->getWidth();
    int height = world->getMap()->getHeight();

    int numTrees = width*height/5;

    while(numTrees > 0)
    {
        int x = rand()%width;
        int y = rand()%height;

        if(world->getMap()->getTile1(x,y) == TILE_GRASS &&
           world->getMap()->getTile1(x+1,y) == TILE_GRASS &&
           world->getMap()->getTile1(x-1,y) == TILE_GRASS &&
           world->getMap()->getTile1(x,y+1) == TILE_GRASS &&
           world->getMap()->getTile1(x,y-1) == TILE_GRASS &&
           world->getMap()->getTile1(x+1,y+1) == TILE_GRASS &&
           world->getMap()->getTile1(x-1,y+1) == TILE_GRASS &&
           world->getMap()->getTile1(x-1,y-1) == TILE_GRASS &&
           world->getMap()->getTile1(x+1,y-1) == TILE_GRASS &&
           objects[x][y] != OBJECT_TREE)
        {
            //world->addObject(factory->makeTree(x*world->getMap()->getTileWidth(), y*world->getMap()->getTileHeight()));
            objects[x][y] = OBJECT_TREE;
            numTrees--;
        }
    }
}



void LevelGen::generateRocks(World* world)
{
    int width = world->getMap()->getWidth();
    int height = world->getMap()->getHeight();
    Map* map = world->getMap();

    int numRockPatches = width*height/400;
    //std::vector<Rock*> rocks;

    while(numRockPatches > 0)
    {
        int x = rand()%width;
        int y = rand()%height;

        if(map->getTile1(x,y) == TILE_GRASS &&
           map->getTile1(x+1,y) == TILE_GRASS &&
           map->getTile1(x-1,y) == TILE_GRASS &&
           map->getTile1(x,y+1) == TILE_GRASS &&
           map->getTile1(x,y-1) == TILE_GRASS &&
           map->getTile1(x+1,y+1) == TILE_GRASS &&
           map->getTile1(x-1,y+1) == TILE_GRASS &&
           map->getTile1(x-1,y-1) == TILE_GRASS &&
           map->getTile1(x+1,y-1) == TILE_GRASS &&
           objects[x][y] != OBJECT_ROCK)
        {
            objects[x][y] = OBJECT_ROCK;
            numRockPatches--;
        }
    }

    for(int i = 0; i < 2; i++)
        for(int y = 1; y < height-1; y++)
            for(int x = 1; x < width-1; x++)
            {
                if(objects[x][y] == OBJECT_ROCK)
                {
                    if(map->getTile1(x,y) == TILE_GRASS &&
                        map->getTile1(x+1,y) == TILE_GRASS &&
                        map->getTile1(x-1,y) == TILE_GRASS &&
                        map->getTile1(x,y+1) == TILE_GRASS &&
                        map->getTile1(x,y-1) == TILE_GRASS &&
                        map->getTile1(x+1,y+1) == TILE_GRASS &&
                        map->getTile1(x-1,y+1) == TILE_GRASS &&
                        map->getTile1(x-1,y-1) == TILE_GRASS &&
                        map->getTile1(x+1,y-1) == TILE_GRASS)
                    {
                        if(rand()%2 == 0)
                            objects[x+1][y] = OBJECT_ROCK;
                        if(rand()%2 == 0)
                            objects[x-1][y] = OBJECT_ROCK;
                        if(rand()%2 == 0)
                            objects[x][y-1] = OBJECT_ROCK;
                        if(rand()%2 == 0)
                            objects[x][y+1] = OBJECT_ROCK;
                    }
                }
            }
}

void LevelGen::generateSpawns(World* world)
{
    int width = world->getMap()->getWidth();
    int height = world->getMap()->getHeight();
    Map* map = world->getMap();
    int numSpawns = 3;//width*height/1600;

    while(numSpawns > 0)
    {
        int x = rand()%width;
        int y = rand()%height;

        if(map->getTile1(x,y) == TILE_GRASS &&
           map->getTile1(x+1,y) == TILE_GRASS &&
           map->getTile1(x-1,y) == TILE_GRASS &&
           map->getTile1(x,y+1) == TILE_GRASS &&
           map->getTile1(x,y-1) == TILE_GRASS)
        {
            //std::vector<WorldObject*> objects = world->getOverlapping(Rect((x)*map->getTileWidth(), (y)*map->getTileHeight(), map->getWidth(), map->getHeight()));


            //if(objects.empty())
            if(objects[x][y] == 0)
            {
                Spawn* spawn = new Spawn(x*world->getMap()->getTileWidth(), y*world->getMap()->getTileHeight());
                world->addObject(spawn);
                numSpawns--;
            }
        }
    }
}

void LevelGen::placeObjects(World* world, EntityFactory* factory)
{
    for(int x = 0; x < (int)objects.size(); x++)
    {
        for(int y = 0; y < (int)objects[x].size(); y++)
        {
            if(objects[x][y] == OBJECT_TREE)
                world->addObject(factory->makeTree(x*world->getMap()->getTileWidth(), y*world->getMap()->getTileHeight()));
        }

        for(int y = 0; y < (int)objects[x].size(); y++)
        {
            if(objects[x][y] == OBJECT_ROCK)
            {
                //world->addObject(
                Rock* rock = ((Rock*)factory->makeRock(x*world->getMap()->getTileWidth(), y*world->getMap()->getTileHeight()));
                world->addObject(rock);

                rock->setFrame(calcFrame(x, y, OBJECT_ROCK));
            }
        }
    }
}

int LevelGen::calcFrame(int x, int y, int key)
{
    int frame = 0;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    if(x > 0 && objects[x-1][y] == key)
    {
        left = true;
    }

    if(x < (int)objects.size()-1 && objects[x+1][y] == key)
    {
        right = true;
    }

    if(y > 0 && objects[x][y-1] == key)
    {
        up = true;
    }

    if(y < (int)objects[0].size()-1 && objects[x][y+1] == key)
    {
        down = true;
    }

    if(left && right)
    {
        if(up && down)
            frame = 10;

        if(!up && !down)
            frame = 2;

        if(!up && down)
            frame = 6;

        if(!down && up)
            frame = 14;
    }

    if(!left && right)
    {
        if(up && down)
            frame = 9;

        if(!up && !down)
            frame = 1;

        if(!up && down)
            frame = 5;

        if(!down && up)
            frame = 13;
    }

    if(left && !right)
    {
        if(up && down)
            frame = 11;

        if(!up && !down)
            frame = 3;

        if(!up && down)
            frame = 7;

        if(!down && up)
            frame = 15;
    }

    if(!left && !right)
    {
        if(up && down)
            frame = 8;

        if(!up && down)
            frame = 4;

        if(!down && up)
            frame = 12;
    }

    return frame;
}

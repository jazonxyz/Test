#include "Game.h"

#include <stdlib.h>

Desktop::Desktop(Player* p, int width, int height) : Window("Desktop", 0, 0, width, height)
{
    player = p;

    int pannelY = height - 279;

    inventoryPanel = addChild(new Gui::PanelWindow("Inventory", 0, height-279, 221, 279));

    int i = 0;
    for(int y = 0; y < 8; y++)
        for(int x = 0; x < 8; x++)
        {
            inventoryButtons[i] = ((Gui::Button*)inventoryPanel->addChild(new Gui::Button("", 7+x*26, pannelY + 64 + y*26, 25, 25)));
            i++;
        }

    i = 0;

    for(int y = 0; y < 2; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            craftButtons[i] = ((Gui::Button*)inventoryPanel->addChild(new Gui::Button("", 7+x*26, pannelY + 7+y*26, 25, 25)));
            i++;
        }
    }
}

void Desktop::update()
{

    if(getInput()->keyHit(SDLK_RETURN))
    {
        if(inventoryPanel->visible())
            inventoryPanel->hide();
        else
            inventoryPanel->show();
    }

    std::vector<Item*> items = player->getInventory()->getList();

    for(int i = 0; i < 64; i++)
    {
        if(i < (int)items.size() && items[i] != NULL)
            inventoryButtons[i]->setIcon(items[i]->getImage());
        else
            inventoryButtons[i]->setIcon(NULL);
    }

    std::vector<int> posCraft = ItemFactory::getPosCraft(player->getInventory());

    for(int i = 0; i < 16; i++)
    {
        if(i < (int)posCraft.size())
        {
            craftButtons[i]->setIcon(player->getInventory()->getFactory()->getItemImage(posCraft[i]));//items[i]->getImage());
            if(craftButtons[i]->isFired())
            {
                //if(!level->getPlayer()->getInventory()->isFull())
                {
                    player->getInventory()->getFactory()->craft(posCraft[i], player->getInventory());
                }
            }
        }
        else
            craftButtons[i]->setIcon(NULL);
    }
}

void Game::init(System* system)
{
    sys = system;

    level.init(system, &player);

    gui.init(new Desktop(&player, system->getClientWidth(), system->getClientHeight()), system, "Font1.bmp");
}

void Game::kill()
{
    gui.kill();
    level.free();
}

void Game::update()
{

    if(sys->getInput()->keyDown(SDLK_a))
       sys->getGraphics()->getCamera()->zoom(1.1);

    if(sys->getInput()->keyDown(SDLK_z))
       sys->getGraphics()->getCamera()->zoom(0.9);

    //if(sys->getGraphics()->getCamera()->getZoom() < 8)
      //  sys->getGraphics()->getCamera()->setZoom(8);

    if(sys->getInput()->keyDown(SDLK_UP))
        sys->getGraphics()->getCamera()->moveY(-10);

    if(sys->getInput()->keyDown(SDLK_DOWN))
        sys->getGraphics()->getCamera()->moveY(10);

    if(sys->getInput()->keyDown(SDLK_LEFT))
        sys->getGraphics()->getCamera()->moveX(-10);

    if(sys->getInput()->keyDown(SDLK_RIGHT))
        sys->getGraphics()->getCamera()->moveX(10);

    if(player.getObject() != NULL)
        sys->getGraphics()->getCamera()->centerAt(player.getObject()->getCenterX(), player.getObject()->getCenterY());

    if(sys->getGraphics()->getCamera()->getTotalWidth() < level.getWorld()->getWidth())
        if(sys->getGraphics()->getCamera()->getX() < 0)
            sys->getGraphics()->getCamera()->setX(0);

    if(sys->getGraphics()->getCamera()->getTotalHeight() < level.getWorld()->getHeight())
        if(sys->getGraphics()->getCamera()->getY() < 0)
            sys->getGraphics()->getCamera()->setY(0);

    if(sys->getGraphics()->getCamera()->getTotalWidth() < level.getWorld()->getWidth())
        if(sys->getGraphics()->getCamera()->getTotalX2() > level.getWorld()->getWidth())
            sys->getGraphics()->getCamera()->setTotalX2(level.getWorld()->getWidth());

    if(sys->getGraphics()->getCamera()->getTotalHeight() < level.getWorld()->getHeight())
        if(sys->getGraphics()->getCamera()->getTotalY2() > level.getWorld()->getHeight())
            sys->getGraphics()->getCamera()->setTotalY2(level.getWorld()->getHeight());

    gui.update();

    level.update();
}

void Game::render(Graphics* g)
{
    g->enableCam(true);
    g->clear(1,1,1,0);

    level.render();

    gui.render();
}

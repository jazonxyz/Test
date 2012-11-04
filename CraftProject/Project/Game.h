#ifndef GAME_H
#define GAME_H

#include "Core\System.h"
#include "Core\World.h"
#include "Core\Gui.h"
#include "Player.h"
#include "Level.h"

class Desktop : public Window
{
private:
    Gui::Button* menuButton;
    Gui::Button* inventoryButtons[64];
    Gui::Button* craftButtons[16];
    Window* inventoryPanel;
    Player* player;
public:
    Desktop(Player* p, int width, int height);
    ~Desktop(){};
    void update();
};

class Game
{
    private:
        System* sys;
        Level level;
        Player player;
        Gui gui;
    public:
        void init(System* system);
        void kill();
        void update();
        void render(Graphics* g);
        World* getWorld(){return level.getWorld();};
};

#endif

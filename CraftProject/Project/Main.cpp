#include "Core/System.h"
#include "Game.h"

#define FPS 60

int main(int argc, char *argv[])
{
    System system;
    Game game;
    Font font;

    system.init("Evolution!", 800, 600);

    font.load("Font1.bmp", &system);

    game.init(&system);

    int frameTime = 0;
    int oldTicks = system.getTicks();

    while(system.isRunning() && !system.getInput()->keyDown(SDLK_ESCAPE))
    {
        int thisFrameTime = system.getTicks();
        system.update();

        game.update();

        game.render(system.getGraphics());

        char buffer[64];


        int fps = 0;

        frameTime = system.getTicks()-oldTicks;
        oldTicks = system.getTicks();

        if(frameTime != 0)
            fps = 1000/(frameTime);

        sprintf(buffer, "FPS: %d Objects: %d Frame Time: %d, CamX: %d\n", fps, game.getWorld()->getObjectCount(), frameTime, (int)system.getGraphics()->getCamera()->getTotalX2());

        font.draw(buffer, 10, 10, Color(0,0,0));

        system.getGraphics()->flip();

        thisFrameTime = system.getTicks()-thisFrameTime;

        if(thisFrameTime < 1000/FPS)
            system.delay(1000/FPS - thisFrameTime);
    }

    font.free();

    game.kill();
    system.kill();

    return 0;
}

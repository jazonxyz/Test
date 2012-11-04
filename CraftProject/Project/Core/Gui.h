#ifndef GUI_H
#define GUI_H

#include "System.h"
#include <vector>
#include <queue>
#include <string>

//enum MsgBoxFlag {MB_OK, MB_YESNO, MB_YESNOCANCEL, MB_NONE, MB_YES, MB_NO, MB_CANCEL};

enum{
    TB_OUTLINE = 0x0000,
    TB_EDIT = 0x0001,
    TB_CENTERED = 0x0002
    };

class Gui;

class Window : public Rect
{
private:
    friend class Gui;
    void destroyAll();
    void initAll();
    void updateAll();
    void updateMouseAll();
    bool isVisible;
    bool isFixed;
    Window* getTopWindow(int x, int y);
    std::queue<Window*> toKill;
    void renderAll();
protected:
    Gui* gui;
    Window* parent;
    std::vector<Window*> children;
    char caption[1024];
public:
    Font* font;
    Window(char cap[], int x, int y, int width, int height);
    System* getSystem();
    Input* getInput();
    virtual ~Window(){};
    virtual void setFocus();
    void show();
    void hide();
    void move(int x, int y);
    void moveTo(int x, int y);
    void pinDown(bool pin){isFixed = pin;};
    void setCaption(char* cap){sprintf(caption,cap);};
    char* getCaption(){return caption;};
    Window* addChild(Window* win);
    virtual void close();

    bool visible(){return isVisible;};

    Window* getParent(){return parent;};
    virtual void init(){};
    virtual void kill(){};
    virtual void render(){};
    virtual void update(){};
    virtual void mouseOver(int mouseX, int mouseY){};
    virtual void mouseLeftDown(int mouseX, int mouseY){};
    virtual void mouseLeftHit(int mouseX, int mouseY){};
    virtual void mouseRightDown(int mouseX, int mouseY){};
    virtual void mouseRightHit(int mouseX, int mouseY){};
    //virtual void keyBoard(Input* in){};
};

class Gui
{
private:
    Window* desktop;
    Window* focus;
    System* system;
    Input* input;
    Font font;
    class Bar;
    class XButton;
public:
    Gui();
    Gui(Window* desk, System* sys, char fontName[]);
    ~Gui();
    void init(Window* desk, System* sys, char fontName[]);
    void kill();
    void update();
    void render();
    Window* getDesktop(){return desktop;};

    Window* getFocus(){return focus;};
    void setFocus(Window* w){focus = w;};

    Window* getTopWindow(int x, int y);
    void putOnTop(Window* win);

    System* getSystem(){return system;};
    Input* getInput(){return input;};

    class PanelWindow;
    class BarWindow;
    class PopUpWindow;
    class Button;
    class Text;
    class ImageBox;
    class CheckBox;
    class VScrollBar;
    class HScrollBar;
};

class Gui::PanelWindow : public Window
{
public:
    PanelWindow(char caption[],int x, int y, int width, int height);
    ~PanelWindow(){};
    virtual void render();
};

class Gui::Bar : public Window
{
private:
    int xOff, yOff;
    bool isEngaged;
public:
    Bar(char caption[]);
    ~Bar(){};
    virtual void setFocus(){};
    virtual void update();
    virtual void render();
    virtual void mouseLeftHit(int mouseX,int mouseY);
};

class Gui::BarWindow : public PanelWindow
{
public:
    BarWindow(char caption[], int x, int y, int width, int height);
    ~BarWindow(){};
};

class Gui::Button : public Window
{
private:
    void (*function)();
protected:
    bool isEnabled;
    bool isEngaged;
    bool isMoused;
    bool fired;
    Image* image;
    Image* icon;
public:
    Button(char caption[], int x, int y, int width, int height);
    Button(char caption[], void (*afunction)(), int x, int y, int width, int height);
    Button(char caption[], void (*afunction)(), Image* aImage, int x, int y, int width, int height);
    ~Button(){};
    void setIcon(Image* i);
    virtual void execute();
    virtual void render();
    virtual void update();
    virtual void mouseLeftHit(int mouseX, int mouseY);
    virtual void mouseOver(int mouseX, int mouseY);
    void enable(bool e = true){isEnabled = e;};
    bool isFired();
};

class Gui::XButton : public Gui::Button
{
public:
    XButton();
    ~XButton(){};
    virtual void render();
    virtual void update();
    virtual void execute();
};

class Gui::PopUpWindow : public Gui::PanelWindow
{
private:
    Window* bar;
public:
    PopUpWindow(char cap[], int x, int y,int width, int height);
    ~PopUpWindow(){};
    Window* getBar(){return bar;};
};

class Gui::Text : public Window
{
private:
    int counter;
    int flags;
    int cursor;
public:
    Text(char cap[], int x, int y, int width, int height, int f = 0);
    ~Text(){};
    virtual void render();
    virtual void update();
};

class Gui::ImageBox : public Window
{
private:
    Image* image;
public:
    ImageBox(Image *i, int x, int y);
    ~ImageBox(){};
    void setImage(Image* i);
    virtual void render();
};

class Gui::CheckBox : public Gui::Button
{
private:
    bool checked;
public:
    CheckBox(char caption[], int x, int y, int width, int height);
    ~CheckBox(){};
    virtual void render();
    virtual void execute();
    void check(bool c = true){checked = c;};
    bool isChecked(){return checked;};
};

class Gui::VScrollBar : public Window
{
private:
    Gui::Button* up;
    Gui::Button* down;
    int sliderY;
    int sliderSize;
    int increments;
    bool locked;
    int lockedY;
public:
    VScrollBar(int x, int y, int height);
    ~VScrollBar(){};
    virtual void update();
    virtual void render();
    Scalar getValue();
    Scalar setScale(Scalar s);
    void setValue(Scalar s);
    void setIncrements(int i){increments = i;};
};

class Gui::HScrollBar : public Window
{
private:
    Gui::Button* up;
    Gui::Button* down;
    int sliderY;
    int sliderSize;
    int increments;
    bool locked;
    int lockedY;
public:
    HScrollBar(int x, int y, int height);
    ~HScrollBar(){};
    virtual void update();
    virtual void render();
    Scalar getValue();
    Scalar setScale(Scalar s);
    void setValue(Scalar s);
    void setIncrements(int i){increments = i;};
};

#endif

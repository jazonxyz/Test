#include "Gui.h"

Gui::Gui()
{
    desktop = focus = NULL;
    input = NULL;
    system = NULL;
}

Gui::Gui(Window* desk, System* sys, char fontName[])
{
    desktop = focus = NULL;
    system = NULL;
    input = NULL;
    init(desk,system, fontName);
}

Gui::~Gui()
{

}

void Gui::init(Window* desk, System* sys, char fontName[])
{
    desktop = desk;
    focus = desktop;
    system = sys;
    input = system->getInput();
    desk->gui = this;
    desk->parent = NULL;

    font.load(fontName,sys);
    desk->font = &font;
}

void Gui::update()
{
    desktop->updateAll();
    //Input* in = Input::getInstance();

    if(system == NULL)
        return;

    if(input == NULL)
        return;

    Window* topWin = desktop->getTopWindow(input->getMouseX(),input->getMouseY());

    if(topWin != NULL)
    {
        topWin->mouseOver(input->getMouseX(),input->getMouseY());

        if(input->mouseHit(MOUSE_LEFT))
        {
            topWin->mouseLeftHit(input->getMouseX(),input->getMouseY());
            if(focus != topWin)
            {
                topWin->setFocus();
                //putOnTop(topWin);
            }
        }

        if(input->mouseHit(MOUSE_RIGHT))
            topWin->mouseRightHit(input->getMouseX(),input->getMouseY());

        if(input->mouseDown(MOUSE_LEFT))
            topWin->mouseLeftDown(input->getMouseX(),input->getMouseY());

        if(input->mouseDown(MOUSE_RIGHT))
            topWin->mouseRightDown(input->getMouseX(),input->getMouseY());
    }
}

void Gui::render()
{
    desktop->show();
    desktop->renderAll();
}

void Gui::kill()
{
    if(desktop != NULL)
    {
        desktop->destroyAll();
        delete desktop;
        desktop = NULL;
    }

    font.free();
}

Window* Gui::getTopWindow(int x, int y)
{
    return desktop->getTopWindow(x,y);
}

void Gui::putOnTop(Window* win)
{
    if(win == desktop)
        return;

    Window* parent = win->parent;

    for(int i = 0; i < (int)parent->children.size(); i++)
    {
        if(parent->children[i] == win)
        {
            parent->children.erase(parent->children.begin() + i);
            break;
        }
    }

    parent->children.push_back(win);

    putOnTop(parent);
}

void Window::destroyAll()
{
    for(int i = 0; i < children.size(); i++)
    {
        children[i]->kill();
        children[i]->destroyAll();
        delete children[i];
        children[i] = NULL;
    }
}

void Window::renderAll()
{
    if(!isVisible)
        return;

    render();

    for(int i = 0; i < (int)children.size(); i++)
    {
        children[i]->renderAll();
    }
}

void Window::initAll()
{

}

void Window::updateAll()
{
    if(!isVisible)
        return;

    update();

    for(int i = 0; i < (int)children.size(); i++)
    {
        children[i]->gui = gui;
        children[i]->parent = this;

        if(children[i]->font == NULL)
            children[i]->font = font;

        children[i]->updateAll();
    }


    while(!toKill.empty())
        for(int i = 0; i < (int)children.size();)
        {
            if(children[i] == toKill.front())
            {
                children[i]->kill();
                children[i]->destroyAll();
                delete children[i];
                children.erase(children.begin() + i);
                toKill.pop();
            }
            else
                i++;
        }
}

void Window::updateMouseAll()
{

}

Window* Window::getTopWindow(int x, int y)
{
    Window* top = NULL;

    for(int i = (int)children.size()-1; i >= 0; i--)
    {
        if(children[i]->isVisible)
        {
            if(children[i]->contains(x,y) || !children[i]->isFixed)
            {
                top = children[i]->getTopWindow(x,y);
                if(top != NULL)
                    break;
            }
        }
    }

    if(top == NULL && contains(x,y))
        return this;

    return top;
}

System* Window::getSystem()
{
    if(gui != NULL)
        return gui->getSystem();

    return NULL;
}

Input* Window::getInput()
{
    if(gui != NULL)
        return gui->getInput();

    return NULL;
}

Window::Window(char cap[], int x, int y, int width, int height) : Rect(x,y,width,height)
{
    sprintf(caption,cap);
    gui = NULL;

    //Window* parent = NULL;

    isVisible = true;
    isFixed = false;
    font = NULL;
}

void Window::setFocus()
{
    //if(isFixed)
      //  return;
    if(gui != NULL)
    {
        gui->setFocus(this);
        gui->putOnTop(this);
    }
}

void Window::show()
{
    isVisible = true;

    for(int i = 0; i < (int)children.size(); i++)
    {
        if(children[i]->isFixed)
            children[i]->show();
    }
}

void Window::hide()
{
    isVisible = false;

    for(int i = 0; i < (int)children.size(); i++)
    {
        if(children[i]->isFixed)
            children[i]->hide();
    }
}

void Window::move(int x, int y)
{
    setX(getX()+x);
    setY(getY()+y);

    for(int i = 0; i < (int)children.size(); i++)
    {
        if(children[i]->isFixed)
            children[i]->move(x,y);
    }
}

void Window::moveTo(int x, int y)
{
    int xDiff = x-getX();
    int yDiff = y-getY();

    move(xDiff,yDiff);
}

Window* Window::addChild(Window* win)
{
    if(win->isFixed)
        children.insert(children.begin(), win);
    else
        children.push_back(win);

    win->parent = this;
    win->gui = gui;

    return win;
}

void Window::close()
{
    parent->toKill.push(this);
}

Gui::PanelWindow::PanelWindow(char caption[],int x, int y, int width, int height) : Window(caption,x,y,width,height)
{
    show();
}

void Gui::PanelWindow::render()
{
    int x = getX();
    int y = getY();
    int width = getWidth();
    int height = getHeight();

    getSystem()->getGraphics()->fillRect(x,y,width,height,Color(220,220,220));
    getSystem()->getGraphics()->drawRect(x,y,width,height,Color(0,0,0));
    for(int i = 0; i<3; i++)
    {
        x++;
        y++;
        width -= 2;
        height -= 2;

        getSystem()->getGraphics()->drawRect(x,y,width,height,Color(192,192,192));
    }
    x++;
    y++;
    width-=2;
    height-=2;
    getSystem()->getGraphics()->drawRect(x,y,width,height,Color(0,0,0));
}

Gui::Bar::Bar(char caption[]) : Window(caption, 0,0,0,0)
{
    pinDown(true);

    xOff = yOff = 0;

    isEngaged = false;
}

void Gui::Bar::update()
{
    if(parent == NULL)
        return;
    //rect = Rectangle(parent->rect.getX()+1,parent->rect.getY()+1,parent->rect.getWidth()-2,5);
    setPos(parent->getX()+1, parent->getY()+1);
    setSize(parent->getWidth()-2, 20);

    //Input* in = Input::getInstance();

    //if(in == NULL)
      //  return;

    if(getInput()->mouseUp(MOUSE_LEFT))
        isEngaged = false;

    if(isEngaged)
    {
        parent->moveTo(getInput()->getMouseX() - xOff -1, getInput()->getMouseY() - yOff -1);
    }
}

void Gui::Bar::mouseLeftHit(int mouseX,int mouseY)
{
    xOff = mouseX-getX();
    yOff = mouseY-getY();
    isEngaged = true;
}


void Gui::Bar::render()
{
    //getGraphics()->fillRect(getX(),getY(),getWidth(),getHeight(),0,0,255);
    getSystem()->getGraphics()->fillRect(getX(),getY(),getWidth(),getHeight(),Color(0,0,255,255));//,Color(0,0,0,255));
    getSystem()->getGraphics()->drawLine(getX(),getY()+getHeight()-1,getX()+getWidth(),getY()+getHeight()-1,Color(0,0,0));

    gui->font.draw(parent->getCaption(),getX()+5,getY()+getHeight()/2-gui->font.getCharHeight()/2,Color(255,255,255));
}

Gui::BarWindow::BarWindow(char caption[], int x, int y, int width, int height) : PanelWindow(caption,x,y,width,height)
{
    addChild(new Bar(caption));
}

Gui::Button::Button(char caption[], int x, int y, int width, int height) : Window(caption,x,y,width,height)
{
    function = NULL;
    pinDown(true);
    isEngaged = isMoused = false;
    isEnabled = true;
    image = NULL;
    fired = false;
    icon = NULL;

}

Gui::Button::Button(char caption[], void (*afunction)(), int x, int y, int width, int height) : Window(caption,x,y,width,height)
{
    function = afunction;
    pinDown(true);
    isEngaged = isMoused = false;
    isEnabled = true;
    image = NULL;
    fired = false;
    icon = NULL;
}

Gui::Button::Button(char caption[], void (*afunction)(), Image* aImage, int x, int y, int width, int height) : Window(caption,x,y,width,height)
{
    function = afunction;
    pinDown(true);
    isEngaged = isMoused = false;
    isEnabled = true;
    image = aImage;
    fired = false;
    icon = NULL;

    image->setFrameSize(getWidth()/2, getHeight()/2);
}

void Gui::Button::setIcon(Image* i)
{
    icon = i;
}

void Gui::Button::execute()
{
    //gui->desktop->addChild(new PopUpWindow("Cool Window",100,100,200,200));
    fired = true;
    if(function != NULL)
        function();
}

void Gui::Button::render()
{
    Image* icon = NULL;
    if(!isEnabled)
    {
        if(image == NULL)
        {
            getSystem()->getGraphics()->fillRect(*this,Color(220,220,220));//Disabled
            getSystem()->getGraphics()->drawRect(*this,Color(128,128,128));
            getSystem()->getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,Color(255,255,255));
        }
        else
            image->draw(getX(),getY(),3);

        //font->drawCentered(caption,*this,128,128,128);
        font->draw(caption,getX(),getY(),Color(128,128,128));

        if(icon != NULL)
            icon->draw((getX()+getWidth()/2)-icon->getWidth()/2, (getY()+getHeight()/2) - icon->getHeight()/2, Color(255,255,255,127));
        return;
    }

    if(!isEngaged)
    {
        //Input* in = Input::getInstance();
        //if(in == NULL)
          //  return;

        if(!isMoused)
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(*this,Color(220,220,220));//Normal
                getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
                getSystem()->getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,Color(255,255,255));
            }
            else
                image->draw(getX(),getY(),0);

            //font->drawCentered(caption,Rectangle(getX(),getY(),getWidth(),getHeight()),0,0,0);
            font->draw(caption,getX(),getY(),Color(0,0,0));
        }
        else
        {
            if(image==NULL)
            {
                getSystem()->getGraphics()->fillRect(*this,Color(255,255,255));//Up
                getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
                getSystem()->getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,Color(0,0,255));
            }
            else
                image->draw(getX(),getY(),2);

            //font->drawCentered(caption,Rectangle(getX()-1,getY()-1,getWidth(),getHeight()),0,0,0);
            font->draw(caption,getX(),getY(),Color(0,0,0));
        }

        if(icon != NULL)
            icon->draw((getX()+getWidth()/2)-icon->getWidth()/2, (getY()+getHeight()/2) - icon->getHeight()/2, Color(255,255,255));
    }

    if(isEngaged)
    {
        //Input* in = Input::getInstance();
        //if(in == NULL)
          //  return;

        if(isMoused)
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(*this,Color(220,220,220));//Down
                getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
                getSystem()->getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,Color(0,0,128));
            }
            else
                image->draw(getX(),getY(),1);

            //font->drawCentered(caption,Rectangle(getX()+1,getY()+1,getWidth(),getHeight()),0,0,0);
            font->draw(caption,getX(),getY(),Color(0,0,0));
            if(icon != NULL)
                icon->draw((getX()+getWidth()/2)-icon->getWidth()/2+1, (getY()+getHeight()/2) - icon->getHeight()/2+1, Color(255,255,255));
        }
        else
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(*this,Color(255,255,255));//Up
                getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
                getSystem()->getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,Color(0,0,255));
            }
            else
                image->draw(getX(),getY(),2);

            //font->drawCentered(caption,Rectangle(getX()-1,getY()-1,getWidth(),getHeight()),0,0,0);
            font->draw(caption,getX(),getY(),Color(0,0,0));
            if(icon != NULL)
                icon->draw((getX()+getWidth()/2)-icon->getWidth()/2 -1, (getY()+getHeight()/2) - icon->getHeight()/2 -1,Color(255,255,255));
        }
    }

    if(this->icon != NULL)
        this->icon->draw(getX(), getY(), Color(255,255,255));
        //this->icon->draw((getX()+getWidth()/2)-icon->getWidth()/2, (getY()+getHeight()/2) - icon->getHeight()/2, Color(255,255,255));
}

void Gui::Button::update()
{
    /*Input* in = Input::getInstance();
    if(in == NULL)
        return;*/

    fired = false;

    if(!getInput()->mouseDown(MOUSE_LEFT) && isEngaged)
    {
        isEngaged = false;

        if(isMoused)
            execute();
    }

    isMoused = false;
}

bool Gui::Button::isFired()
{
   bool ret = fired;

   fired = false;

   return ret;
}

void Gui::Button::mouseLeftHit(int mouseX, int mouseY)
{
    isEngaged = true;
}

void Gui::Button::mouseOver(int mouseX, int mouseY)
{
    isMoused = true;
}

Gui::XButton::XButton() : Button("X",NULL,0,0,20,20)
{

}

void Gui::XButton::render()
{
    if(!isEnabled)
    {
        getSystem()->getGraphics()->fillRect(*this,Color(0,255,0,255));//,Color(0,0,0,255));
        getSystem()->getGraphics()->fillRect(*this,Color(220,220,220,128));
        getSystem()->getGraphics()->drawRect(*this,Color(128,128,128));

        //font->drawCentered(caption,*this,128,128,128);
        font->draw(caption,getX(),getY(),Color(128,128,128));
        return;
    }

    if(!isEngaged)
    {
        //Input* in = Input::getInstance();
        //if(in == NULL)
          //  return;

        if(!isMoused)
        {
            //getGraphics()->fillRect(*this,220,220,220);
            getSystem()->getGraphics()->fillRect(*this,Color(0,255,0,255));//Color(0,0,0,255));
            getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
            //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,255,255,255);
            //font->drawCentered(caption,Rectangle(getX(),getY(),getWidth(),getHeight()),255,255,255);
            font->draw(caption, getX(), getY(), Color(255,255,255));
            return;
        }
        else
        {
            getSystem()->getGraphics()->fillRect(*this,Color(0,255,0,255));//,Color(128,128,128,255));

            getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
            //font->drawCentered(caption,Rectangle(getX()-1,getY()-1,getWidth(),getHeight()),255,255,255);
            font->draw(caption,getX(),getY(),Color(255,255,255));
        }
    }

    if(isEngaged)
    {
        /*Input* in = Input::getInstance();
        if(in == NULL)
            return;*/

        if(isMoused)
        {
            getSystem()->getGraphics()->fillRect(*this,Color(128,255,128,255));//,Color(0,0,0,255));
            getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
            //font->drawCentered(caption,Rectangle(getX()+1,getY()+1,getWidth(),getHeight()),255,255,255);
            font->draw(caption,getX(),getY(),Color(255,255,255));
        }
        else
        {
            getSystem()->getGraphics()->fillRect(*this,Color(0,255,0,255));//,Color(128,128,128,255));
            getSystem()->getGraphics()->drawRect(*this,Color(0,0,0));
            //font->drawCentered(caption,Rectangle(getX()-1,getY()-1,getWidth(),getHeight()),255,255,255);
            font->draw(caption,getX(),getY(),Color(255,255,255));
        }
    }
}

void Gui::XButton::update()
{
    int x = parent->getX()+parent->getWidth() - getWidth()-2;
    setPos(x,parent->getY()-1);

    //Input* in = Input::getInstance();
    //if(in == NULL)
      //  return;

    if(!getInput()->mouseDown(MOUSE_LEFT) && isEngaged)
    {
        isEngaged = false;

        if(isMoused)
            execute();
    }

    isMoused = false;
}

void Gui::XButton::execute()
{
    getParent()->getParent()->close();
}

Gui::PopUpWindow::PopUpWindow(char cap[], int x, int y, int width, int height) : PanelWindow(cap,x,y,width,height)
{
    bar = new Bar(cap);
    addChild(bar);
    bar->addChild(new XButton());
    //addChild(new Button("Byte me!", NULL, x+100,y+100,75,40));
    show();
}

Gui::Text::Text(char cap[], int x, int y, int width, int height, int f) : Window(cap,x,y,width,height)
{
    pinDown(true);
    flags = f;
    cursor = strlen(cap);
}

void Gui::Text::render()
{
    int maxCursor;

    if(flags& TB_EDIT)
        getSystem()->getGraphics()->fillRect((*this),Color(255,255,255));

    if(flags & TB_CENTERED)
        font->draw(caption,getX(),getY(),Color(0,0,0));
        //maxCursor = font->drawCentered(caption, (Rectangle(getX()+5, getY()+5, getWidth()-10, getHeight()-10)), 0,0,0);
    else
        font->draw(caption,getX(),getY(),Color(0,0,0));
        //maxCursor = font->draw(caption, (Rectangle(getX()+5, getY()+5, getWidth()-10, getHeight()-10)), 0,0,0);

    maxCursor = 0;

    if(flags & TB_OUTLINE)
        getSystem()->getGraphics()->drawRect((*this),Color(0,0,0));

    if(flags & TB_EDIT)
    {
        getSystem()->getGraphics()->drawRect((*this),Color(0,0,0));

        if(gui->getFocus() == this)
        {
            //int x1 = getX()+5+cursor%(getWidth()/font->getCharWidth())*font->getCharWidth();
            //int y1 = getY()+5+cursor/(getWidth()/font->getCharWidth())*font->getCharHeight();
            int x1 = getX()+maxCursor/(((int)getWidth()-10)/font->getCharWidth())*font->getCharWidth()*-1 - 5;
            int y1 = 5+getY()+maxCursor%(((int)getWidth()-10)/font->getCharWidth())*font->getCharHeight()*-1;

            getSystem()->getGraphics()->drawLine(x1,y1,x1,y1+font->getCharHeight(),Color(0,0,0));
        }
    }
}

void Gui::Text::update()
{
    if(flags & TB_EDIT && gui->getFocus() == this)
    {
        //Input* in = Input::getInstance();

        //if(in == NULL)
          //  return;

        // int key = system->getChar();
        int key  = 69;

        if(key != -1)
        {
            char buffer[4] = " ";
            buffer[0] = key;
            if(getInput()->keyHit(key))
                strcat(caption,buffer);

            cursor = strlen(caption);
        }
    }
}

Gui::ImageBox::ImageBox(Image* i, int x, int y) : Window(" ",x,y,10,10)
{
    image = i;

    pinDown(true);

    if(i!=NULL)
        setSize(i->getWidth(), i->getHeight());
}

void Gui::ImageBox::setImage(Image* i)
{
    image = i;

    if(i!=NULL)
        setSize(i->getWidth(), i->getHeight());
}

void Gui::ImageBox::render()
{
    if(image != NULL)
    {
        image->draw(getX(),getY(),Color(255,255,255));
        getSystem()->getGraphics()->drawRect((*this), Color(0,0,0));
    }
}

Gui::CheckBox::CheckBox(char caption[], int x, int y, int width, int height) : Gui::Button(caption,x,y,width,height)
{
    checked = false;
}

void Gui::CheckBox::execute()
{
    checked = !checked;
}

void Gui::CheckBox::render()
{
    Rect box(getX(),(getY()+getHeight()/2) - font->getCharHeight()/2,font->getCharHeight(),font->getCharHeight());
    Rect textBox(getX()+ box.getWidth()+4, getY() + font->getCharHeight()/2, getWidth()-box.getWidth()*2, getHeight());
    if(!isEnabled)
    {
        if(image == NULL)
        {
            getSystem()->getGraphics()->fillRect(box,Color(220,220,220));//Disabled
            getSystem()->getGraphics()->drawRect(box,Color(128,128,128));
            //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,255,255,255);
        }
        else
            image->draw(getX(),getY(),3);

        //font->draw(caption,textBox,Color(128,128,128));
        font->draw(caption,getX(),getY(),Color(128,128,128));
        if(icon != NULL)
            icon->draw((textBox.getX()+textBox.getWidth()/2)-icon->getWidth()/2, (textBox.getY()+textBox.getHeight()/2) - icon->getHeight()/2, Color(255,255,255,127));
        return;
    }

    if(!isEngaged)
    {
        //Input* in = Input::getInstance();
        //if(in == NULL)
          //  return;

        if(!isMoused)
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(box,Color(220,220,220));//Normal
                getSystem()->getGraphics()->drawRect(box,Color(0,0,0));
                //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,255,255,255);
            }
            else
                image->draw(getX(),getY(),0);

            //font->draw(caption,textBox,Color(0,0,0));
            font->draw(caption, getX(), getY(), Color(0,0,0));

            if(isChecked())
            {
                getSystem()->getGraphics()->fillRect(Rect(box.getX()+2, box.getY()+2, box.getWidth()-4, box.getHeight()-4),Color(0,0,0));
            }
        }
        else
        {
            if(image==NULL)
            {
                getSystem()->getGraphics()->fillRect(box,Color(255,255,255));//Up
                getSystem()->getGraphics()->drawRect(box,Color(0,0,255));
                //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,0,0,255);
            }
            else
                image->draw(getX(),getY(),2);

            //font->draw(caption,textBox,0,0,255);
            font->draw(caption,getX(),getY(),Color(0,0,255));
            if(isChecked())
            {
                getSystem()->getGraphics()->fillRect(Rect(box.getX()+2, box.getY()+2, box.getWidth()-4, box.getHeight()-4),Color(0,0,255));
            }
        }

        if(icon != NULL)
            icon->draw((textBox.getX()+textBox.getWidth()/2)-icon->getWidth()/2, (textBox.getY()+textBox.getHeight()/2) - icon->getHeight()/2, Color(255,255,255));
    }

    if(isEngaged)
    {
        //Input* in = Input::getInstance();
        //if(in == NULL)
          //  return;

        if(isMoused)
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(box,Color(255,255,255));//Down
                getSystem()->getGraphics()->drawRect(box,Color(0,0,255));
                //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,0,0,128);
            }
            else
                image->draw(getX(),getY(),1);

            //font->draw(caption,textBox,0,0,0);
            font->draw(caption,getX(),getY(),Color(0,0,0));
            if(isChecked())
            {
                getSystem()->getGraphics()->fillRect(Rect(box.getX()+2, box.getY()+2, box.getWidth()-4, box.getHeight()-4),Color(0,0,255));
            }

            if(icon != NULL)
                icon->draw((textBox.getX()+textBox.getWidth()/2)-icon->getWidth()/2+1, (textBox.getY()+textBox.getHeight()/2) - icon->getHeight()/2+1,Color(255,255,255));
        }
        else
        {
            if(image == NULL)
            {
                getSystem()->getGraphics()->fillRect(box,Color(255,255,255));//Up
                getSystem()->getGraphics()->drawRect(box,Color(0,0,255));
                //getGraphics()->drawRect(getX()+1,getY()+1,getWidth()-2,getHeight()-2,0,0,255);
            }
            else
                image->draw(getX(),getY(),2);

            //font->draw(caption,textBox,0,0,255);
            font->draw(caption,getX(),getY(),Color(0,0,255));
            if(isChecked())
            {
                getSystem()->getGraphics()->fillRect(Rect(box.getX()+2, box.getY()+2, box.getWidth()-4, box.getHeight()-4),Color(0,0,255));
            }

            if(icon != NULL)
                icon->draw((textBox.getX()+textBox.getWidth()/2)-icon->getWidth()/2 -1, (textBox.getY()+textBox.getHeight()/2) - icon->getHeight()/2 -1,Color(255,255,255));
        }
    }
}

Gui::VScrollBar::VScrollBar(int x, int y, int height) : Window("VScrollBar",x,y,20,height)
{
    if(height < 40)
        setHeight(40);

    sliderSize = height - 40;
    sliderSize = 10;
    sliderY = 0;

    up = new Gui::Button("^",x,y,20,20);
    down = new Gui::Button("v",x,y+height-20,20,20);
    addChild(up);
    addChild(down);
    setScale(.5);
    increments = 1;
    setIncrements(5);
    locked = false;
    lockedY = 0;
}

void Gui::VScrollBar::update()
{
    if(up->isFired())
        sliderY-=increments;

    if(down->isFired())
        sliderY+=increments;

    /*if(sliderY < 0)
        sliderY = 0;

    if(sliderY+sliderSize > getHeight()-40)
        sliderY = getHeight()-sliderSize-40;*/

    if(contains(getInput()->getMouseX(), getInput()->getMouseY()) && getInput()->mouseHit(MOUSE_LEFT))
    {
        if(getInput()->getMouseY() > getY()+sliderY+20 && getInput()->getMouseY() < getY()+sliderY + sliderSize+20)
        {
            locked = true;
            lockedY = getInput()->getMouseY() - getY() - 20 - sliderY;
        }
        else if(getInput()->getMouseY() > getY()+20 && getInput()->getMouseY() < getY()+getHeight()-20)
        {
            sliderY = getInput()->getMouseY()-getY()-sliderSize/2-20;
            locked = true;
            lockedY = sliderSize/2;
        }
    }

    if(!getInput()->mouseDown(MOUSE_LEFT))
        locked = false;

    if(locked)
    {
        sliderY = getInput()->getMouseY()-getY()-20-lockedY;
    }

    if(sliderY < 0)
        sliderY = 0;

    if(sliderY+sliderSize > getHeight()-40)
        sliderY = getHeight()-sliderSize-40;
}

void Gui::VScrollBar::render()
{
    getSystem()->getGraphics()->fillRect((*this),Color(255,255,255));
    getSystem()->getGraphics()->drawRect((*this),Color(0,0,0));
    getSystem()->getGraphics()->drawRect(getX()+1,getY()+sliderY+20,18,sliderSize,Color(0,0,0));
    char buffer[1000];
    sprintf(buffer,"%d",(int)(getValue()*1000));
    font->draw(buffer,getX()+25,getY(),Color(0,0,0));
}

Scalar Gui::VScrollBar::getValue()
{
    return sliderY/(getHeight()-40-sliderSize);
}

void Gui::VScrollBar::setValue(Scalar s)
{
    sliderY = (int)(s*getHeight()-40-sliderSize);
}

Scalar Gui::VScrollBar::setScale(Scalar s)
{
    if(s <= 0 || s > 1)
        s = 1;
    sliderSize = (getHeight()-40)*s;
}

Gui::HScrollBar::HScrollBar(int x, int y, int height) : Window("HScrollBar",x,y,height,20)
{
    if(height < 40)
        setHeight(40);

    sliderSize = height - 40;
    sliderSize = 10;
    sliderY = 0;

    up = new Gui::Button("<",x,y,20,20);
    down = new Gui::Button(">",x+height-20,y,20,20);
    addChild(up);
    addChild(down);
    setScale(.5);
    increments = 1;
    setIncrements(5);
    locked = false;
    lockedY = 0;
}

void Gui::HScrollBar::update()
{
    if(up->isFired())
        sliderY-=increments;

    if(down->isFired())
        sliderY+=increments;

    if(sliderY < 0)
        sliderY = 0;

    if(sliderY+sliderSize > getWidth()-40)
        sliderY = getWidth()-sliderSize-40;

    if(contains(getInput()->getMouseX(),getInput()->getMouseY())&&getInput()->mouseHit(MOUSE_LEFT))
    {
        if(getInput()->getMouseX() > getX()+sliderY+20 && getInput()->getMouseX() < getX()+sliderY + sliderSize+20)
        {
            locked = true;
            lockedY = getInput()->getMouseX() - getX() - 20 - sliderY;
        }
        else if(getInput()->getMouseX() > getX()+20 && getInput()->getMouseX() < getX()+getWidth()-20)
        {
            sliderY = getInput()->getMouseX()-getX()-sliderSize/2-20;
            locked = true;
            sliderY = sliderSize/2;
        }
    }

    if(!getInput()->mouseDown(MOUSE_LEFT))
        locked = false;

    if(locked)
    {
        sliderY = getInput()->getMouseX()-getX()-20-lockedY;
    }

    if(sliderY < 0)
        sliderY = 0;

    if(sliderY+sliderSize > getWidth()-40)
        sliderY = getWidth()-sliderSize-40;
}

void Gui::HScrollBar::render()
{
    getSystem()->getGraphics()->fillRect((*this),Color(255,255,255));
    getSystem()->getGraphics()->drawRect((*this),Color(0,0,0));
    getSystem()->getGraphics()->drawRect(getX()+sliderY+20,getY()+1,sliderSize,18,Color(0,0,0));
    char buffer[1000];
    sprintf(buffer,"%d",(int)(getValue()*1000));
    font->draw(buffer,getX()+100,getY(),Color(0,0,0));
}

Scalar Gui::HScrollBar::getValue()
{
    return sliderY/(getWidth()-40-sliderSize);
}

void Gui::HScrollBar::setValue(Scalar s)
{
    sliderY = (int)(s*getWidth()-40-sliderSize);
}

Scalar Gui::HScrollBar::setScale(Scalar s)
{
    if(s <= 0 || s > 1)
        s = 1;
    sliderSize = (getWidth()-40)*s;
}

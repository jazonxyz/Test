#include "Graphics.h"
#include "glext.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////////////
//Graphics////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Graphics::init(int aWidth, int aHeight)
{
    width = aWidth;
    height = aHeight;
    if(height < 1) height = 1;
    //float ratio = (float)width/(float)height;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    cam.setPos(0,0);//,Vector(0,0,-1),Vector(0,1,0));//Fix this
    cam.setSize(width,height);

    glEnable(GL_TEXTURE_2D);

    resize(width,height);

    isCamEnabled = false;
    glEnable(GL_MULTISAMPLE);

    glLoadIdentity();
    glTranslated(0.375f,0.375f,0);
}

void Graphics::resize(int aWidth, int aHeight)
{
    width = aWidth;
    height = aHeight;

    glMatrixMode(GL_PROJECTION);//Projection :)
    glLoadIdentity();
    glOrtho(0,getWidth2D(), getHeight2D(), 0,0,1);
    glMatrixMode(GL_MODELVIEW);
}

void Graphics::clear(Scalar r, Scalar g, Scalar b, Scalar a)
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::flip()//Zoom might not work but fuck it
{
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,getWidth2D()*cam.getZoom().getX(), getHeight2D()*cam.getZoom().getY(), 0,0,1);//Zoom, you know
    glMatrixMode(GL_MODELVIEW);*/
    FLIP_BUFFER;
}

void Graphics::enableCam(bool c)
{
    if(c && !isCamEnabled)
    {
        glPushMatrix();

        Camera* cam = getCamera();
        glTranslated(0-cam->getX(), 0-cam->getY(),0);
        glTranslated(cam->getCenterX(),cam->getCenterY(),0);
        glRotated(cam->getRot(),0,0,-1);
        glTranslated(0-cam->getCenterX(),0-cam->getCenterY(),0);
        glScaled(cam->getZoom(),cam->getZoom(),1);

        isCamEnabled = true;
    }

    if(!c && isCamEnabled)
    {
        glPopMatrix();
        isCamEnabled = false;
    }
}

void Graphics::drawPoint(int x, int y, Color c)
{
    glPushMatrix();
    glLoadIdentity();
    //glScalef(ScaleX,ScaleY,1.0f);
    glTranslatef(0.375f,0.375f,0);
    //DrawStuff
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_POINTS);
        glColor4d(c.GetR(),c.GetG(),c.GetB(),c.GetA());
        glVertex2d(x,y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Graphics::drawLine(Scalar x, Scalar y, Scalar x2, Scalar y2, Color c)
{
    glPushMatrix();

    Scalar width = x2-x;

    Scalar height = (y2-y);

    glTranslated(x+width/2, y+height/2,0);

    //DrawStuff
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
        glColor4d(c.GetR(),c.GetG(),c.GetB(),c.GetA());
        glVertex2d(0-width/2,0-height/2);
        glVertex2d(width/2,height/2);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Graphics::drawRect(Rect r, Color c)
{
    drawLine(r.getX(),r.getY(),r.getX2(),r.getY(),c);
    drawLine(r.getX2(),r.getY(),r.getX2(),r.getY2(),c);
    drawLine(r.getX(),r.getY2(),r.getX2()+1/getCamera()->getZoom(),r.getY2(),c);
    drawLine(r.getX(),r.getY(),r.getX(),r.getY2(),c);
}

void Graphics::drawRect(int x, int y, int width, int height, Color c)
{
    drawRect(Rect(x,y,width,height),c);
}

void Graphics::fillRect(Rect r, Color c)
{
    glPushMatrix();

    glTranslated(r.getX()+(Scalar)r.getWidth()/2,r.getY()+(Scalar)r.getHeight()/2,0);
    //DrawStuff
    glRotated(r.getRot(),0,0,-1);

    int w = (int)r.getWidth();
    int h = (int)r.getHeight();

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glColor4d(c.GetR(),c.GetG(),c.GetB(),c.GetA());
        glVertex2d(-.5*w,-.5*h);
        glVertex2d(-.5*w,.5*h);
        glVertex2d(.5*w,.5*h);
        glVertex2d(.5*w,-.5*h);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}

void Graphics::fillRect(int x, int y, int width, int height, Color c)
{
    fillRect(Rect(x,y,width,height),c);
}

void Graphics::fillRect(Rect r, Color c1, Color c2, Color c3, Color c4)
{
    glPushMatrix();

        glTranslated(r.getX()+(Scalar)r.getWidth()/2+0.375f,r.getY()+(Scalar)r.getHeight()/2+0.375f,0);
        //DrawStuff
        glRotated(r.getRot(),0,0,-1);

        int w = (int)r.getWidth();
        int h = (int)r.getHeight();

        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glColor4d(c1.GetR(),c1.GetG(),c1.GetB(),c1.GetA());
            glVertex2d(-.5*w,-.5*h);
            glColor4d(c3.GetR(),c3.GetG(),c3.GetB(),c3.GetA());
            glVertex2d(-.5*w,.5*h);
            glColor4d(c4.GetR(),c4.GetG(),c4.GetB(),c4.GetA());
            glVertex2d(.5*w,.5*h);
            glColor4d(c2.GetR(),c2.GetG(),c2.GetB(),c2.GetA());
            glVertex2d(.5*w,-.5*h);
        glEnd();
        glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}

/*void Graphics::EnableQuick2D(bool q2d)//Move to init
{

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,GetWidth2D(), GetHeight2D(), 0,0,1);
        glMatrixMode(GL_MODELVIEW);

}*/

//////////////////////////////////////////////////////////////////////////////
//Image///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Image::Image() : Resource("None")
{
    width = height = frameWidth = frameHeight = texSize = -1;
}

Image::Image(char Name[], char FileName[], System* sys) : Resource(Name)
{
    load(FileName, sys);
}

Image::Image(char Name[], char FileName[], System* sys, int aFrameWidth, int aFrameHeight) : Resource(Name)
{
    load(FileName,sys,aFrameWidth, aFrameHeight);
}

void Image::load(char fileName[], System* sys)
{
    system = sys;
    unsigned char* data = sys->loadImageData(fileName, &width, &height, &texSize);

    if(data == NULL) return;

    glGenTextures(1,&imageTag);
    glBindTexture(GL_TEXTURE_2D, imageTag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST_MIPMAP_NEAREST);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);//image->pixels);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texSize, texSize, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSize,texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    strcpy(imageName, fileName);
    frameWidth = -1;
    frameHeight = -1;
    setFileName(fileName);
}

void Image::load(char fileName[], System* sys, int aFrameWidth, int aFrameHeight)
{
    load(fileName,sys);

    frameWidth = aFrameWidth;
    frameHeight = aFrameHeight;
}

void Image::setFrameSize(int aFrameWidth,int aFrameHeight)
{
    if(aFrameWidth < 1 || aFrameHeight < 1)
        return;

    frameWidth = aFrameWidth;
    frameHeight = aFrameHeight;
}

Vector Image::tex(Scalar u, Scalar v)
{
    glTexCoord2f(u*((Scalar)(width-1)/(texSize-1)),v*((Scalar)(height-1)/(texSize-1)));
    return Vector(u*((Scalar)width/(texSize-1)),v*((Scalar)height/(texSize-1)),0);
}

void Image::draw(int x, int y, Color c)
{
    draw(x,y,0,0,width,height,1.0,1.0,0.0f,c);
}

void Image::draw(int x, int y, Scalar ScaleX, Scalar ScaleY, Scalar Rot, Color c)
{
    draw(x,y,0,0,width,height,ScaleX,ScaleY,Rot,c);
}

void Image::draw(int x, int y, int x1, int y1, int w, int h, Scalar ScaleX, Scalar ScaleY, Scalar Rot, Color c)
{
    glPushMatrix();

        glTranslated(x+(Scalar)w/2,y+(Scalar)h/2,0);
        //DrawStuff
        glRotated(Rot,0,0,-1);

        glBindTexture(GL_TEXTURE_2D, imageTag);
        glBegin(GL_QUADS);
            glColor4d(c.GetR(),c.GetG(),c.GetB(),c.GetA());
            //tex((Scalar)x1/width,(Scalar)y1/height);
            glTexCoord2d((Scalar)(x1+0.375f)/texSize,Scalar(y1+0.375f)/texSize);
            glVertex2d(-.5*w*ScaleX,-.5*h*ScaleY);
            //tex((Scalar)x1/width,(Scalar)(y1+h)/height);
            glTexCoord2d((Scalar)(x1+0.357f)/texSize,(Scalar)(y1+h-0.625f)/texSize);
            glVertex2d(-.5*w*ScaleX,.5*h*ScaleY);
            //tex((Scalar)(x1+w)/width,(Scalar)(y1+h)/height);
            glTexCoord2d(((Scalar)x1+w-0.625f)/texSize,(Scalar)(y1+h-0.625f)/texSize);
            glVertex2d(.5*w*ScaleX,.5*h*ScaleY);
            //tex((Scalar)(x1+w)/width,(Scalar)y1/height);
            glTexCoord2d(((Scalar)x1+w-0.625f)/texSize,(Scalar)(y1+0.375f)/texSize);
            glVertex2d(.5*w*ScaleX,-.5*h*ScaleY);
        glEnd();

    glPopMatrix();
}

void Image::draw(int x, int y, int frame, Scalar ScaleX, Scalar ScaleY, Scalar Rot, Color c)
{
    int x1 = frame%(width/frameWidth);
    int y1 = frame/(width/frameWidth);

    draw(x,y,x1*frameWidth,y1*frameHeight,frameWidth,frameHeight,ScaleX,ScaleY,Rot,c);
}

int Image::getWidth()
{
    return width;
}

int Image::getHeight()
{
    return height;
}

int Image::getFrameWidth()
{
    return frameWidth;
}

int Image::getFrameHeight()
{
    return frameHeight;
}

void Image::free()
{
    width = height = frameWidth = frameHeight = -1;
    glDeleteTextures(1,&imageTag);
}

//////////////////////////////////////////////////////////////////////////////
//Camera//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void Camera::moveX(Scalar d)
{
    //Vector v = look;
    //v.SetLength(d);
    //v.Rotate(-90, up);
    //pos = pos + v;
    setX(getX()+d);
}

void Camera::moveY(Scalar d)
{
    //Vector v = up;
    //v.SetLength(d);
    //pos = pos + v;
    Scalar y = getY();
    setY(getY()+d);
    y = getY();
}

void Camera::rotate(Scalar a)//fix for 2d
{
    setRot(getRot()+a);
}

void Camera::setZoom(Scalar z)
{
    Scalar centerx = getTotalX()+(getTotalWidth()-1)/2;
    Scalar centery = getTotalY()+(getTotalHeight()-1)/2;

    if(z <= 0)
        return;

    zoomIn = z;

    //zoomIn+=z;

    //if(zoomIn < 0)
      //  zoomIn = 0.0001;

    setX((centerx-(getTotalWidth()-1)/2)*zoomIn);
    setY((centery-(getTotalHeight()-1)/2)*zoomIn);
}

void Camera::zoom(Scalar z)
{
    if(z == 0)
        return;

    Scalar centerx = getTotalX()+(getTotalWidth()-1)/2;
    Scalar centery = getTotalY()+(getTotalHeight()-1)/2;

    zoomIn*=z;

    //zoomIn+=z;

    //if(zoomIn < 0)
      //  zoomIn = 0.0001;

    //setX((centerx-(getTotalWidth()-1)/2)*zoomIn);
    //setY((centery-(getTotalHeight()-1)/2)*zoomIn);
    centerAt(centerx, centery);
}

void Camera::centerAt(Scalar x, Scalar y)
{
    setX((x-(getTotalWidth()-1)/2)*zoomIn);
    setY((y-(getTotalHeight()-1)/2)*zoomIn);
}

//////////////////////////////////////////////////////////////////////////////
//Font////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Font::Font() : Resource("None")
{
}

Font::Font(char Name[], char FileName[], System* sys) : Resource(Name)
{
    load(FileName,sys);
}

void Font::load(char fileName[], System* system)
{
    image.load(fileName, system);

    charWidth = image.getWidth()/16;
    charHeight = image.getHeight()/16;

    sys = system;

    image.setFrameSize(charWidth, charHeight);
    setFileName(fileName);
}

void Font::draw(char message[],int x, int y, Scalar ScaleX, Scalar ScaleY ,Scalar rot, Color c)
{
    if(rot != 0)
    {
        Vector v(1,0);

        v.rotate(rot);//,Vector(0,0,-1));

        for(int i = 0; i < strlen(message); i++)
        {
            v.setLength((int)((Scalar)x+i*(charWidth)*ScaleX));
            image.draw(x+v.getX(),y+v.getY(),message[i],ScaleX,ScaleY,rot,c);
        }
    }
    else
    {
        for(int i = 0; i < strlen(message); i++)
        {
            image.draw((int)((Scalar)x+i*(charWidth)*ScaleX),y,message[i],ScaleX,ScaleY,0,c);
        }
    }
}

void Font::draw(char message[],int x, int y, Color c)
{
    draw(message, x, y, 1, 1, 0, c);
}

void Font::free()
{
    image.free();
}


//////////////////////////////////////////////////////////////////////////////
//Map/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Map::Map(char name[], char fileName[], System *sys) : Resource(name)
{
    load(fileName,sys);
    system = sys;
}

void Map::create(char imageName[], System* sys, int aWidth, int aHeight,int aTileWidth, int aTileHeight)
{
     free();
    system = sys;
     width = aWidth;
     height = aHeight;
     tileWidth = aTileWidth;
     tileHeight = aTileHeight;

     for(int i = 0; i < (width*height); i++)
     {
         layer1.push_back(0);
         layer2.push_back(0);
         solidLayer.push_back(0);
     }

     image.load(imageName,sys,tileWidth,tileHeight);
}

bool Map::load(char fileName[], System* sys)
{
    setFileName(fileName);
     free();
    system = sys;
     char mapName[1024] = "";
     char imageName[1024] = "";

     strcpy(mapName,fileName);
     strcat(mapName,".mp");

     strcpy(imageName,fileName);
     strcat(imageName,".bmp");

     std::ifstream in(mapName, std::ios::binary);

     if(in.good())
     {

         in.read((char*)&width, sizeof(int));
         in.read((char*)&height, sizeof(int));
         in.read((char*)&tileWidth, sizeof(int));
         in.read((char*)&tileHeight, sizeof(int));

         int var;

         for(int i = 0; i < (width*height); i++)
         {
             in.read((char*)&var,sizeof(int));
             layer1.push_back(var);

             in.read((char*)&var,sizeof(int));
             layer2.push_back(var);

             in.read((char*)&var,sizeof(int));
             solidLayer.push_back(var);
         }

         image.load(imageName, sys,tileWidth,tileHeight);
     }

     in.close();
}

void Map::setTile1(int tile, int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return;

    layer1[index] = tile;
}

void Map::setTile2(int tile, int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return;

    layer2[index] = tile;
}

void Map::setSolid(int tile, int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return;

    solidLayer[index] = tile;
}


void Map::draw1(int x, int y)
{
     if(width <0 || height < 0 || tileWidth < 0 || tileHeight < 0)
         return;

     int index;
     int startX = -(x/tileWidth);
     int endX = startX + (system->getGraphics()->getCamera()->getTotalWidth()/tileWidth)+1;

     int startY = -(y/tileHeight);

     if(startY < 0)
         startY = 0;

     if(startX < 0)
         startX = 0;

     int endY = startY + (system->getGraphics()->getCamera()->getTotalHeight()/tileHeight)+2;

     /*if(system->getGraphics()->getCamera()->getRot() != 0)
     {
        Scalar cWidth = system->getGraphics()->getCamera()->getTotalWidth();
        Scalar cHeight = system->getGraphics()->getCamera()->getTotalHeight();
        Scalar size = cWidth;
        Scalar tileSize = getTileWidth();

        if(tileSize < getTileHeight())
            tileSize = getTileHeight();

        if(cHeight > size)
            size = cHeight;

        startY = startX = (totalSize/tileSize)+2;
        endY = height;
        endX = width;
     }*/

     for(int drawY = startY; drawY < endY && drawY < height; drawY++)
     {
         for(int drawX = startX; drawX <= endX && drawX < width; drawX++)
         {
             index = layer1[(drawX)+(drawY)*width] -1;

             if(index != -1)
             {
                 image.draw((drawX*tileWidth), (drawY*tileHeight),index,1,1,0,Color(1,1,1));
             }
         }
     }
}

void Map::draw2(int x, int y)
{
     if(width <0 || height < 0 || tileWidth < 0 || tileHeight < 0)
         return;

     int index;
     int startX = -(x/tileWidth);
     int endX = startX + (system->getGraphics()->getCamera()->getTotalWidth()/tileWidth)+1;

     int startY = -(y/tileHeight);

     if(startY < 0)
         startY = 0;

     if(startX < 0)
         startX = 0;

     int endY = startY + (system->getGraphics()->getCamera()->getTotalHeight()/tileHeight)+2;

     for(int drawY = startY; drawY < endY && drawY < height; drawY++)
     {
         for(int drawX = startX; drawX <= endX && drawX < width; drawX++)
         {
             index = layer2[(drawX)+(drawY)*width] -1;

             if(index != -1)
             {
                 image.draw((drawX*tileWidth), (drawY*tileHeight),index,1,1,0,Color(1,1,1));
             }
         }
     }
}

int Map::getTile1(int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return 0;
    else
        return layer1[index];
}

int Map::getTile2(int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return 0;
    else
        return layer2[index];
}

int Map::getSolid(int x, int y)
{
    int index = x+y*width;

    if(x >= width || x < 0 || y >= height || y < 0)
        return 0;
    else
    {
        return solidLayer[index];
    }
}

int Map::getWidth()
{
     return width;
}

int Map::getHeight()
{
     return height;
}

int Map::getTileWidth()
{
     return tileWidth;
}

int Map::getTileHeight()
{
     return tileHeight;
}


void Map::free()
{
     image.free();
     layer1.clear();
     layer2.clear();
     solidLayer.clear();
     width = height = tileWidth = tileHeight = 0;
}

bool Map::rectCollides(int code, Rect r)
{
    return rectOverlaps(code, Rect(r.getX()-1,r.getY()-1,r.getWidth()+2,r.getHeight()+2));
}

bool Map::rectOverlaps(int code, Rect r)//Basically, this function checks all the points in the rectangle
{                                       //So that if a tile was to be overlapping the rectangle, at least one
                                        //of the points is sure to be contained in the tile.
                                        //Points are checked in a grid the with each square of the grid
                                        //being tile sized and encompassing the entire rectangle
    int stepsX = r.getWidth()/tileWidth;
    int stepsY = r.getHeight()/tileHeight;

    /*if(r.getWidth() == 1)
    {
        for(int i = 0; i <= stepsY; i++)
        {
            int y = r.getY()-4;//(r.getY()+i*tileHeight);
            int x = r.getX()-4;
            system->getGraphics()->drawPoint(x,y,Color(0,0,0));
            if(getSolid(x/tileWidth,y/tileHeight) == code)
                return true;
        }

        system->getGraphics()->drawPoint(r.getX(),r.getY2(),Color(0,0,0));

        if(getSolid(r.getX()/tileWidth, r.getY2()/tileHeight))
            return true;

        return false;
    }*/

    for(int y1 = 0; y1 <= stepsY; y1++) //Check all the points on the top, left, and center
        for(int x1 = 0; x1 <= stepsX; x1++)
        {
            int x = (r.getX()+x1*tileWidth);
            int y = (r.getY()+y1*tileHeight);

            if(y1 > 0)
                y--;

            if(x1 > 0)
                x--;

            //system->getGraphics()->drawPoint(x,y,Color(0,0,0));
            if(getSolid(x/tileWidth,y/tileHeight) == code)
                return true;
        }

    for(int y1 = 0; y1 <= stepsY; y1++)  //Check all points on the right
    {
        int x = r.getX2();
        int y = (r.getY()+(y1*tileHeight));
        if(y1 > 0)
            y--;
        //system->getGraphics()->drawPoint(x,y,Color(0,0,0));
        if(getSolid(x/tileWidth, y/tileHeight) == code)
            return true;
    }

    for(int x1 = 0; x1 <= stepsX; x1++)  //Check all points on the bottom
    {
        int x = (r.getX()+x1*tileWidth);
        int y = r.getY2();

        if(x1 > 0)
            x--;

        //system->getGraphics()->drawPoint(x,y,Color(0,0,0));//Debuggin purposes

        if(getSolid(x/tileWidth, y/tileHeight) == code)
            return true;
    }


    //system->getGraphics()->drawPoint(r.getX2(),r.getY2(),Color(0,0,0));//Debuggin purposes
    if(getSolid(r.getX2()/tileWidth, r.getY2()/tileHeight) == code) //Check bottom right corner
        return true;

    return false;
}


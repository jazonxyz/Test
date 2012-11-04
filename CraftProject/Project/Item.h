#ifndef ITEM_H
#define ITEM_H

#include "Core/Resources.h"
#include "Core/World.h"
#include "Entity.h"

enum{ITEM_EQUIPMENT, ITEM_MATERIAL, ITEM_CONSUMABLE};

enum{ITEM_WOOD, ITEM_TABLE};

class Inventory;

class Item
{
private:
    Image* icon;
    int type;
    int id;
public:
    Item(Image* img, int t, int i);
    ~Item(){};
    bool isStackable();
    int getType(){return type;};
    int getID(){return id;};
    Image* getImage(){return icon;};
};

class ItemFactory
{
private:
    ResourceManager resMgr;
public:
    void init(System* sys);
    void free();

    static std::vector<int> getPosCraft(Inventory* inv);
    void craft(int id, Inventory* inv);

    Image* getItemImage(int id);
    Item* makeItem(int id);
};

class Inventory
{
private:
    std::vector<Item*> items;
    int capacity;
    ItemFactory* factory;
public:
    Inventory(){capacity = 64;};
    ~Inventory(){};
    void addItem(int id);
    void addItem(Item* item);
    void removeItem(Item* item);
    void removeItem(int item);
    void clearItems();
    void setFactory(ItemFactory* f){factory = f;};
    ItemFactory* getFactory(){return factory;};

    bool containsItem(int id, int qty);

    Item* getItem(int i){if(i< 0 || i > (int)items.size()) return NULL; return items[i];};
    std::vector<Item*> getList(){return items;};

    bool isFull(){return (int)items.size() >= capacity;};
};

class SolidItem : public Entity
{
private:
    int id;
    static int type;
    Image* image;
public:
    SolidItem(Image* img, int x, int y, int i);
    ~SolidItem(){};
    void render();
    void update(){};
    int getID(){return id;};
    static int getType(){return type;};
};

class WoodItem : public Item
{
private:
public:
    WoodItem(Image* img);
    ~WoodItem(){};
};

class TableItem : public Item
{
private:
public:
    TableItem(Image* img);
    ~TableItem(){};
};

#endif

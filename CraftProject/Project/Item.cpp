#include "Item.h"

void Inventory::addItem(int id)
{
    addItem(factory->makeItem(id));
}

void Inventory::addItem(Item* item)
{
    for(int i = 0; i < (int)items.size(); i++)
    {
        if(items[i] == NULL)
        {
            items[i] = item;
            return;
        }
    }

    items.push_back(item);
}

void Inventory::removeItem(Item* item)
{
    for(int i = 0; i < (int)items.size(); i++)
    {
        if(items[i] != NULL && items[i] == item)
        {
            delete item;
            items[i] = NULL;
            //items.erase(items.begin() + i);
            return;
        }
    }
}

void Inventory::removeItem(int item)
{
    for(int i = 0; i < (int)items.size(); i++)
    {
        if(items[i] != NULL && items[i]->getID() == item)
        {
            delete items[i];
            items[i] = NULL;
            //items.erase(items.begin() + i);
            return;
        }
    }
}

bool Inventory::containsItem(int id, int qty)
{
    int quantity = 0;

    for(int i = 0; i < (int)items.size(); i++)
    {
        if(items[i] != NULL && items[i]->getID() == id)
        {
            quantity++;
        }
    }

    return quantity >= qty;
}

void Inventory::clearItems()
{
    for(int i = 0; i < (int)items.size(); i++)
        delete items[i];

    items.clear();
}

Item::Item(Image* img, int t, int i)
{
    icon = img;
    type = t;
    id = i;
}

bool Item::isStackable()
{
    if(type == ITEM_EQUIPMENT)
        return false;

    return true;
}

void ItemFactory::init(System* sys)
{
    resMgr.add(new Image("woodImage", "wood.bmp", sys));
    resMgr.add(new Image("tableImage", "table.bmp", sys));
}

void ItemFactory::free()
{
    resMgr.freeAll();
}

std::vector<int> ItemFactory::getPosCraft(Inventory* inv)
{
    std::vector<int> result;

    if(inv->containsItem(ITEM_WOOD, 2))
            result.push_back(ITEM_TABLE);

    return result;
}

void ItemFactory::craft(int id, Inventory* inv)
{
    if(id == ITEM_TABLE)
    {
        inv->removeItem(ITEM_WOOD);
        inv->removeItem(ITEM_WOOD);

        inv->addItem(makeItem(ITEM_TABLE));
    }
}

Image* ItemFactory::getItemImage(int id)
{
    if(id == ITEM_WOOD)
        return (Image*)resMgr.get("woodImage");

    if(id == ITEM_TABLE)
        return (Image*)resMgr.get("tableImage");

    return NULL;
}

Item* ItemFactory::makeItem(int id)
{
    if(id == ITEM_WOOD)
    {
        return new WoodItem((Image*)resMgr.get("woodImage"));
    }

    if(id == ITEM_TABLE)
    {
        return new TableItem((Image*)resMgr.get("tableImage"));
    }

    return NULL;
}

int SolidItem::type = WorldObject::generateType();

SolidItem::SolidItem(Image* img, int x, int y, int i) : Entity(x, y, ENTITY_ITEM)
{
    setSolid(false);
    image = img;
    setSize(25,25);
    id = i;
}

void SolidItem::render()
{
    image->draw(getX(),getY(), Color(255,255,255));
}

WoodItem::WoodItem(Image* img) : Item(img, ITEM_MATERIAL, ITEM_WOOD)
{

}

TableItem::TableItem(Image* img) : Item(img, ITEM_MATERIAL, ITEM_TABLE)
{

}

#include "Resources.h"
#include <stdio.h>
#include <string.h>

Resource::Resource()
{
    sprintf(name,"None");
}

Resource::Resource(char aname[])
{
    sprintf(name, aname);
}

void Resource::setFileName(char aFileName[])
{
    sprintf(fileName, aFileName);
}

//////////////////////////////////////////////////////////////////////////////
//ResourceManager/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void ResourceManager::add(Resource* res)
{
    resources.push_back(res);
}

void ResourceManager::remove(char name[])
{
    for(int i = 0; i < (int)resources.size(); i++)
    {
        if(!strcmp(name, resources[i]->getName()))
        {
            resources[i]->free();
            resources.erase(resources.begin() + i);
            break;
        }
    }
}

Resource* ResourceManager::get(char name[])
{
    for(int i = 0; i < (int)resources.size(); i++)
    {
        if(!strcmp(name, resources[i]->getName()))
        {
            return resources[i];
        }
    }

    return NULL;
}

void ResourceManager::freeAll()
{
    for(int i = 0; i < (int)resources.size(); i++)
    {
        resources[i]->free();
        resources.erase(resources.begin() + i);
    }
}

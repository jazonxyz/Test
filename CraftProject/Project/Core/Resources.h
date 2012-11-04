#ifndef RESOURCES_H
#define RESOURCES_H

#include <vector>

class Resource;

class ResourceManager                  //A general resource bank
{                                               //Resources have unique identifiers and if a resource is added with a name
private:                                        //that has been designated for another resource, the latter resource will be
    std::vector<Resource*> resources;           //Freed and dismissed.  FIX ME: Add error reporting to this routine.
public:
    //ResourceManager(){};
    //~ResourceManager(){};
    void add(Resource* res);                    //Supply a pointer to a resource (e.g. mgr.add(new Image("Hi.bmp","HiImage"));)
    void remove(char name[]);                   //Frees and removes the resource from the list
    Resource* get(char name[]);                 //Returns a pointer to the resource with the desired identifier
    void freeAll();                             //Free all resources :)
};

class Resource                  //This class is to be inherited from
{                               //to make loading and releasing resources
private:                        //a bit cleaner and easier
    char name[128];                         //Identifier
    char fileName[1024];                    //Store filename for possible reloading
    ResourceManager* mgr;
public:
    Resource();
    Resource(char aname[]);                 //An unique string name to identify the resource
    ~Resource(){};
    virtual bool load(char fileName[]){return false;};   //Inherit from this to load a resource
    virtual void free(){};                  //Inherit from this to free a resource
    void setFileName(char aFileName[]);
    char* getFileName(){return fileName;};
    char* getName(){return name;};
    void setManager(ResourceManager* aMgr){mgr = aMgr;};
    ResourceManager* getMgr(){return mgr;};
};

#endif

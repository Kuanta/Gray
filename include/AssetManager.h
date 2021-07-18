#ifndef ASSET_MAN
#define ASSET_MAN

#include <map>
#include "Object.h"
#include "ModelLoader.h"

class GameManager;

class AssetManager
{
public:
    AssetManager();
    AssetManager(GameManager* gm);
    void loadAsset(const char* filePath, const char* assetName);
    void loadAnimation(const char* filePath, const char *assetName, const char* animationName);
    void addAsset(const char *assetName, Object* asset);
    Object *getAsset(const char *assetName);
    Object* cloneAsset(const char *assetName);
    map<string, Object*> assets;
    GameManager* gm;
    ModelLoader ml;
};

#endif

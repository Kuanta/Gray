#include "GameManager.h"
#include "AssetManager.h"

AssetManager::AssetManager()
{
    this->gm = nullptr;
}
AssetManager::AssetManager(GameManager* gm)
{
    this->gm = gm;
}
void AssetManager::loadAsset(const char *filepath, const char *assetName)
{
    Object* asset = this->ml.loadModel(filepath);
    this->addAsset(assetName, asset);
}
void AssetManager::addAsset(const char *assetName, Object* asset)
{
    if (this->assets.find(assetName) != this->assets.end())
    {
        this->assets.at(assetName)->cleanup();
    }
    this->assets.insert(std::pair<string, Object *>(assetName, asset));
}
Object* AssetManager::getAsset(const char *assetName)
{
    if(this->assets.find(assetName) != this->assets.end())
    {
        return this->assets.at(assetName);
    }
    return nullptr;
}
void AssetManager::loadAnimation(const char *filePath, const char *assetName, const char *animationName)
{
    Object* root = this->getAsset(assetName);
    if(root != nullptr)
    {
        this->ml.importAnimations(root, filePath, animationName);
    }
}
Object* AssetManager::cloneAsset(const char *assetName)
{
    if (this->assets.find(assetName) != this->assets.end())
    {
        return this->assets.at(assetName)->clone();
    }
    return nullptr;
}
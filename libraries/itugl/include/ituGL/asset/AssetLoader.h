#pragma once

#include <unordered_map>
#include <string>
#include <memory>

template <typename T>
class AssetLoader
{
public:
    AssetLoader();

    virtual bool IsValid(const char* path);

    virtual T Load(const char* path) = 0;
    virtual T* LoadNew(const char* path);
    virtual std::shared_ptr<T> LoadShared(const char* path);
    virtual bool LoadInto(T&, const char* path);

private:
    std::unordered_map<std::string, std::shared_ptr<T>> m_sharedAssets;
};

template <typename T>
AssetLoader<T>::AssetLoader()
{
}

template <typename T>
bool AssetLoader<T>::IsValid(const char* path)
{
    return true;
}

template <typename T>
T* AssetLoader<T>::LoadNew(const char* path)
{
    T* t = nullptr;
    if (IsValid(path))
    {
        t = new T(Load(path));
    }
    return t;
}

template <typename T>
std::shared_ptr<T> AssetLoader<T>::LoadShared(const char* path)
{
    std::shared_ptr<T> t;
    if (IsValid(path))
    {
        std::string pathString(path);
        auto itAsset = m_sharedAssets.find(pathString);
        if (itAsset != m_sharedAssets.end())
        {
            t = itAsset->second;
        }
        else
        {
            t = std::make_shared<T>(Load(path));
            m_sharedAssets.insert(std::make_pair(pathString, t));
        }
    }
    return t;
}

template <typename T>
bool AssetLoader<T>::LoadInto(T& t, const char* path)
{
    bool valid = IsValid(path);
    if (valid)
    {
        t = Load(path);
    }
    return valid;
}

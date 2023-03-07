#pragma once

#include <unordered_map>
#include <string>
#include <memory>

// Base class for all asset loaders
template <typename T>
class AssetLoader
{
public:
    AssetLoader();

    // Check if an asset is valid for this loader
    virtual bool IsValid(const char* path);

    // Load the asset from a path and return it as value
    virtual T Load(const char* path) = 0;

    // Load the asset from a path into a newly allocated object
    // Requires delete on the new object when it is not used anymore
    virtual T* LoadNew(const char* path);

    // Load the asset from a path into a shared pointer
    virtual std::shared_ptr<T> LoadShared(const char* path);

    // Load the asset from a path into the object passed as a parameter
    virtual bool LoadInto(const char* path, T&);

    inline bool GetKeepShared() const { return m_keepShared; }
    inline void SetKeepShared(bool keepShared) { m_keepShared = keepShared; }

private:
    // If true, keep a reference to assets loaded as shared, to avoid loading twice
    bool m_keepShared;

    // Map of loaded shared assets
    std::unordered_map<std::string, std::shared_ptr<T>> m_sharedAssets;
};

template <typename T>
AssetLoader<T>::AssetLoader() : m_keepShared(true)
{
}

template <typename T>
bool AssetLoader<T>::IsValid(const char* path)
{
    // By default, always valid
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
        // Try to find the asset on the previously loaded
        std::string pathString(path);
        auto itAsset = m_sharedAssets.find(pathString);
        if (itAsset != m_sharedAssets.end())
        {
            t = itAsset->second;
        }
        else
        {
            // If not found, create a new one
            t = std::make_shared<T>(Load(path));
            if (m_keepShared)
            {
                m_sharedAssets.insert(std::make_pair(pathString, t));
            }
        }
    }
    return t;
}

template <typename T>
bool AssetLoader<T>::LoadInto(const char* path, T& t)
{
    bool valid = IsValid(path);
    if (valid)
    {
        t = Load(path);
    }
    return valid;
}

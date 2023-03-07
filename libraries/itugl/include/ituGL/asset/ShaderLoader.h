#pragma once

#include <ituGL/asset/AssetLoader.h>
#include <ituGL/shader/Shader.h>
#include <span>

class ShaderLoader : AssetLoader<Shader>
{
public:
    ShaderLoader(Shader::Type type);

    using AssetLoader<Shader>::IsValid;
    bool IsValid(std::span<const char*> paths);

    Shader Load(const char* path) override;
    using AssetLoader<Shader>::LoadNew;
    using AssetLoader<Shader>::LoadInto;

    Shader Load(std::span<const char*> paths);
    Shader* LoadNew(std::span<const char*> paths);
    bool LoadInto(Shader& shader, std::span<const char*> paths);

    static Shader Load(Shader::Type type, const char* path);

private:
    void Compile(Shader& shader);

    Shader::Type m_type;
};

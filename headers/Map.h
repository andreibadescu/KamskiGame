#pragma once

#include <KamskiEngine/engine/deps/glm/vec2.hpp>
#include "Defines.h"

class Map
{
public:
    Map() = delete;

    void init(u32 sizeX, u32 sizeY);
    void load() const;
    void render(void(*draw)(const glm::vec2& position, const glm::vec2& size, u32 texId)) const;

    void linkTextureTagAndId(texture_id id, TextureTag tag);
    [[nodiscard]]
    texture_id getTextureIdByTag(TextureTag tag) const;

private:
    struct
    {
        texture_id* tiles;
        struct
        {
            u32 x;
            u32 y;
        } size;
    } map{};
    texture_id textureIdsByTag[static_cast<u32>(TextureTag::COUNT)]{};
};

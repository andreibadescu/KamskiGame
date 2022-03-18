#pragma once

#include <KamskiEngine/engine/deps/glm/glm.hpp>

#include "Defines.h"

class Map
{
public:
    Map() = delete;

    void init(u32 sizeX, u32 sizeY);
    void load() const;
    void render() const;
    [[nodiscard]]
    glm::vec2 getQuadSize() const;

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

    glm::vec2 quadSize;
};

#include "headers/Map.h"

void Map::init(const u32 sizeX, const u32 sizeY)
{
    map.size.x = sizeX;
    map.size.y = sizeY;
    // TODO: Use LinearAllocator instead of new
    map.tiles = new texture_id[sizeY * sizeX]{};
}

void Map::load() const
{
    // LEFT and RIGHT edges
    // i = {1, 2, 3, ..., map.size.y - 2}
    for (u32 i = 1; i + 1 < map.size.y; ++i)
    {
        // tiles[i][0]
        map.tiles[i * map.size.x] = getTextureIdByTag(TextureTag::WALL_LEFT);
        // tiles[i][sizeX - 1]
        map.tiles[(i + 1) * map.size.x - 1] = getTextureIdByTag(TextureTag::WALL_RIGHT);
    }

    // TOP and BOTTOM edges
    // j = {1, 2, 3, ..., map.size.x - 2}
    for (u32 j = 1; j + 1 < map.size.x; ++j)
    {
        // tiles[0][j]
        map.tiles[j] = getTextureIdByTag(TextureTag::WALL_BOTTOM);
        // tiles[sizeY - 1][j]
        map.tiles[(map.size.y - 1) * map.size.x + j] = getTextureIdByTag(TextureTag::WALL_TOP);
    }

    // CORNERS
    // tiles[0][0]
    map.tiles[0] = getTextureIdByTag(TextureTag::WALL_CORNER_BOTTOM_LEFT);
    // tiles[0][map.size.x - 1]
    map.tiles[map.size.x - 1] = getTextureIdByTag(TextureTag::WALL_CORNER_BOTTOM_RIGHT);
    // tiles[map.size.y - 1][0]
    map.tiles[map.size.x * (map.size.y - 1)] = getTextureIdByTag(TextureTag::WALL_CORNER_TOP_LEFT);
    // tiles[map.size.y - 1][map.size.x - 1]
    map.tiles[map.size.x * map.size.y - 1] = getTextureIdByTag(TextureTag::WALL_CORNER_TOP_RIGHT);

    // NO EDGE, only inside tiles
    // i = {1, 2, 3, ..., map.size.y - 2}
    // j = {1, 2, 3, ..., map.size.x - 2}
    for (u32 i = 1; i + 1 < map.size.y; ++i)
    {
        for (u32 j = 1; j + 1 < map.size.x; ++j)
        {
            map.tiles[i * map.size.x + j] = getTextureIdByTag(TextureTag::FLOOR);
        }
    }
}

void Map::render(void(*draw)(const glm::vec2& position, const glm::vec2& size, u32 texId)) const
{
    const glm::vec2 quadSize{ 2.0f / static_cast<f32>(map.size.x), 2.0f / static_cast<f32>(map.size.y) };
    for (u32 i = 0; i < map.size.y; ++i)
    {
        for (u32 j = 0; j < map.size.x; ++j)
        {
            // Y-axis is reversed
            glm::vec2 position = glm::vec2(quadSize.x * static_cast<f32>(j), quadSize.y * static_cast<f32>(i))
                + quadSize / 2.0f
                - 1.0f;
            draw(position, quadSize, map.tiles[i * map.size.x + j]);
        }
    }
}

void Map::linkTextureTagAndId(const texture_id id, const TextureTag tag)
{
    textureIdsByTag[static_cast<u32>(tag)] = id;
    textureTagsById[id] = tag;
}

TextureTag Map::getTextureTagById(const u32 id) const
{
    return textureTagsById[id];
}

texture_id Map::getTextureIdByTag(const TextureTag tag) const
{
    return textureIdsByTag[static_cast<u32>(tag)];
}

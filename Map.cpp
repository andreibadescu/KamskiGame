#include "headers/Map.h"
#include "headers/GameState.h"

void Map::init(const u32 sizeX, const u32 sizeY)
{
    map.size.x = sizeX;
    map.size.y = sizeY;
    map.tiles = (texture_id*)MEMORY->globalAlloc(sizeof(texture_id) * sizeY * sizeX);
    quadSize = { 2.0f * SCREEN_SIZE_WORLD_COORDS / static_cast<f32>(map.size.x), 2.0f * SCREEN_SIZE_WORLD_COORDS / static_cast<f32>(map.size.y) };
}

void Map::load() const
{
    // LEFT and RIGHT edges
    // i = {1, 2, 3, ..., map.size.y - 2}
    for (u32 i = 1; i + 1 < map.size.y; ++i)
    {
        // tiles[i][0]
        map.tiles[i * map.size.x] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_LEFT);
        // tiles[i][sizeX - 1]
        map.tiles[(i + 1) * map.size.x - 1] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_RIGHT);
    }

    // TOP and BOTTOM edges
    // j = {1, 2, 3, ..., map.size.x - 2}
    for (u32 j = 1; j + 1 < map.size.x; ++j)
    {
        // tiles[0][j]
        map.tiles[j] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_BOTTOM);
        // tiles[sizeY - 1][j]
        map.tiles[(map.size.y - 1) * map.size.x + j] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_TOP);
    }

    // CORNERS
    // tiles[0][0]
    map.tiles[0] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_CORNER_BOTTOM_LEFT);
    // tiles[0][map.size.x - 1]
    map.tiles[map.size.x - 1] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_CORNER_BOTTOM_RIGHT);
    // tiles[map.size.y - 1][0]
    map.tiles[map.size.x * (map.size.y - 1)] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_CORNER_TOP_LEFT);
    // tiles[map.size.y - 1][map.size.x - 1]
    map.tiles[map.size.x * map.size.y - 1] = GAME_STATE->getTextureIdByTag(TextureTag::WALL_CORNER_TOP_RIGHT);

    // NO EDGE, only inside tiles
    // i = {1, 2, 3, ..., map.size.y - 2}
    // j = {1, 2, 3, ..., map.size.x - 2}
    for (u32 i = 1; i + 1 < map.size.y; ++i)
    {
        for (u32 j = 1; j + 1 < map.size.x; ++j)
        {
            map.tiles[i * map.size.x + j] = GAME_STATE->getTextureIdByTag(TextureTag::FLOOR);
        }
    }
}

void Map::render() const
{
    for (u32 i = 0; i < map.size.y; ++i)
    {
        for (u32 j = 0; j < map.size.x; ++j)
        {
            // Y-axis is reversed
            glm::vec2 position = glm::vec2(quadSize.x * static_cast<f32>(j), quadSize.y * static_cast<f32>(i))
                + quadSize / 2.0f
                - SCREEN_SIZE_WORLD_COORDS;
            RENDERER->drawTexturedQuad(position, quadSize, map.tiles[i * map.size.x + j], 0);
        }
    }
}

glm::vec2 Map::getQuadSize() const
{
    return quadSize;
}

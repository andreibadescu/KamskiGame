#include "headers/GameState.h"

bool isCollision(glm::vec2 posA, glm::vec2 posB,
                 glm::vec2 sizeA, glm::vec2 sizeB)
{
    return (abs(posA.x - posB.x) <= sizeA.x / 2.0f + sizeB.x / 2.0f) &&
    (abs(posA.y - posB.y) <= sizeA.y / 2.0f + sizeB.y / 2.0f);
}

void drawSprites()
{
    //AssetId assetId = engine->findAsset(ASSET_TAG_BUTTON, attribVec);
    
    //engine->drawTexturedQuad({0, 0}, {0,0}, engine->getTextureFromAssetId(assetId);
}
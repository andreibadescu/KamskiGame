#include "headers/InputState.h"

void InputState::init()
{

}

void InputState::pressUp()
{
    setUp(true);
}

void InputState::releaseUp()
{
    setUp(false);
}

void InputState::pressDown()
{
    setDown(true);
}

void InputState::releaseDown()
{
    setDown(false);
}

void InputState::pressLeft()
{
    setLeft(true);
}

void InputState::releaseLeft()
{
    setLeft(false);
}

void InputState::pressRight()
{
    setRight(true);
}

void InputState::releaseRight()
{
    setRight(false);
}

bool InputState::isUpKeyPressed() const
{
    return static_cast<bool>(keysMask & static_cast<u8>(KeyBit::UP));
}

bool InputState::isDownKeyPressed() const
{
    return static_cast<bool>(keysMask & static_cast<u8>(KeyBit::DOWN));
}

bool InputState::isLeftKeyPressed() const
{
    return static_cast<bool>(keysMask & static_cast<u8>(KeyBit::LEFT));
}

bool InputState::isRightKeyPressed() const
{
    return static_cast<bool>(keysMask & static_cast<u8>(KeyBit::RIGHT));
}

void InputState::setUp(const bool pressed)
{
    if (pressed)
    {
        keysMask |= static_cast<u8>(KeyBit::UP);
    }
    else
    {
        keysMask &= ~static_cast<u8>(KeyBit::UP);
    }
}

void InputState::setDown(const bool pressed)
{
    if (pressed)
    {
        keysMask |= static_cast<u8>(KeyBit::DOWN);
    }
    else
    {
        keysMask &= ~static_cast<u8>(KeyBit::DOWN);
    }
}

void InputState::setLeft(const bool pressed)
{
    if (pressed)
    {
        keysMask |= static_cast<u8>(KeyBit::LEFT);
    }
    else
    {
        keysMask &= ~static_cast<u8>(KeyBit::LEFT);
    }
}

void InputState::setRight(const bool pressed)
{
    if (pressed)
    {
        keysMask |= static_cast<u8>(KeyBit::RIGHT);
    }
    else
    {
        keysMask &= ~static_cast<u8>(KeyBit::RIGHT);
    }
}

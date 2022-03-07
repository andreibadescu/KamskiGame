#pragma once

#include "Defines.h"

class InputState
{
public:
    void init();

    void pressUp();
    void releaseUp();

    void pressDown();
    void releaseDown();

    void pressLeft();
    void releaseLeft();

    void pressRight();
    void releaseRight();

    void setUp(bool pressed);
    void setDown(bool pressed);
    void setLeft(bool pressed);
    void setRight(bool pressed);

    [[nodiscard]]
    bool isUpKeyPressed() const;
    [[nodiscard]]
    bool isDownKeyPressed() const;
    [[nodiscard]]
    bool isLeftKeyPressed() const;
    [[nodiscard]]
    bool isRightKeyPressed() const;

private:
    enum class KeyBit: u8
    {
        UP = 0x1,
        DOWN = 0x2,
        LEFT = 0x4,
        RIGHT = 0x8
    };

    // 8-bit mask
    u8 keysMask;
};

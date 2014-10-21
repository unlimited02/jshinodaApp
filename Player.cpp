#include "Player.h"

USING_NS_CC;

//初期化
bool Player::init()
{
    if (!Sprite::initWithFile("penguin_1.png"))
    {
        return false;
    }
    return true;
}
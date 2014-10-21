#ifndef __TheEscapist__Player__
#define __TheEscapist__Player__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
    virtual bool init(); //初期化
    CREATE_FUNC(Player); //create関数マクロ
    
};

#endif /* defined(__TheEscapist__Player__) */

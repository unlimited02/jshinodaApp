#ifndef __TheEscapist__GameOver__
#define __TheEscapist__GameOver__

#include "cocos2d.h"
#include "SaveSQL.h"


class GameOver : public cocos2d::Layer
{
    void initBackground();
    void initMenu();
    void menuRetryCallback(Ref* pSender);
    void endCallback(Ref* pSender);
    void initShowScore();
    void RankUpdate();
    std::string showRunk(int score);
    
    int _showScore;
    int _showhighScore;
    const char *gameScoreRank;
    const char *highScoreRank;
    
public:

    GameOver();
    virtual ~GameOver();

    virtual void onEnter();
    virtual void onExit();
    
    
    static cocos2d::Scene* createScene(); //シーンを作成
    virtual bool init(); //初期化
    CREATE_FUNC(GameOver); //create関数作成
    
    virtual bool onTouchBegan(cocos2d::Touch *Touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *Touch, cocos2d::Event *unused_event);
};





#endif /* defined(__TheEscapist__GameOver__) */

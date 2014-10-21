#include "Obstacle.h"

USING_NS_CC;

//障害物タイプのコンストラクタ
Obstacle::Obstacle(int obstacleType) : _obstacleType (obstacleType)
{
}

Obstacle* Obstacle::create(int obstacleType)
{
    Obstacle *obstacle = new Obstacle(obstacleType);
    obstacle->init();
    obstacle->autorelease();
    
    return obstacle;
}

bool Obstacle::init()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //障害物移動範囲のランダム設定
    std::random_device rd;
    _mt = std::mt19937(rd());
    _height = std::uniform_real_distribution<double>(0.0, 1.0);
    _bigHeight = std::uniform_real_distribution<double>(0.7, 1.0);
    _smallHeight = std::uniform_real_distribution<double>(0.0, 0.3);
    
    std::string filename; //ファイル名
    Vec2 initPos; //初期位置
    Action* action; //障害物の動き
    
    
    switch (_obstacleType)
    {
        case Type1:
            filename = "HelloWorld.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.9);
            action = getAction1();
            break;
            
        case Type2:
            filename = "Enemy2.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.7);
            action = getAction2();
            break;
            
        case Type3:
            filename = "Enemy1.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.5);
            action = getAction3();
            break;
            
        case Type4:
            filename = "Enemy2.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.3);
            action = getAction4();
            break;
            
        case Type5:
            filename = "Enemy1.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.2);
            action = getAction5();
            break;
            
        case Type6:
            filename = "HelloWorld.png";
            initPos = Vec2(winSize.width * 1.1, winSize.height * 0.1);
            action = getAction6();
            break;
    }
    
    if(!Sprite::initWithFile(filename))
    {
        return false;
    }
    //初期位置とアクションをセット
    setPosition(initPos);
    runAction(action);
    
    
    return true;
}

//Type1のアクション
Action* Obstacle::getAction1()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1.8, Vec2(winSize.width * -0.1, winSize.height * _bigHeight(_mt)));
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
   
}

Action* Obstacle::getAction2()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1, Vec2(winSize.width * -0.1, winSize.height * _height(_mt)));
    auto remove = RemoveSelf::create();
    //消す
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
}

Action* Obstacle::getAction3()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1, Vec2(winSize.width * -0.1, winSize.height * _height(_mt)));
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
}

Action* Obstacle::getAction4()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1.2, Vec2(winSize.width * -0.1, winSize.height * _height(_mt)));
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
}

Action* Obstacle::getAction5()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1.3, Vec2(winSize.width * -0.1, winSize.height * _height(_mt)));
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
}

Action* Obstacle::getAction6()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    //移動アクションを生成
    auto move = MoveTo::create(1.8, Vec2(winSize.width * -0.1, winSize.height * _smallHeight(_mt)));
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move, remove, NULL);
    
    return seq;
}




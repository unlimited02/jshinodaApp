#include "MainScene.h"
#include "Obstacle.h"
#include "Player.h"
#include "GameOver.h"
#include "macros.h"

// enum Tagからの決めうち
#define T_itemMcr(__var) 900+__var

USING_NS_CC;

MainScene::MainScene()
: _score(0)
, _state(State::GameStart)
, _time(0.f)
, _jumpingTime(0.f)
, _jumpPointY(0.f)
, _nextObstacleTime(0.f)
, _nextItemTime(0.f)
, _plusPoint(0.f)
{
}

MainScene::~MainScene()
{
//    this->removeAllChildren();
}

Scene* MainScene::createScene()
{
    Scene* scene = Scene::create();
    MainScene* layer = MainScene::create();
    
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainScene::init()
{
    CCLOG("プルリクエスト用");
    if ( !Layer::init() )
    {
        return false;
        
    }
    std::random_device rd;
    _mt = std::mt19937(rd());
    
    //表示する障害物のランダム設定
    _obtype = std::uniform_int_distribution<>(OB_RANDOM);
    //アイテム表示のランダム設定
    _itemType = std::uniform_int_distribution<>(ITEM_RANDOM);
    
    EventListenerTouchAllAtOnce *listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void MainScene::initBackground()
{

    Sprite* backgrounds1 = Sprite::create("sky.png");
    backgrounds1->setPosition(Point(WINSIZE.width / 2, WINSIZE.height / 2));
    this->addChild(backgrounds1,Z_Bg,T_BG1);
    
    Sprite* backgrounds2 = Sprite::create("sky2.png");
    backgrounds2->setPosition(Point(WINSIZE.width / 2 + WINSIZE.width, WINSIZE.height / 2));
    this->addChild(backgrounds2,Z_Bg,T_BG2);
    
}

void MainScene::initScore()
{
    Label *scoreLabel =(Label*)getChildByTag(T_label);
    if(!scoreLabel)
    {
        auto scoreLabel = Label::createWithSystemFont("","Marker Felt", 48);
        scoreLabel->setPosition(Point(WINSIZE.width * 0.5, WINSIZE.height * 0.9));
        scoreLabel->setColor(Color3B::WHITE);
        scoreLabel->enableOutline(Color4B::BLACK,5);

        scoreLabel->setTag(T_label);
        this->addChild(scoreLabel,Z_Score);
    }
}

void MainScene::onEnter()
{
    Layer::onEnter();
    
    //背景を表示
    initBackground();
    
    //プレイヤーを表示
    initPlayer();
    
    //スコア表示
    initScore();
    
    auto titleLabel = Label::createWithSystemFont("現実逃飛行","Marker Felt", 100);
    titleLabel->setPosition(Point(WINSIZE.width * 0.5, WINSIZE.height * 0.8));
    titleLabel->setColor(Color3B::ORANGE);
    titleLabel->enableOutline(Color4B::BLACK,5);
    this->addChild(titleLabel,Z_Title,T_Title);
    
    
    Sprite* popup = Sprite::create("getready.png");
    popup->setPosition(Point(WINSIZE.width/2,WINSIZE.height * 0.4));
    addChild(popup, Z_GameStartLayer,T_Start);
    
    _state = State::GameStart;
}

void MainScene::onExit()
{
    Layer::onExit();
    
    this->removeAllChildren();
}

void MainScene::onTouchesBegan(const std::vector<Touch*> & touches, Event *event)
{
    auto _player = this->getChildByTag(T_Player);
    
   //フレームにplistを追加
    SpriteFrameCache* playerCache = SpriteFrameCache::getInstance();
    playerCache->addSpriteFramesWithFile("pengX.plist");
    
     //プレイヤーアニメーションの作成
    auto playerAnimation = Animation::create();
    for (int i = 1; i < 4; i++)
    {
        auto str = __String::createWithFormat("penguin_%d.png",i);
        SpriteFrame *sprite = playerCache->getSpriteFrameByName(str->getCString());
        playerAnimation->addSpriteFrame(sprite);
    }
    
    playerAnimation->setDelayPerUnit(ANIME_SPEED); //ANIME SPEED default 0.25f 羽ばたく速度
    //playerAnimation->setRestoreOriginalFrame(true); // 画像を元に戻す
    auto playerAnimate = Animate::create(playerAnimation);
    
    
    if(_state == State::GameStart)
    {
        //タッチと同時にスタートが消える
        Node* start = this->getChildByTag(T_Start);
        start->runAction(FadeOut::create(1.f));
        
        auto title = (Label*)getChildByTag(T_Title);
        title->runAction(FadeOut::create(0.5f));
        
        
        _plusPoint = 1;
        
        _state = State::Gaming;
        //_player->runAction(RepeatForever::create(playerAnimate));
        _time = 0;
        _jumpingTime = 0;
        
        //最初の障害物、アイテムが出るまでの時間
        _nextObstacleTime = INIT_OBSTACLE;
        _nextItemTime = INIT_ITEM;
        
        _jumpPointY = _player->getPositionY();
        
        scheduleUpdate();
    }
    
    
    if(_state == State::Gaming)
    {
        _player->runAction(playerAnimate);
        _jumpPointY = _player->getPositionY();
        _jumpingTime = _time;
    }
}


void MainScene::initPlayer()
{
     SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pengX.plist");
    auto _player = Sprite::createWithSpriteFrameName("penguin_1.png");
    _player->setPosition(Point(WINSIZE.width * 0.2, WINSIZE.height / 2));
    this->addChild(_player,Z_Player,T_Player);
}


//障害物を表示する
void MainScene::showObstacle()
{
 
    //障害物のタイプ設定
    int type = _obtype(_mt);
    _obstacle = Obstacle::create(type);
    
    this->addChild(_obstacle, Z_Obstscle, T_Obstacle);
    
}


void MainScene::showItem()
{
    int itemType = _itemType(_mt);
    _item = Item::create(itemType);
    

    if(itemType >= 1 && itemType <= 3)
    {
        this->addChild(_item, Z_Item, T_itemMcr(itemType));
    }
}


//毎フレーム処理
void MainScene::update(float dt)
{
    
    //トータル時間を加算する
    _time += dt;
    
    //アイテム衝突処理
    contactItem();
    
    //背景スピード = アイテム加算値 限界スピード7.0
    float backSpeedUp = _plusPoint;

    if(backSpeedUp > 7.0)
    {
        backSpeedUp = 7.0;
    }
    
    //背景無限ループ処理
     auto BG1 = this->getChildByTag(T_BG1);
    {
        float nextX = BG1->getPositionX() - BACK_SPEED * backSpeedUp;
      
        float BackgrondsW = BG1->boundingBox().size.width;
        if(BG1->getPositionX() < - WINSIZE.width /2)
        {
            nextX = WINSIZE.width + BackgrondsW/2;
        }
        BG1->setPositionX(nextX);

    }
    
    auto BG2 = this->getChildByTag(T_BG2);
    {
        float nextX = BG2->getPositionX() - BACK_SPEED * backSpeedUp;
        
        float BackgrondsW = BG1->boundingBox().size.width;
        if(BG2->getPositionX() < - WINSIZE.width /2)
        {
            nextX = WINSIZE.width + BackgrondsW/2;
        }
        BG2->setPositionX(nextX);
    }
    
    
    auto scoreLabel = (Label*)getChildByTag(T_label);
    if(scoreLabel)
    {
        //時間の表示
        scoreLabel->setString(StringUtils::format("現実逃避日数 : %10.0f 日", _time * 10 * _plusPoint));
    }
    
    //次の障害物andアイテムの出現時間
    if(_time > _nextObstacleTime)
    {
        showObstacle();
        
        _nextObstacleTime += NEXT_OBSTACLE;
    }
    
    if(_time > _nextItemTime)
    {
        showItem();
        
        
        _nextItemTime += NEXT_ITEM;
    }
    
    //プレイヤースプライトの呼び出し
    auto _player = this->getChildByTag(T_Player);
    //ゲームオーバー（落下、上昇しすぎ、障害物にぶつかる）

    const float offsetPlayerHeight = (_player->getContentSize().height / 2.f);

    if(_player->getPositionY() < (0 - offsetPlayerHeight) ||
       _player->getPositionY() > WINSIZE.height + offsetPlayerHeight ||
       contactObstacle() )
    {
        unscheduleUpdate();
        CCLOG("ミス！！");
        _state = State::GameOver;
        
        for(auto node : getChildren())
        {
            node->stopAllActions();
        }
        
        this->ScoreSave();
        
        // シーンアウト処理の遅延実行
        const float delayTime = 0.5f;
        this->scheduleOnce(schedule_selector(MainScene::toGameOverScene), delayTime);
    }
    
    //プレイヤーの位置を変更
    float time = _time - _jumpingTime;
    float newY = (JUMP_V0 * time - GRAVITY * time * time / 2) * POSITION_RATE + _jumpPointY;
    _player -> setPositionY(newY);
    
    
}

bool MainScene::contactObstacle()
{
    auto _player = this->getChildByTag(T_Player);
    Rect playerRect = _player->getBoundingBox();
    
    for(auto node : getChildren())
    {
        if(node->getTag() == T_Obstacle)
        {
            auto Obstacles = node;
            Rect obstacleRect = Obstacles->getBoundingBox();
            
            if(playerRect.intersectsRect(obstacleRect))
            {
                CCLOG("衝突");
                return true;
            }
        }
    }
 
    return false;
}


void MainScene::contactItem()
{
    auto _player = this->getChildByTag(T_Player);
    Action* action;
   Rect playerRect = _player->getBoundingBox();
    
    for(Node* node : getChildren())
    {
        bool continueFlag = false;
        
        float plusPt = 0.f;
        switch(node->getTag())
        {
            case T_Item1:
                plusPt = 0.5f;
                break;
            case T_Item2:
                plusPt = 1.f;
                break;
            case T_Item3:
                plusPt = 1.5f;
                break;
                
            default:
                continueFlag = true;
                break;
        }
        
        // 条件に当てはまらなかったら後の処理を行わない
        if(continueFlag)
        {
            continue;
        }
        
        auto item = node;
        Rect itemRect = item->getBoundingBox();
        // 弾く処理
        if(!playerRect.intersectsRect(itemRect))
        {
            continue;
        }
        
        _plusPoint += plusPt; //スコアが上がる速度にプラスする掛け値
        
        action = RemoveSelf::create();
        item->runAction(action);
        CCLOG("get%d", (int)plusPt*2);
    }

}


void MainScene::ScoreSave()
{
    _score = _time * 10 * _plusPoint;
    
    //scoreを保存
    auto score = UserDefault::getInstance();
    score->setIntegerForKey("gameScore", _score);
    score->flush();
    
    //スコアがハイスコアを超えていた場合更新
    auto highScore =UserDefault::getInstance();
    int judgeHighScore = highScore->getIntegerForKey("highScore");
    
    if(_score > judgeHighScore)
    {
        highScore->setIntegerForKey("highScore", _score);
        highScore->flush();
    }
}


void MainScene::toGameOverScene(float delayTime)
{
    CCLOG("wait time finished !!!");
    // シーン遷移の処理を別関数に記述
    Scene* scene =  GameOver::createScene();
    Scene* tran = TransitionTurnOffTiles::create(1.5f,scene);
    Director::getInstance()->replaceScene(tran);
}

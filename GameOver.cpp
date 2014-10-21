#include "GameOver.h"
#include "MainScene.h"
#include "macros.h"


USING_NS_CC;

GameOver::GameOver()
: _showhighScore(0)
, _showScore(0)
{
}

GameOver::~GameOver()
{
    this->removeAllChildren();
}

Scene* GameOver::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOver::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameOver::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //シングルタップ用リスナーを用意する
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches);
    
    //各イベントの割当
    listener->onTouchBegan = CC_CALLBACK_2(GameOver::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(GameOver::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameOver::onTouchEnded, this);
    
    //イベントディスバッチャにシングルタップ用リスナーを追加する
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameOver::onEnter()
{
    Layer::onEnter();
    
    //背景を表示
    initBackground();
    
    initMenu();
    
    initShowScore();
    
    //RankUpdate();
}

void GameOver::onExit()
{
    Layer::onExit();
}

void GameOver::initBackground()
{
    //背景の生成
    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Point(0, 0.5));
    background->setPosition(Point(0, WINSIZE.height / 2));
    this->addChild(background);
    
}

void GameOver::initMenu()
{
    //戻るボタンとやめるボタンの実装
    auto retryItem = MenuItemImage::create("back_normal.png",
                                           "back_selected.png",
                                           CC_CALLBACK_1(GameOver::menuRetryCallback,this));
    auto endItem = MenuItemImage::create("CloseNormal.png",
                                         "CloseSelected.png",
                                         CC_CALLBACK_1(GameOver::endCallback,this));
    
    
    retryItem->setPosition(Point(WINSIZE.width / 2 , WINSIZE. height * 0.3));
    endItem->setPosition(Point(WINSIZE.width * 0.9 , WINSIZE. height * 0.1));
    
    //menuに登録
    auto menu = Menu::create(retryItem,endItem,NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    
}


void GameOver::initShowScore()
{
    //今回のスコアを取り出す
    auto score = UserDefault::getInstance();
    int _showScore = score->getIntegerForKey("gameScore");
    
    //ハイスコアを取り出す
    auto highScore = UserDefault::getInstance();
    int _showhighScore = highScore->getIntegerForKey("highScore");
    
    //今回のスコアからランクを呼び出す（SQliteを使用）
    const char * gameScoreRank = SaveSQL::sqliteGetRankForScore(_showScore);
    const char * highScoreRank = SaveSQL::sqliteGetRankForScore(_showhighScore);
    
    SaveSQL::sqliteUpdateValueForKey(gameScoreRank,1);
    
    //スコア表示
    auto scoreLabel = Label::createWithSystemFont("","Marker Felt", 48);
    scoreLabel->setPosition(Point(WINSIZE.width * 0.5, WINSIZE.height * 0.9));
    scoreLabel->setColor(Color3B::BLACK);
    this->addChild(scoreLabel);
    
    scoreLabel->setString(StringUtils::format("現実逃避日数 : %d 日 \n現実逃避年数 : %d 年 %d 日", _showScore, _showScore/365, _showScore%365));
    
    
    //ランク表示
    auto rankLabel = Label::createWithSystemFont("","Marker Felt", 50);
    rankLabel->setPosition(Point(WINSIZE.width * 0.5, WINSIZE.height * 0.5));
    rankLabel->setColor(Color3B::WHITE);
    rankLabel->enableOutline(Color4B::BLACK,5);
    
    this->addChild(rankLabel);
    
    rankLabel->setString(StringUtils::format("ランク : %s", gameScoreRank));
 
    
    //ハイスコアandランク表示
    auto scoreLabel2 = Label::createWithSystemFont("","Marker Felt", 25);
    scoreLabel2->setPosition(Point(WINSIZE.width * 0.7, WINSIZE.height * 0.7));
    scoreLabel2->setColor(Color3B::RED);


    this->addChild(scoreLabel2);
    
    scoreLabel2->setString(StringUtils::format("最高逃避日数 : %d 日 \n最高現実逃避年数 : %d 年 %d 日 \n ランク : %s", _showhighScore, _showhighScore/365, _showhighScore%365, highScoreRank));
}


bool GameOver::onTouchBegan(Touch *touch, Event *unused_event)
{
    //タップ開始時の処理
    CCLOG("onTouchBegan");
    
    return true;
}


void GameOver::onTouchEnded(Touch *touch, Event *unused_event)
{
    //タップ終了時の処理
    CCLOG("onTouchEnded");
}


void GameOver::menuRetryCallback(Ref* pSender)
{
    //新しく画面を用意する
    auto scene = MainScene::createScene();
    
    //フェードアニメーションの設定
    auto tran = TransitionFade::create(1, scene);
    
    //新しい画面に画面遷移する
    Director::getInstance()->replaceScene(tran);
}

void GameOver::endCallback(Ref* pSender)
{
    auto scene = MainScene::createScene();
    
    //フェードアニメーションの設定
    auto tran = TransitionFade::create(1, scene);
    
    //新しい画面に画面遷移する
    Director::getInstance()->replaceScene(tran);
}

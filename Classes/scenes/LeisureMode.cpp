#include "LeisureMode.h"
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
USING_NS_CC;
using namespace CocosDenshion;

Scene* LeisureMode::createScene(const char* language, const char* level)
{
	auto scene = Scene::create();
	auto layer = new LeisureMode();
	if (layer && layer->init(language, level)) {
		layer->autorelease();
	}
	else
	{
		delete layer;
		layer = NULL;
	}
	scene->addChild(layer);
	return scene;
}
bool LeisureMode::init(const char* language, const char* level)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	const char* level_;
	if (strcmp(level, "one") == 0)
		level_ = "1";
	else if (strcmp(level, "two") == 0)
		level_ = "2";
	else if (strcmp(level, "three") == 0)
		level_ = "3";
	else if (strcmp(level, "four") == 0)
		level_ = "4";
	else if (strcmp(level, "five") == 0)
		level_ = "5";
	else if (strcmp(level, "six") == 0)
		level_ = "6";
	DictLayer::language = language;
	DictLayer::level = level_;
	dictLayer = DictLayer::create();
	this->addChild(dictLayer);

	//键盘监听
	auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(EndlessMode::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(LeisureMode::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

void LeisureMode::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{

		//auto scene = MenuScene::createScene();
		//auto transitionScene = TransitionFade::create(switchMenuTime,
		//	scene);
		//auto director = Director::getInstance();
		//director->replaceScene(transitionScene);

	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU || keyCode == EventKeyboard::KeyCode::KEY_TAB)
	{
	}
}
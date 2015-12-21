#include "MenuScene.h"
#include "resources.h"
#include "TransmitterMode.h"
#include "MorseCode.h"
#include "Dict.h"
#include "GameConfigureScene.h"
#include "GadgetScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
MenuScene::MenuScene() :
		sound_sign(true) {
}
MenuScene::~MenuScene() {
}
Scene* MenuScene::createScene() {
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(layer, 0);

	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE) {
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		font_size = font_menuitem_size;
		buttonNormal = black_button;
		buttonSelect = black_select_button;
	} else {
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_menuitem_size;
		buttonNormal = black_button;
		buttonSelect = black_select_button;
	}

	auto playNormalLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("Play"))->_string, font_type,
			font_size);
	playNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* playNormal = createBlock(buttonNormal, Vec2(0.5, 0.5),
			playNormalLabel);

	auto playSelectLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("Play"))->_string, font_type,
			font_size);
	playSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* playSelect = createBlock(buttonSelect, Vec2(0.5, 0.5),
			playSelectLabel);

	MenuItemSprite* menuItemPlay =
			MenuItemSprite::create(playNormal, playSelect, NULL,
					[&](Ref* sender)
					{
						SimpleAudioEngine::getInstance()->end();
						auto scene = GameConfigureScene::createScene();
						auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
						auto director = Director::getInstance();
						director->replaceScene(transitionScene);
					});

//	auto transmitterNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Transmitter"))->_string, font_type, font_size);
//	transmitterNormalLabel->setColor(Color3B(255, 255, 255));
//	Sprite* transmitterNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), transmitterNormalLabel);
//
//	auto transmitterSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Transmitter"))->_string, font_type, font_size);
//	transmitterSelectLabel->setColor(Color3B(205, 205, 205));
//	Sprite* transmitterSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), transmitterSelectLabel);
//
//	MenuItemSprite* menuItemTransmitter = MenuItemSprite::create(transmitterNormal, transmitterSelect, NULL, [&](Ref* sender)
//	{
//		SimpleAudioEngine::getInstance()->end();
//		auto scene = TransmitterMode::createScene();
//		auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
//		auto director = Director::getInstance();
//		director->replaceScene(transitionScene);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		if (!AdmobHelper::isAdShowing)
//			AdmobHelper::showAd();
//#endif
//	});
//	auto listenerNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Listener"))->_string, font_type, font_size);
//	listenerNormalLabel->setColor(Color3B(255, 255, 255));
//	Sprite* listenerNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), listenerNormalLabel);
//
//	auto listenerSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Listener"))->_string, font_type, font_size);
//	listenerSelectLabel->setColor(Color3B(205, 205, 205));
//	Sprite* listenerSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), listenerSelectLabel);
//
//	MenuItemSprite* menuItemListener = MenuItemSprite::create(listenerNormal, listenerSelect, NULL, [&](Ref* sender)
//	{
//	});
//
//	auto monitorNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Monitor"))->_string, font_type, font_size);
//	monitorNormalLabel->setColor(Color3B(255, 255, 255));
//	Sprite* monitorNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), monitorNormalLabel);
//
//	auto monitorSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Monitor"))->_string, font_type, font_size);
//	monitorSelectLabel->setColor(Color3B(205, 205, 205));
//	Sprite* monitorSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), monitorSelectLabel);
//
//	MenuItemSprite* menuItemMonitor = MenuItemSprite::create(monitorNormal, monitorSelect, NULL, [&](Ref* sender)
//	{
//	});

	auto codeBookNormalLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("MorseCode"))->_string,
			font_type, font_size);
	codeBookNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* codeBookNormal = createBlock(buttonNormal, Vec2(0.5, 0.5),
			codeBookNormalLabel);

	auto codeBookSelectLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("MorseCode"))->_string,
			font_type, font_size);
	codeBookSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* codeBookSelect = createBlock(buttonSelect, Vec2(0.5, 0.5),
			codeBookSelectLabel);

	MenuItemSprite* menuItemCodeBook =
			MenuItemSprite::create(codeBookNormal, codeBookSelect, NULL,
					[&](Ref* sender)
					{
						SimpleAudioEngine::getInstance()->end();
						auto scene = MorseCode::createScene();
						auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
						auto director = Director::getInstance();
						director->replaceScene(transitionScene);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					if (!AdmobHelper::isAdShowing)
						AdmobHelper::showAd();
					AdmobHelper::showGDTInterstitialAd();
#endif
				});

	auto gadgetNormalLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("CommonFunctions"))->_string,
			font_type, font_size);
	gadgetNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* gadgetNormal = createBlock(buttonNormal, Vec2(0.5, 0.5),
			gadgetNormalLabel);

	auto gadgetSelectLabel = Label::createWithTTF(
			((__String *) strings->objectForKey("CommonFunctions"))->_string,
			font_type, font_size);
	gadgetSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* gadgetSelect = createBlock(buttonSelect, Vec2(0.5, 0.5),
			gadgetSelectLabel);

	MenuItemSprite* menuItemGadget =
			MenuItemSprite::create(gadgetNormal, gadgetSelect, NULL,
					[&](Ref* sender)
					{
						SimpleAudioEngine::getInstance()->end();
						auto scene = GadgetScene::createScene();
						auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
						auto director = Director::getInstance();
						director->replaceScene(transitionScene);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					if (!AdmobHelper::isAdShowing)
					AdmobHelper::showAd();
#endif
				});
//	auto dictNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Dict"))->_string, font_type, font_size);
//	dictNormalLabel->setColor(Color3B(255, 255, 255));
//	Sprite* dictNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), dictNormalLabel);
//
//	auto dictSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Dict"))->_string, font_type, font_size);
//	dictSelectLabel->setColor(Color3B(205, 205, 205));
//	Sprite* dictSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), dictSelectLabel);
//
//	MenuItemSprite* menuItemDict = MenuItemSprite::create(dictNormal, dictSelect, NULL, [&](Ref* sender)
//	{
//		SimpleAudioEngine::getInstance()->end();
//		auto scene = Dict::createScene();
//		auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
//		auto director = Director::getInstance();
//		director->replaceScene(transitionScene);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		if (!AdmobHelper::isAdShowing)
//			AdmobHelper::showAd();
//#endif
//	});

	Menu* menu = Menu::create(menuItemPlay,/*menuItemTransmitter, menuItemListener, menuItemMonitor,*/
	menuItemCodeBook, /*menuItemDict,*/menuItemGadget, NULL);
	menu->setPosition(
			Vec2(visibleSize.width / 2 + origin.x,
					visibleSize.height / 2 + origin.y));
	menu->alignItemsVerticallyWithPadding(30);
	//menu->alignItemsHorizontallyWithPadding(0);
	//menu->alignItemsInColumns(1,1,1,1,1);
	layer->addChild(menu);

	auto hornButton = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(MenuScene::menuMusicToggleCallback, this),
			MenuItemImage::create("horn.png", "horn.png"),
			MenuItemImage::create("nohorn.png", "nohorn.png"), NULL);
	hornButton->setPosition(
			Vec2(visibleSize.width - hornButton->getContentSize().width,
					hornButton->getContentSize().height * 2));
	auto hornMenu = Menu::create(hornButton, NULL);
	hornMenu->setPosition(Point::ZERO);
	layer->addChild(hornMenu);

	SimpleAudioEngine::getInstance()->playBackgroundMusic(morseMusic, true);
	return true;
}
void MenuScene::menuMusicToggleCallback(Ref* sender) {
	if (sound_sign) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
		sound_sign = false;
	} else if (!sound_sign) {
		SimpleAudioEngine::getInstance()->playBackgroundMusic(morseMusic, true);
		sound_sign = true;
	}
}
void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
			|| keyCode == EventKeyboard::KeyCode::KEY_ESCAPE
			|| keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
					{
		Director::getInstance()->end();
	} else if (keyCode == EventKeyboard::KeyCode::KEY_MENU
			|| keyCode == EventKeyboard::KeyCode::KEY_TAB) {
	}
}
Sprite* MenuScene::createBlock(std::string buttonName, Point anchor,
		Label* blockLabel) {
	auto button = Sprite::create(buttonName);
	button->setAnchorPoint(anchor);
	button->addChild(blockLabel, 10);
	blockLabel->setPosition(
			Vec2(button->getContentSize().width / 2,
					button->getContentSize().height / 2));
	return button;
}

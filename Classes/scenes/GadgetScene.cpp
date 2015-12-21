#include "GadgetScene.h"
#include "MenuScene.h"
#include "ShareScene.h"
#include "SMSScene.h"
#include "FlashScene.h"
USING_NS_CC;
Scene* GadgetScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GadgetScene::create();
	scene->addChild(layer);
	return scene;
}
bool GadgetScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//键盘监听
	auto listener = EventListenerKeyboard::create();
	//listener->onKeyPressed = CC_CALLBACK_2(EndlessMode::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GadgetScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE){
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		font_size = font_menuitem_size;
		buttonNormal = black_button;
		buttonSelect = black_select_button;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_menuitem_size;
		buttonNormal = black_button;
		buttonSelect = black_select_button;
	}
	createMenu();
	return true;
}
void GadgetScene::createMenu(){
	auto flashNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("flash"))->_string, font_type, font_size);
	flashNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* flashNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), flashNormalLabel);

	auto flashSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("flash"))->_string, font_type, font_size);
	flashSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* flashSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), flashSelectLabel);

	MenuItemSprite* menuItemFlash = MenuItemSprite::create(flashNormal, flashSelect, NULL, [&](Ref* sender)
	{
		auto scene = FlashScene::createScene();
		auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	});

	auto smsNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("sms"))->_string, font_type, font_size);
	smsNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* smsNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), smsNormalLabel);

	auto smsSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("sms"))->_string, font_type, font_size);
	smsSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* smsSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), smsSelectLabel);

	MenuItemSprite* menuItemSMS = MenuItemSprite::create(smsNormal, smsSelect, NULL, [&](Ref* sender)
	{
		auto scene = SMSScene::createScene();
		auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	});

	auto shareNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("share"))->_string, font_type, font_size);
	shareNormalLabel->setColor(Color3B(255, 255, 255));
	Sprite* shareNormal = createBlock(buttonNormal, Vec2(0.5, 0.5), shareNormalLabel);

	auto shareSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("share"))->_string, font_type, font_size);
	shareSelectLabel->setColor(Color3B(205, 205, 205));
	Sprite* shareSelect = createBlock(buttonSelect, Vec2(0.5, 0.5), shareSelectLabel);

	MenuItemSprite* menuItemShare = MenuItemSprite::create(shareNormal, shareSelect, NULL, [&](Ref* sender)
	{
		auto scene = ShareScene::createScene();
		auto transitionScene = TransitionSlideInR::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	});

	Menu* menu = Menu::create(menuItemFlash, menuItemSMS, menuItemShare, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		visibleSize.height / 2 + origin.y));
	menu->alignItemsVerticallyWithPadding(30);
	this->addChild(menu);
}
Sprite* GadgetScene::createBlock(std::string buttonName, Point anchor, Label* blockLabel){
	auto button = Sprite::create(buttonName);
	button->setAnchorPoint(anchor);
	button->addChild(blockLabel, 10);
	blockLabel->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
	return button;
}
void GadgetScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = MenuScene::createScene();
		auto transitionScene = TransitionFade::create(switchMenuTime,
			scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU || keyCode == EventKeyboard::KeyCode::KEY_TAB)
	{
	}
}
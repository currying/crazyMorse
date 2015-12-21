#include "Dict.h"
#include "DictList.h"
#include "resources.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
//using namespace tinyxml2;
Dict::Dict(){
}
Dict::~Dict() {
}
Scene* Dict::createScene() {
	auto scene = Scene::create();
	//scene->setTag(classic_mode_scene_tag);
	auto layer = Dict::create();
	scene->addChild(layer);
	return scene;
}

bool Dict::init() {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	if (!LayerColor::initWithColor(Color4B(25, 120, 187, 200)))
	{
		return false;
	}
	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE){
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		font_size = font_dictmenu_size;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_dictmenu_size;
	}
	readDictIndex();
	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(Dict::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	screenShot = ScreenShot::create();
	addChild(screenShot);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!AdmobHelper::isAdShowing)
		AdmobHelper::showAd();
#endif
	return true;
}
void Dict::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = MenuScene::createScene();
		auto transitionScene = TransitionFade::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU || keyCode == EventKeyboard::KeyCode::KEY_TAB)
	{
		auto screenShotCallback = [&](const char* fullPath){
			//根据语言环境加载不同语言,目前只有中英文
			LanguageType nType = Application::getInstance()->getCurrentLanguage();
			__Dictionary *strings;
			if (nType == LanguageType::CHINESE){
				strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
			}
			else{
				strings = __Dictionary::createWithContentsOfFile(il18n_english);
			}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			ShareHelper::share(((__String*)strings->objectForKey("Share"))->getCString(), ((__String*)strings->objectForKey("ShareContent"))->getCString(), fullPath);
#endif
		};
		screenShot->saveScreenshot("share.png", screenShotCallback);
	}
}

void Dict::readDictIndex(){
	sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
	const char * query = "SELECT * FROM [dict_language]";
	sqlite3_stmt * stmt;
	int result = sqlite3_prepare_v2(sqlite, query, -1, &stmt, NULL);
	Vector<MenuItem*> menuItems;
	while (sqlite3_step(stmt) == SQLITE_ROW){
		const char * str = (const char *)sqlite3_column_text(stmt, 1);
		auto normalLabel = Label::createWithTTF(((__String*)strings->objectForKey(str))->_string, font_type, font_size);
		normalLabel->setTag(1);
		normalLabel->setUserData((void*)(str));
		normalLabel->setColor(Color3B(255, 255, 255));
		Sprite* normal = createButton(normalLabel->getContentSize()*1.2, Color3B(0, 0, 0), normalLabel);
		auto selectLabel = Label::createWithTTF(((__String*)strings->objectForKey(str))->_string, font_type, font_size);
		selectLabel->setColor(Color3B(205, 205, 205));
		Sprite* select = createButton(selectLabel->getContentSize()*1.2, Color3B(128, 128, 128), selectLabel);
		auto menuItem = MenuItemSprite::create(normal, select, NULL, [=](Ref* sender)
		{
			auto scene = DictList::createScene((const char*)(((Label*)(normal->getChildByTag(1)))->getUserData()));
			auto transitionScene = TransitionFade::create(switchGameTime, scene);
			auto director = Director::getInstance();
			director->replaceScene(transitionScene);
		});
		menuItems.pushBack(menuItem);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(sqlite);
	Menu* menu = Menu::createWithArray(menuItems);
	menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		visibleSize.height * 2 / 3 + origin.y));
	menu->alignItemsVerticallyWithPadding(30);
	this->addChild(menu);
}
Sprite* Dict::createButton(Size size, Color3B color, Label* label){
	auto button = Sprite::create();
	button->setContentSize(size);
	button->setTextureRect(Rect(0, 0, size.width, size.height));
	button->setColor(color);
	button->addChild(label);
	label->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
	return button;
}

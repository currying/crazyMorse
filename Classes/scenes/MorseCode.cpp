#include "MorseCode.h"
#include "MenuScene.h"
#include "resources.h"
#include "tinyxml2/tinyxml2.h"
#include "resources.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace CocosDenshion;
using namespace tinyxml2;
MorseCode::MorseCode(){
}
MorseCode::~MorseCode() {
}
Scene* MorseCode::createScene(/*const char* language*/) {
	auto scene = Scene::create();
	//scene->setTag(classic_mode_scene_tag);
	auto layer = MorseCode::create(/*language*/);
	scene->addChild(layer);
	return scene;
}
MorseCode* MorseCode::create(/*const char* language*/){
	auto layer = new MorseCode();
	if (layer && layer->init(/*language*/))
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		delete layer;
		layer = NULL;
		return NULL;
	}
}
bool MorseCode::init(/*const char* language*/) {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE){
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_consolas;
		font_size = font_morsecode_size;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_consolas;
		font_size = font_morsecode_size;
	}

	readMorseCode(/*language*/);

	Button* button = Button::create("48X48_2-1.png", "48X48_2-2.png");
	button->setAnchorPoint(Vec2(0.5,0));
	button->addTouchEventListener([&](Ref *pSender, Widget::TouchEventType type){
		switch (type){
		case Widget::TouchEventType::BEGAN:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			AdmobHelper::showGDTAppWall();
#endif
			break;
		default:
			break;
		}
	});
	button->setPosition(Vec2(visibleSize.width / 2, 0));
	this->addChild(button);

	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(MorseCode::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	////触摸监听
	//auto touchListener = EventListenerTouchAllAtOnce::create();
	//touchListener->onTouchesBegan = CC_CALLBACK_2(MorseCode::onTouchesBegan, this);
	//touchListener->onTouchesEnded = CC_CALLBACK_2(MorseCode::onTouchesEnded, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!AdmobHelper::isAdShowing)
		AdmobHelper::showAd();
#endif
	return true;
}
void MorseCode::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = MenuScene::createScene();
		auto transitionScene = TransitionFade::create(switchMenuTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU || keyCode == EventKeyboard::KeyCode::KEY_TAB)
	{
		auto screenShotCallback = [&](bool succeed, const std::string fullPath){
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
			ShareHelper::share(((__String*)strings->objectForKey("Share"))->getCString(), ((__String*)strings->objectForKey("ShareContent"))->getCString(), fullPath.c_str());
#endif
		};
		utils::captureScreen(screenShotCallback, "share.png");
	}
	}

//void MorseCode::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
//{
//}
//void MorseCode::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
//{
//}
void MorseCode::readMorseCode(/*const char* language*/){
	sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
	const char * query = "SELECT * FROM [morse_code]";
	sqlite3_stmt * stmt;
	int result = sqlite3_prepare_v2(sqlite, query, -1, &stmt, NULL);
			int i = 0;
			int j = 0;
			int rows = 15;
	while (sqlite3_step(stmt) == SQLITE_ROW){
		const char * name = (const char *)sqlite3_column_text(stmt, 2);
		const char * value = (const char *)sqlite3_column_text(stmt, 3);
		auto nameLabel = Label::createWithSystemFont(name, font_type, font_size);
		nameLabel->setColor(Color3B(0, 0, 0));
		nameLabel->setAnchorPoint(Vec2(0, 0));
		nameLabel->setPosition(Vec2((visibleSize.width / 10) + (floor(i / rows)*(visibleSize.width / 6)), visibleSize.height - nameLabel->getContentSize().height * 5 - nameLabel->getContentSize().height*j));
		i++;
		j++;
		if (j >= rows)
			j = 0;
		addChild(nameLabel);
		auto valueLabel = Label::createWithSystemFont(value, font_type, font_size);
		valueLabel->setAnchorPoint(Vec2(0, 0));
		valueLabel->setColor(Color3B(0, 0, 0));
		valueLabel->setPosition(Vec2(nameLabel->getPositionX() + nameLabel->getContentSize().width + 10, nameLabel->getPositionY()));
		addChild(valueLabel);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(sqlite);




	//tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument();
	//xmlDoc->LoadFile((FileUtils::getInstance()->getWritablePath() + morseCode).c_str());
	//tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();
	//XMLElement* languageElement = rootElement->FirstChildElement("language");
	//while (languageElement){
	//	const XMLAttribute *languageAttribute = languageElement->FirstAttribute();
	//	const char* attribute = languageAttribute->Value();
	//	if (strcmp(attribute, language) == 0){
	//		XMLElement* codeElement = languageElement->FirstChildElement();
	//		int i = 0;
	//		int j = 0;
	//		int rows = 15;
	//		while (codeElement){
	//			const XMLAttribute *nameAttribute = codeElement->FirstAttribute();
	//			auto nameLabel = Label::createWithSystemFont(nameAttribute->Value(), font_type, font_size);
	//			nameLabel->setColor(Color3B(0,0,0));
	//			nameLabel->setAnchorPoint(Vec2(0,0));
	//			nameLabel->setPosition(Vec2((visibleSize.width / 10) + (floor(i / rows)*(visibleSize.width / 6)), visibleSize.height - nameLabel->getContentSize().height * 5 - nameLabel->getContentSize().height*j));
	//			i++;
	//			j++;
	//			if (j >= rows)
	//				j = 0;
	//			addChild(nameLabel);
	//			const XMLAttribute *valueAttribute = nameAttribute->Next();
	//			auto valueLabel = Label::createWithSystemFont(valueAttribute->Value(), font_type, font_size);
	//			valueLabel->setAnchorPoint(Vec2(0, 0));
	//			valueLabel->setColor(Color3B(0, 0, 0));
	//			valueLabel->setPosition(Vec2(nameLabel->getPositionX() + nameLabel->getContentSize().width+10, nameLabel->getPositionY()));
	//			addChild(valueLabel);
	//			codeElement = codeElement->NextSiblingElement();
	//		}
	//	}
	//	languageElement = languageElement->NextSiblingElement();
	//}
}
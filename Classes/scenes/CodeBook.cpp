#include "CodeBook.h"
#include "MorseCode.h"
#include "resources.h"
#include "tinyxml2/tinyxml2.h"
#include "resources.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace tinyxml2;
CodeBook::CodeBook(){
}
CodeBook::~CodeBook() {
}
Scene* CodeBook::createScene() {
	auto scene = Scene::create();
	//scene->setTag(classic_mode_scene_tag);
	auto layer = CodeBook::create();
	scene->addChild(layer);
	return scene;
}

bool CodeBook::init() {
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
		font_type = font_droid_sans_fallback;
		font_size = font_codebook_size;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_codebook_size;
	}
	readCodeBook();
	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(CodeBook::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	////触摸监听
	//auto touchListener = EventListenerTouchAllAtOnce::create();
	//touchListener->onTouchesBegan = CC_CALLBACK_2(CodeBook::onTouchesBegan, this);
	//touchListener->onTouchesEnded = CC_CALLBACK_2(CodeBook::onTouchesEnded, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	screenShot = ScreenShot::create();
	addChild(screenShot);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!AdmobHelper::isAdShowing)
		AdmobHelper::showAd();
#endif
	return true;
}
void CodeBook::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
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

void CodeBook::readCodeBook(){
	//tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument();
	//xmlDoc->LoadFile((FileUtils::getInstance()->getWritablePath() + morseCode).c_str());
	//tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();
	//XMLElement* languageElement = rootElement->FirstChildElement("language");
	//Vector<MenuItem*> menuItems;
	//while (languageElement){
	//	const XMLAttribute *languageAttribute = languageElement->FirstAttribute();

	//	auto normalLabel = Label::createWithTTF(((__String*)strings->objectForKey(languageAttribute->Value()))->_string, font_type, font_size);
	//	normalLabel->setTag(1);
	//	normalLabel->setUserData((void*)(languageAttribute->Value()));
	//	normalLabel->setColor(Color3B(255, 255, 255));
	//	Sprite* normal = createButton(normalLabel->getContentSize()*1.2, Color3B(0, 0, 0), normalLabel);

	//	auto selectLabel = Label::createWithTTF(((__String*)strings->objectForKey(languageAttribute->Value()))->_string, font_type, font_size);
	//	selectLabel->setColor(Color3B(205, 205, 205));
	//	Sprite* select = createButton(selectLabel->getContentSize()*1.2, Color3B(128, 128, 128), selectLabel);

	//	auto menuItem = MenuItemSprite::create(normal, select, NULL, [=](Ref* sender)
	//	{
	//		//auto scene = MorseCode::createScene((const char*)(((Label*)(normal->getChildByTag(1)))->getUserData()));
	//		//auto transitionScene = TransitionFade::create(switchGameTime, scene);
	//		//auto director = Director::getInstance();
	//		//director->replaceScene(transitionScene);
	//	});
	//	menuItems.pushBack(menuItem);
	//	languageElement = languageElement->NextSiblingElement();
	//}
	//Menu* menu = Menu::createWithArray(menuItems);
	//menu->setPosition(Vec2(visibleSize.width / 2 + origin.x,
	//	visibleSize.height * 2 / 3 + origin.y));
	//menu->alignItemsVerticallyWithPadding(20);
	//this->addChild(menu);
}

Sprite* CodeBook::createButton(Size size, Color3B color, Label* blockLabel){
	auto button = Sprite::create();
	button->setContentSize(size);
	button->setTextureRect(Rect(0, 0, size.width, size.height));
	button->setColor(color);
	button->addChild(blockLabel);
	blockLabel->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
	return button;
}

#include "GameConfigureScene.h"
#include "resources.h"
#include "MenuScene.h"
#include "tinyxml2/tinyxml2.h"
#include "TransmitterMode.h"
#include "LeisureMode.h"
using namespace tinyxml2;
GameConfigureScene::GameConfigureScene() {
}
GameConfigureScene::~GameConfigureScene() {
}
Scene* GameConfigureScene::createScene() {
	auto scene = Scene::create();
	auto layer = GameConfigureScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameConfigureScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased =
		CC_CALLBACK_2(GameConfigureScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE) {
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		font_size = font_menuitem_size;
	}
	else {
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_menuitem_size;
	}
	createConfigure();//初始化运行配置文件，如果没有就新建
	pageView = CustomPageView::create();
	pageView->setTouchEnabled(true);
	pageView->setContentSize(visibleSize);
	pageView->setPosition(Point::ZERO);
	createCustomPage();
	pageView->addEventListener(CC_CALLBACK_2(GameConfigureScene::pageViewEvent, this));
	addChild(pageView);
	return true;
}
void GameConfigureScene::pageViewEvent(Ref *pSender, PageView::EventType type) {

}
void GameConfigureScene::createCustomPage() {
	createIdentitySelectPage();
	createDictSelectPage();
	createLevelSelectPage();
	pageView->addPage(identitySelectlayout);
	pageView->addPage(dictSelectlayout);
	pageView->addPage(levelSelectlayout);
}
void GameConfigureScene::createIdentitySelectPage() {
	identitySelectlayout = Layout::create();
	identitySelectlayout->setContentSize(visibleSize);
	auto identituSelcetLabel = Label::createWithTTF(
		((__String *)strings->objectForKey("SelectIdentity"))->_string,
		font_type, 48);
	identituSelcetLabel->setColor(Color3B(0, 0, 0));
	identituSelcetLabel->setPosition(
		Vec2(identitySelectlayout->getContentSize().width / 2,
		identitySelectlayout->getContentSize().height * 12 / 15));
	identitySelectlayout->addChild(identituSelcetLabel);

	auto leisureNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Leisure"))->_string, font_type, font_size);
	leisureNormalLabel->setColor(Color3B(0, 0, 0));

	auto leisureSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Leisure"))->_string, font_type, font_size);
	leisureSelectLabel->setColor(Color3B(128, 128, 128));

	MenuItemSprite* menuItemLeisure = MenuItemSprite::create(leisureNormalLabel, leisureSelectLabel, NULL, [&](Ref* sender)
	{
		modifyConfigure("Leisure", nullptr, nullptr);
		pageView->scrollToPage(pageView->getCurPageIndex() + 1);
	});
	menuItemLeisure->setPosition(
		Vec2(identitySelectlayout->getContentSize().width / 2,
		identitySelectlayout->getContentSize().height * 9 / 15));

	auto transmitterNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Transmitter"))->_string, font_type, font_size);
	transmitterNormalLabel->setColor(Color3B(0, 0, 0));

	auto transmitterSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Transmitter"))->_string, font_type, font_size);
	transmitterSelectLabel->setColor(Color3B(128, 128, 128));

	MenuItemSprite* menuItemTransmitter = MenuItemSprite::create(transmitterNormalLabel, transmitterSelectLabel, NULL, [this](Ref* sender)
	{
		modifyConfigure("Transmitter", nullptr, nullptr);
		pageView->scrollToPage(pageView->getCurPageIndex() + 1);
	});
	menuItemTransmitter->setPosition(
		Vec2(identitySelectlayout->getContentSize().width / 2,
		identitySelectlayout->getContentSize().height * 7 / 15));

	auto listenerNormalLabel = Label::createWithTTF(((__String*)strings->objectForKey("Listener"))->_string, font_type, font_size);
	listenerNormalLabel->setColor(Color3B(0, 0, 0));

	auto listenerSelectLabel = Label::createWithTTF(((__String*)strings->objectForKey("Listener"))->_string, font_type, font_size);
	listenerSelectLabel->setColor(Color3B(128, 128, 128));

	MenuItemSprite* menuItemListener = MenuItemSprite::create(listenerNormalLabel, listenerSelectLabel, NULL, [&](Ref* sender)
	{
		modifyConfigure("Listener", nullptr, nullptr);
		pageView->scrollToPage(pageView->getCurPageIndex() + 1);
	});
	menuItemListener->setPosition(
		Vec2(identitySelectlayout->getContentSize().width / 2,
		identitySelectlayout->getContentSize().height * 5 / 15));
	menuItemListener->setEnabled(false);

	Menu* menu = Menu::create(menuItemLeisure, menuItemTransmitter, menuItemListener, NULL);
	menu->setPosition(Point::ZERO);
	identitySelectlayout->addChild(menu);
}
void GameConfigureScene::createDictSelectPage() {
	dictSelectlayout = Layout::create();
	dictSelectlayout->setContentSize(visibleSize);
	auto dictSelectLabel = Label::createWithTTF(
		((__String *)strings->objectForKey("SelectLanguage"))->_string,
		font_type, 48);
	dictSelectLabel->setColor(Color3B(0, 0, 0));
	dictSelectLabel->setPosition(
		Vec2(dictSelectlayout->getContentSize().width / 2,
		dictSelectlayout->getContentSize().height * 12 / 15));
	dictSelectlayout->addChild(dictSelectLabel);
	readDictIndex();
}
void GameConfigureScene::createLevelSelectPage(){
	levelSelectlayout = Layout::create();
	levelSelectlayout->setContentSize(visibleSize);
	auto levelSelectLabel = Label::createWithTTF(
		((__String *)strings->objectForKey("SelectLevel"))->_string,
		font_type, 48);
	levelSelectLabel->setColor(Color3B(0, 0, 0));
	levelSelectLabel->setPosition(
		Vec2(levelSelectlayout->getContentSize().width / 2,
		levelSelectlayout->getContentSize().height * 12 / 15));
	levelSelectlayout->addChild(levelSelectLabel);
	startButton = Button::create("start.png");
	startButton->setPosition(Vec2(levelSelectlayout->getContentSize().width - startButton->getContentSize().width * 2 / 3, startButton->getContentSize().height));
	startButton->setVisible(false);
	startButton->addTouchEventListener(CC_CALLBACK_2(GameConfigureScene::startGame, this));
	levelSelectlayout->addChild(startButton);
	readLevel();
}
void GameConfigureScene::startGame(Ref * pObjc, Widget::TouchEventType type){
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	pDoc->LoadFile((FileUtils::getInstance()->getWritablePath() + run_configure).c_str());
	XMLElement *rootEle = pDoc->RootElement();
	const XMLAttribute* identityAttribute = rootEle->FirstAttribute();
	const XMLAttribute* languageAttribute = identityAttribute->Next();
	const XMLAttribute* levelAttribute = languageAttribute->Next();
	if (strcmp(identityAttribute->Value(), "Transmitter") == 0){
		auto scene = TransmitterMode::createScene(languageAttribute->Value(), levelAttribute->Value());
		auto transitionScene = TransitionFade::create(startGameTime,
			scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (strcmp(identityAttribute->Value(), "Listener")==0){
	}
	else if (strcmp(identityAttribute->Value(), "Leisure") == 0){
		auto scene = LeisureMode::createScene(languageAttribute->Value(), levelAttribute->Value());
		auto transitionScene = TransitionFade::create(startGameTime,
			scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	}
void GameConfigureScene::readLevel(){
	//Vector<MenuItem*> menuItems;
	float i = 0.0;
	for (auto item : levels_){
		//auto normalLabel = Label::createWithTTF(((__String*)strings->objectForKey(level))->_string, font_type, font_size);
		////normalLabel->setTag(1);
		////normalLabel->setUserData((void*)(language.c_str()));
		//normalLabel->setColor(Color3B(0, 0, 0));

		//auto selectLabel = Label::createWithTTF(((__String*)strings->objectForKey(level))->_string, font_type, font_size);
		//selectLabel->setColor(Color3B(128, 128, 128));

		//auto menuItem = MenuItemSprite::create(normalLabel, selectLabel, NULL, [=](Ref* sender)
		//{
		//	modifyConfigure(nullptr, nullptr, level);
		//	pageView->scrollToPage(pageView->getCurPageIndex() + 1);
		//});
		auto btn = Button::create("button.png");
		btn->setTitleText(((__String*)strings->objectForKey(item))->_string);
		btn->setTitleFontSize(36);
		btn->setUserData((void*)item);
		btn->setTouchEnabled(true);
		btn->addTouchEventListener(CC_CALLBACK_2(GameConfigureScene::onFocus, this));
		btn->setPosition(
			Vec2(dictSelectlayout->getContentSize().width / 2,
			dictSelectlayout->getContentSize().height * (10.0f - i) / 15.0f));
		i += 1.3f;
		//menuItems.pushBack(menuItem);
		levelSelectlayout->addChild(btn);
		buttons.pushBack(btn);
}
	//Menu* menu = Menu::createWithArray(menuItems);
	//menu->setPosition(Point::ZERO);
	//levelSelectlayout->addChild(menu);
}
void GameConfigureScene::onFocus(Ref * pObjc, Widget::TouchEventType type){
	for (auto button : buttons){
		button->setFocusEnabled(false);
		button->setColor(Color3B::WHITE);
	}
	modifyConfigure(nullptr, nullptr, static_cast<const char*>(((Button*)pObjc)->getUserData()));
	((Button*)pObjc)->setFocused(true);
	((Button*)pObjc)->setColor(Color3B(88, 88, 88));
	startButton->setVisible(true);
}
void GameConfigureScene::onKeyReleased(EventKeyboard::KeyCode keyCode,
	Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_ESCAPE
		|| keyCode == EventKeyboard::KeyCode::KEY_SYSREQ) //返回
	{
		if (pageView->getCurPageIndex() > 0)
			pageView->scrollToPage(pageView->getCurPageIndex() - 1);
		else {
			auto scene = MenuScene::createScene();
			auto transitionScene = TransitionFade::create(switchMenuTime,
				scene);
			auto director = Director::getInstance();
			director->replaceScene(transitionScene);
		}
	}
}

void GameConfigureScene::readDictIndex(){
	Vector<MenuItem*> menuItems;
	int i = 0;
	for (const char* language : dictLanguages){
		auto normalLabel = Label::createWithTTF(((__String*)strings->objectForKey(language))->_string, font_type, font_size);
		//normalLabel->setTag(1);
		//normalLabel->setUserData((void*)(language.c_str()));
		normalLabel->setColor(Color3B(0, 0, 0));

		auto selectLabel = Label::createWithTTF(((__String*)strings->objectForKey(language))->_string, font_type, font_size);
		selectLabel->setColor(Color3B(128, 128, 128));
		auto menuItem = MenuItemSprite::create(normalLabel, selectLabel, NULL, [=](Ref* sender)
		{
			modifyConfigure(nullptr, language, nullptr);
			pageView->scrollToPage(pageView->getCurPageIndex() + 1);
		});
		menuItem->setPosition(
			Vec2(dictSelectlayout->getContentSize().width / 2,
			dictSelectlayout->getContentSize().height * (9 - i) / 15));
		i += 2;
		menuItems.pushBack(menuItem);
	}
	Menu* menu = Menu::createWithArray(menuItems);
	menu->setPosition(Point::ZERO);
	dictSelectlayout->addChild(menu);
}
void GameConfigureScene::createConfigure(){
	if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + run_configure)){
		tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
		if (NULL == pDoc) {
			return;
		}
		//XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		//if (NULL == pDel) {
		//	return;
		//}
		//pDoc->LinkEndChild(pDel);
		XMLElement *plistElement = pDoc->NewElement("configure");
		plistElement->SetAttribute("identity", "");
		plistElement->SetAttribute("language", "");
		plistElement->SetAttribute("level", "");
		pDoc->LinkEndChild(plistElement);
		pDoc->SaveFile((FileUtils::getInstance()->getWritablePath() + run_configure).c_str(), true);
		delete pDoc;
	}
}

void GameConfigureScene::modifyConfigure(const char* identity, const char* language, const char* level){
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	pDoc->LoadFile((FileUtils::getInstance()->getWritablePath() + run_configure).c_str());
	XMLElement *rootEle = pDoc->RootElement();
	if (identity){
		rootEle->SetAttribute("identity", identity);
	}
	if (language)
		rootEle->SetAttribute("language", language);
	if (level)
		rootEle->SetAttribute("level", level);
	pDoc->SaveFile((FileUtils::getInstance()->getWritablePath() + run_configure).c_str(), true);
	delete pDoc;
}
//sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
//const char * query = "SELECT * FROM [dict_language]";
//sqlite3_stmt * stmt;
//int result = sqlite3_prepare_v2(sqlite, query, -1, &stmt, NULL);
//int i = 0;
//while (sqlite3_step(stmt) == SQLITE_ROW){
//	const char * str = (const char *)sqlite3_column_text(stmt, 1);
//	auto normalLabel = Label::createWithTTF(((__String*)strings->objectForKey(str))->_string, font_type, font_size);
//	normalLabel->setTag(1);
//	normalLabel->setUserData((void*)(str));
//	normalLabel->setColor(Color3B(0, 0, 0));
//	auto selectLabel = Label::createWithTTF(((__String*)strings->objectForKey(str))->_string, font_type, font_size);
//	selectLabel->setColor(Color3B(128, 128, 128));
//	auto menuItem = MenuItemSprite::create(normalLabel, selectLabel, NULL, [=](Ref* sender)
//	{
//	});
//	normalLabel->setPosition(
//		Vec2(dictSelectlayout->getSize().width / 2,
//		dictSelectlayout->getSize().height * (9 - i) / 15));
//	i += 2;
//	menuItems.pushBack(menuItem);
//}
//sqlite3_finalize(stmt);
//sqlite3_close(sqlite);
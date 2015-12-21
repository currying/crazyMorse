#include "FlashScene.h"
#include "GadgetScene.h"
USING_NS_CC;
FlashScene::FlashScene() :i(0){
}
FlashScene::~FlashScene() {
}
Scene* FlashScene::createScene() {
	auto scene = Scene::create();
	auto layer = FlashScene::create();
	scene->addChild(layer);
	return scene;
}
bool FlashScene::init() {
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE){
		font_type = font_droid_sans_fallback;
	}
	else{
		font_type = font_consolas;
	}


	//键盘监听
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(FlashScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	initData();
	createListView();
	return true;
}
void FlashScene::initData() {
	sqlite3_open(
		(FileUtils::getInstance()->getWritablePath() + morseData).c_str(),
		&sqlite);
	std::string select_from_common = std::string("SELECT * FROM [common] ;");
	sqlite3_stmt * stmt;
	sqlite3_prepare_v2(sqlite, select_from_common.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const char * row = (const char *)sqlite3_column_text(stmt, 0);
		const char * value = (const char *)sqlite3_column_text(stmt, 1);
		const char * morseCode = (const char *)sqlite3_column_text(stmt, 2);
		LanguageType nType = Application::getInstance()->getCurrentLanguage();
		if (nType == LanguageType::CHINESE) {
			const char * translation = (const char *)sqlite3_column_text(stmt,
				4);
			commons.push_back({ value, morseCode, translation });
		}
		else {
			const char * translation = (const char *)sqlite3_column_text(stmt,
				3);
			commons.push_back({ value, morseCode, translation });
		}

	}
}
void FlashScene::createListView() {
	ListView* listView = ListView::create();
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	listView->setBounceEnabled(true);
	listView->setContentSize(
		Size(visibleSize.width, visibleSize.height * 5 / 6));
	listView->setPosition(Vec2(visibleSize.width / 30, 0));
	listView->addEventListener(
		(ui::ListView::ccListViewCallback) CC_CALLBACK_2(
		FlashScene::selectedItemEvent, this));
	listView->addEventListener(
		(ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(
		FlashScene::selectedItemEventScrollView, this));
	listView->setGravity(ListView::Gravity::CENTER_VERTICAL);
	listView->setItemsMargin(5.0f);
	this->addChild(listView);
	for (auto common : commons) {
		Button* button = Button::create("button_b.png", "button_p.png");
		button->setTitleText(common.value + "(" + common.translation + ")");
		button->setTitleFontSize(24);
		button->setTitleFontName(font_type);
		Layout* item = Layout::create();
		item->setTouchEnabled(true);
		item->setContentSize(button->getContentSize());
		button->setPosition(
			Vec2(item->getContentSize().width / 2.0f,
			item->getContentSize().height / 2.0f));
		item->addChild(button);
		listView->addChild(item);
	}
}
void FlashScene::selectedItemEvent(Ref * pObjc, ListView::EventType type) {
	switch (type) {
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END: {
		ListView* listView = static_cast<ListView*>(pObjc);
		currentIndex = listView->getCurSelectedIndex();
		this->stopAllActions();
		i = 0;
		this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(FlashScene::flash, this)), NULL));
		break;
	}
	default:
		break;
	}
}
void FlashScene::selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type){
}
void FlashScene::flash() {
	if (i <= strlen(commons.at(currentIndex).code.c_str()) - 1){
		if (commons.at(currentIndex).code[i] == '.') {
			executeDit();
		}
		else if (commons.at(currentIndex).code[i] == '-') {
			executeDah();
		}
		else if (commons.at(currentIndex).code[i] == ' ') {
			executeSpace();
		}
		else if (commons.at(currentIndex).code[i] == '/') {
			executeDiscontinuous();
		}
		i++;
	}
	else{
		i = 0;
	}
}
void FlashScene::executeDit() {
	this->runAction(
		Sequence::create(
		CallFunc::create(CC_CALLBACK_0(FlashScene::open, this)),
		DelayTime::create(base_time),
		CallFunc::create(CC_CALLBACK_0(FlashScene::close, this)),
		DelayTime::create(base_time),
		CallFunc::create(CC_CALLBACK_0(FlashScene::flash, this)), NULL));
}
void FlashScene::executeDah() {
	this->runAction(
		Sequence::create(
		CallFunc::create(CC_CALLBACK_0(FlashScene::open, this)),
		DelayTime::create(3 * base_time),
		CallFunc::create(CC_CALLBACK_0(FlashScene::close, this)),
		DelayTime::create(base_time),
		CallFunc::create(CC_CALLBACK_0(FlashScene::flash, this)), NULL));
}
void FlashScene::executeSpace() {
	this->runAction(Sequence::create(DelayTime::create(2 * base_time), CallFunc::create(CC_CALLBACK_0(FlashScene::flash, this)), NULL));
}
void FlashScene::executeDiscontinuous() {
	this->runAction(Sequence::create(DelayTime::create(6 * base_time), CallFunc::create(CC_CALLBACK_0(FlashScene::flash, this)), NULL));
}
void FlashScene::open() {
	//log("open");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FlashHelper::openFlash();
#endif
}
void FlashScene::close() {
	//log("close");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	FlashHelper::closeFlash();
#endif
}
void FlashScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_ESCAPE
		|| keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		this->stopAllActions();
		this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(FlashScene::close, this)), NULL));
		auto scene = GadgetScene::createScene();
		auto transitionScene = TransitionFade::create(switchMenuTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU
		|| keyCode == EventKeyboard::KeyCode::KEY_TAB) {
	}
}

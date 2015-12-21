#include "SMSScene.h"
#include "GadgetScene.h"
USING_NS_CC;

Scene* SMSScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SMSScene::create();
	scene->addChild(layer);
	return scene;
}
bool SMSScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
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
	listener->onKeyReleased = CC_CALLBACK_2(SMSScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	initData();
	createListView();
	return true;
}
void SMSScene::initData() {
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
void SMSScene::createListView() {
	ListView* listView = ListView::create();
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	listView->setBounceEnabled(true);
	listView->setContentSize(
		Size(visibleSize.width, visibleSize.height * 5 / 6));
	listView->setPosition(Vec2(visibleSize.width / 30, 0));
	listView->addEventListener(
		(ui::ListView::ccListViewCallback) CC_CALLBACK_2(
		SMSScene::selectedItemEvent, this));
	listView->addEventListener(
		(ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(
		SMSScene::selectedItemEventScrollView, this));
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
void SMSScene::selectedItemEvent(Ref * pObjc, ListView::EventType type) {
	switch (type) {
	case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END: {
		ListView* listView = static_cast<ListView*>(pObjc);
		currentIndex = listView->getCurSelectedIndex();
		this->runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(SMSScene::sendSMS, this)), NULL));
		break;
	}
	default:
		break;
	}
}
void SMSScene::selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type){
}
void SMSScene::sendSMS() {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	SMSHelper::sendSMS(commons.at(currentIndex).code.c_str(),"13775362742");
//#endif
}
void SMSScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = GadgetScene::createScene();
		auto transitionScene = TransitionFade::create(switchMenuTime,
			scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU || keyCode == EventKeyboard::KeyCode::KEY_TAB)
	{
	}
}
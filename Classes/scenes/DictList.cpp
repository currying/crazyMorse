#include "DictList.h"
#include "Dict.h"
#include "resources.h"
#include "tinyxml2/tinyxml2.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace tinyxml2;
DictList::DictList(){
}
DictList::~DictList() {
}
Scene* DictList::createScene(std::string dictListName) {
	auto scene = Scene::create();
	//scene->setTag(classic_mode_scene_tag);
	auto layer = DictList::create(dictListName);
	scene->addChild(layer);
	return scene;
}
DictList* DictList::create(std::string dictListName){
	auto layer = new DictList();
	if (layer && layer->init(dictListName))
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
bool DictList::init(std::string dictListName) {
	dictListPath = FileUtils::getInstance()->getWritablePath() + dictListName;
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
		font_type = font_consolas;
		font_size = font_dictlist_size;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_consolas;
		font_size = font_dictlist_size;
	}
	listView = ListView::create();
	// set list view ex direction
	listView->setDirection(ui::ScrollView::Direction::VERTICAL);
	listView->setTouchEnabled(true);
	listView->setBounceEnabled(false);
	listView->setContentSize(Size(visibleSize.width * 3 / 7, visibleSize.height * 5 / 7));
	listView->setPosition(Point(visibleSize.width / 7, visibleSize.height / 7));
	listView->addEventListener((ui::ListView::ccListViewCallback)(CC_CALLBACK_2(DictList::selectedItemEvent, this)));
	this->addChild(listView);
	readDictList(dictListPath);

	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(DictList::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//触摸监听
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(DictList::onTouchesBegan, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(DictList::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	screenShot = ScreenShot::create();
	addChild(screenShot);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (!AdmobHelper::isAdShowing)
		AdmobHelper::showAd();
#endif
	return true;
}
void DictList::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = Dict::createScene();
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

void DictList::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
{
}
void DictList::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *unused_event)
{
}
void DictList::readDictList(std::string dictListPath){
	if (!FileUtils::getInstance()->isFileExist(dictListPath)){
		createDictList();
	}
	tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument();
	xmlDoc->LoadFile(dictListPath.c_str());
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();
	XMLElement* wordElement = rootElement->FirstChildElement("word");
	while (wordElement){
		const XMLAttribute *wordAttribute = wordElement->FirstAttribute();
		auto label = Label::createWithSystemFont(wordAttribute->Value(), font_type, font_size);
		Layout* default_item = Layout::create();
		default_item->setTouchEnabled(false);
		default_item->setContentSize(label->getContentSize());
		label->setPosition(Vec2(default_item->getContentSize().width / 2.0f,
			default_item->getContentSize().height / 2.0f));
		default_item->addChild(label);
		listView->pushBackCustomItem(default_item);
		wordElement = wordElement->NextSiblingElement();
	}
}
void DictList::createDictList(){
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc) {
		return;
	}
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL == pDel) {
		return;
	}
	pDoc->LinkEndChild(pDel);
	XMLElement *plistElement = pDoc->NewElement("dict");
	pDoc->LinkEndChild(plistElement);
	pDoc->SaveFile(dictListPath.c_str(), true);
	delete pDoc;
}
void DictList::selectedItemEvent(Ref *pSender, ListView::EventType type)
{
}

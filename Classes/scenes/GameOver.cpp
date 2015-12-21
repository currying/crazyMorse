#include "GameOver.h"
#include "resources.h"
#include "tinyxml2/tinyxml2.h"
#include "LeisureMode.h"
#include "GameConfigureScene.h"
#include "MenuScene.h"
USING_NS_CC;
using namespace tinyxml2;
using namespace CocosDenshion;
GameOver::GameOver() :baseScore(0), sumTime(0.0f), finishTime(0.0f), sumTimes(0), errorTimes(0), missTimes(0){
}
GameOver::~GameOver() {
}
Scene* GameOver::createScene() {
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

bool GameOver::init() {
	if (!LayerColor::initWithColor(Color4B(255, 0, 0, 255)))
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//监听退出按键
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameOver::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE){
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		font_size = font_gameover_size;
	}
	else{
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		font_size = font_gameover_size;
	}
	readScore();
	readConfig();
	//标题
	auto titleLabel = Label::createWithTTF(((__String*)strings->objectForKey(mode + std::string("Mode")))->_string, font_type, font_size);
	titleLabel->setColor(Color3B(0, 0, 0));
	titleLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 16 / 18));
	addChild(titleLabel);
	//等级
	auto levelLabel = Label::createWithTTF(((__String*)strings->objectForKey("Level"))->_string + ": " + ((__String*)strings->objectForKey(level))->_string, font_type, 30);
	levelLabel->setColor(Color3B(0, 0, 0));
	levelLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 15 / 18));
	addChild(levelLabel);
	//基础分数
	auto baseScoreLabel = Label::createWithTTF(((__String*)strings->objectForKey("BaseScore"))->_string + __String::createWithFormat("%d", baseScore)->_string, font_type, 30);
	baseScoreLabel->setColor(Color3B(255, 255, 255));
	baseScoreLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 14 / 18));
	addChild(baseScoreLabel);
	//总时间
	auto sumTimeLabel = Label::createWithTTF(((__String*)strings->objectForKey("SumTime"))->_string + __String::createWithFormat("%.3f", sumTime)->_string, font_type, 30);
	sumTimeLabel->setColor(Color3B(255, 255, 255));
	sumTimeLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 13 / 18));
	addChild(sumTimeLabel);
	//完成时间
	auto finishTimeLabel = Label::createWithTTF(((__String*)strings->objectForKey("FinishTime"))->_string + __String::createWithFormat("%.3f", finishTime)->_string, font_type, 30);
	finishTimeLabel->setColor(Color3B(255, 255, 255));
	finishTimeLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 12 / 18));
	addChild(finishTimeLabel);
	//总字符数
	auto sumTimesLabel = Label::createWithTTF(((__String*)strings->objectForKey("SumTimes"))->_string + __String::createWithFormat("%d", sumTimes)->_string, font_type, 30);
	sumTimesLabel->setColor(Color3B(255, 255, 255));
	sumTimesLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 11 / 18));
	addChild(sumTimesLabel);
	//错误字符数
	auto errorTimesLabel = Label::createWithTTF(((__String*)strings->objectForKey("ErrorTimes"))->_string + __String::createWithFormat("%d", errorTimes)->_string, font_type, 30);
	errorTimesLabel->setColor(Color3B(255, 255, 255));
	errorTimesLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 10 / 18));
	addChild(errorTimesLabel);
	//漏掉字符数
	auto missTimesLabel = Label::createWithTTF(((__String*)strings->objectForKey("MissTimes"))->_string + __String::createWithFormat("%d", missTimes)->_string, font_type, 30);
	missTimesLabel->setColor(Color3B(255, 255, 255));
	missTimesLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 9 / 18));
	addChild(missTimesLabel);
	//速打比例
	auto rcrLabel = Label::createWithTTF(((__String*)strings->objectForKey("RCR"))->_string + __String::createWithFormat("%.3f", ((finishTime / sumTime) * 100))->_string + "%", font_type, 30);
	rcrLabel->setColor(Color3B(255, 255, 255));
	rcrLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 8 / 18));
	addChild(rcrLabel);
	//错误率
	auto erLabel = Label::createWithTTF(((__String*)strings->objectForKey("ER"))->_string + __String::createWithFormat("%.3f", (((float)errorTimes / (float)sumTimes) * 100))->_string + "%", font_type, 30);
	erLabel->setColor(Color3B(255, 255, 255));
	erLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 7 / 18));
	addChild(erLabel);
	//失误率
	auto mrLabel = Label::createWithTTF(((__String*)strings->objectForKey("MR"))->_string + __String::createWithFormat("%.3f", (((float)missTimes / (float)sumTimes) * 100))->_string + "%", font_type, 30);
	mrLabel->setColor(Color3B(255, 255, 255));
	mrLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 6 / 18));
	addChild(mrLabel);
	//综合评分
	float highest = (((baseScore * 100) / (finishTime / sumTime))*((float(sumTimes - errorTimes) / (float)sumTimes))*(float(sumTimes - missTimes) / (float)sumTimes));
	auto csLabel = Label::createWithTTF(((__String*)strings->objectForKey("CS"))->_string + __String::createWithFormat("%.3f", highest)->_string, font_type, 48);
	csLabel->setColor(Color3B(255, 255, 255));
	csLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 4 / 18));
	addChild(csLabel);
	float hotmh = readHighest(mode, level);
	if (hotmh != 0){
		auto hotmhLabel = Label::createWithTTF(((__String*)strings->objectForKey("HOTMH"))->_string + __String::createWithFormat("%.3f", hotmh)->_string, font_type, 30);
		hotmhLabel->setColor(Color3B(255, 255, 255));
		hotmhLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height * 3 / 18));
		addChild(hotmhLabel);
	}
	if (highest > hotmh){
		storeHighest(mode, level, highest);
	}

	auto showOfflLabel = Label::createWithTTF(((__String*)strings->objectForKey("ShowOff"))->_string, font_type, font_size);
	showOfflLabel->setColor(Color3B(0, 0, 0));
	MenuItemSprite* menuItemShowOff = MenuItemSprite::create(showOfflLabel, NULL, [&](Ref* sender)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if (AdmobHelper::isAdShowing)
			AdmobHelper::hideAd();
#endif
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
	});
	//

	auto backLabel = Label::createWithTTF(((__String*)strings->objectForKey("Back"))->_string, font_type, font_size);
	backLabel->setColor(Color3B(0, 0, 0));
	MenuItemSprite* menuItemBack = MenuItemSprite::create(backLabel, NULL, [&](Ref* sender)
	{
		auto scene = GameConfigureScene::createScene();
		auto transitionScene = TransitionFade::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	});

	auto praiselLabel = Label::createWithTTF(((__String*)strings->objectForKey("Praise"))->_string, font_type, font_size);
	praiselLabel->setColor(Color3B(0, 0, 0));
	MenuItemSprite* menuItemPraise = MenuItemSprite::create(praiselLabel, NULL, [&](Ref* sender)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		ShareHelper::praise();
#endif
	});


	Menu* menu = Menu::create(menuItemShowOff, menuItemBack, menuItemPraise, NULL);
	menu->setPosition(Point(visibleSize.width / 2, visibleSize.height * 1 / 18));
	menu->alignItemsHorizontallyWithPadding(30);
	addChild(menu);
	return true;
}
void GameOver::readConfig(){
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	pDoc->LoadFile((FileUtils::getInstance()->getWritablePath() + run_configure).c_str());
	XMLElement *rootEle = pDoc->RootElement();
	const XMLAttribute* identityAttribute = rootEle->FirstAttribute();
	const XMLAttribute* languageAttribute = identityAttribute->Next();
	const XMLAttribute* levelAttribute = languageAttribute->Next();
	mode = identityAttribute->Value();
	level = levelAttribute->Value();
}
void GameOver::readScore(){
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "score.xml";
	tinyxml2::XMLDocument *xmlDoc = new tinyxml2::XMLDocument();
	xmlDoc->LoadFile(filePath.c_str());
	tinyxml2::XMLElement* rootElement = xmlDoc->RootElement();
	XMLElement* element = rootElement->FirstChildElement();
	while (element){
		const XMLAttribute *groupIDAttribute = element->FirstAttribute();
		const XMLAttribute *groupLimitTimeAttribute = groupIDAttribute->Next();
		const XMLAttribute *groupFinishTimeAttribute = groupLimitTimeAttribute->Next();
		const XMLAttribute *groupTimesAttribute = groupFinishTimeAttribute->Next();
		const XMLAttribute *groupErrorTimesAttribute = groupTimesAttribute->Next();
		const XMLAttribute *groupMissTimesAttribute = groupErrorTimesAttribute->Next();
		const XMLAttribute *groupScoreAttribute = groupMissTimesAttribute->Next();
		sumTime += groupLimitTimeAttribute->FloatValue();
		finishTime += groupFinishTimeAttribute->FloatValue();
		sumTimes += groupTimesAttribute->IntValue();
		errorTimes += groupErrorTimesAttribute->IntValue();
		missTimes += groupMissTimesAttribute->IntValue();
		baseScore += groupScoreAttribute->IntValue();
		element = element->NextSiblingElement();
	}
}
float GameOver::readHighest(std::string mode, std::string level){
	float score = 0.000f;
	sqlite3_open(
		(FileUtils::getInstance()->getWritablePath() + morseData).c_str(),
		&sqlite);
	std::string select_from_highest = std::string("SELECT * FROM [highest] where mode = '") + mode.c_str() + "' AND level= '" + level.c_str() + "';";
	sqlite3_stmt * stmt;
	sqlite3_prepare_v2(sqlite, select_from_highest.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const char *  s = (const char *)sqlite3_column_text(stmt, 2);
		score = atof(s);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(sqlite);
	return score;
}
void GameOver::storeHighest(std::string mode, std::string level, float highest){
//	char* errMsg;
	sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
	sqlite3_exec(sqlite, "BEGIN;", 0, 0, NULL);
/**	int error = **/sqlite3_exec(sqlite, ("UPDATE [highest] SET score = '" + __String::createWithFormat("%.3f", highest)->_string + "' WHERE mode = '" + mode.c_str() + "' AND level = '" + level.c_str() + "';").c_str(), 0, 0, NULL);
/**	if (error != SQLITE_OK)
	{
		log("msg=%s,errMsg=%s", "执行SQL出错.", errMsg);
	}
	else
	{
		log("msg=%s", "SQL成功执行.");
	}**/
	sqlite3_exec(sqlite, "COMMIT;", 0, 0, NULL);
	sqlite3_close(sqlite);
}
void GameOver::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_SYSREQ)  //返回
	{
		auto scene = MenuScene::createScene();
		auto transitionScene = TransitionFade::create(switchGameTime, scene);
		auto director = Director::getInstance();
		director->replaceScene(transitionScene);
	}
}

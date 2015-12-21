#include "DictLayer.h"
#include "tinyxml2/tinyxml2.h"
#include "GameOver.h"
std::string DictLayer::language;
std::string DictLayer::level;
using namespace tinyxml2;

DictLayer::DictLayer() :
dict_signal(0),
atomicHeight(0.0f),
atomicWidth(0.0f),
scale(0.0f),
groupID(0),
groupLimitTime(0.0f),
groupFinishTime(0.0f),
groupScore(0),
groupTimes(0),
groupErrorTimes(0),
groupMissTimes(0),
timeSeed(0.0f)
//, 
//sumMiss(0), 
//sumError(0), 
//sumTimes(0)
{
}
DictLayer::~DictLayer() {
}
bool DictLayer::init() {
	if (!Layer::init())
	{
		return false;
	}
	preloadEffect();
	visibleSize = Director::getInstance()->getVisibleSize();
	initLanguage(level.c_str());
	//_sumTimeLabel = Label::createWithSystemFont("0.0\"", font_consolas, 56);
	//_sumTimeLabel->setAnchorPoint(Vec2(0.5, 0.5));
	//_sumTimeLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 5));
	//_sumTimeLabel->setColor(Color3B(60, 60, 60));
	//_sumTimeLabel->setOpacity(100);
	//this->addChild(_sumTimeLabel);
	initDict(language.c_str(), level.c_str());
	createDictLabel(language.c_str(), level.c_str());
	createTimeBar();
	createMorseCodeSprites();
	createButton();
	this->scheduleUpdate();
	//NotificationCenter::getInstance()->addObserver(this,
	//	callfuncO_selector(DictLayer::execSignal), "signal", NULL);
	//auto touchListener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchBegan = CC_CALLBACK_2(DictLayer::onTouchBegan, this);
	//touchListener->onTouchEnded = CC_CALLBACK_2(DictLayer::onTouchEnded, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	return true;
}
//void DictLayer::onToucheBegan(Touch* touche, Event *unused_event){
//}
//void DictLayer::onToucheEnded(Touch* touche, Event *unused_event){
//}
void DictLayer::initLanguage(const char* level){
	//根据语言环境加载不同语言,目前只有中英文
	LanguageType nType = Application::getInstance()->getCurrentLanguage();
	if (nType == LanguageType::CHINESE) {
		strings = __Dictionary::createWithContentsOfFile(il18n_chinese);
		font_type = font_droid_sans_fallback;
		if (strcmp(level, "5") == 0
			|| strcmp(level, "6") == 0) {
			font_label_size = font_5_6_label_size;
			scale = 16.0f;
		}
		else if (strcmp(level, "4") == 0){
			font_label_size = font_4_label_size;
			scale = 12.0f;
		}
		else{
			font_label_size = font_1_2_3_label_size;
			scale = 10.0f;
		}
	}
	else {
		strings = __Dictionary::createWithContentsOfFile(il18n_english);
		font_type = font_marker_felt;
		if (strcmp(level, "5") == 0
			|| strcmp(level, "6") == 0) {
			font_label_size = font_5_6_label_size;
			font_code_size = font_5_6_code_size;
			scale = 16.0f;
		}
		else if (strcmp(level, "4") == 0){
			font_label_size = font_4_label_size;
			font_code_size = font_4_code_size;
			scale = 12.0f;
		}
		else{
			font_label_size = font_1_2_3_label_size;
			font_code_size = font_1_2_3_code_size;
			scale = 10.0f;
		}
	}
}
void DictLayer::initDict(const char* language, const char* level) {
	//char* errMsg;
	sqlite3_open(
		(FileUtils::getInstance()->getWritablePath() + morseData).c_str(),
		&sqlite);
	size_t group = 0;
	if (strcmp(level, "6") == 0){
		group = 2;
		timeSeed = 0.24;
	}
	else if (strcmp(level, "5") == 0){
		group = 2;
		timeSeed = 0.26;
	}
	else if (strcmp(level, "4") == 0){
		group = 3;
		timeSeed = 0.28;
	}
	else if (strcmp(level, "3") == 0){
		group = 5;
		timeSeed = 0.3;
	}
	else if (strcmp(level, "2") == 0){
		group = 10;
		timeSeed = 0.4;
	}
	else if (strcmp(level, "1") == 0){
		group = 15;
		timeSeed = 0.4;
	}
	std::string select_from_language_level = std::string("SELECT * FROM [dict] where level = ") + level + " ORDER BY RANDOM() limit " + __String::createWithFormat("%d", group)->_string + ";";
	sqlite3_stmt * stmt;
	sqlite3_prepare_v2(sqlite, select_from_language_level.c_str(), -1, &stmt, NULL);
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const char *  row = (const char *)sqlite3_column_text(stmt, 0);
		const char * level = (const char *)sqlite3_column_text(stmt, 1);
		const char * value = (const char *)sqlite3_column_text(stmt, 2);
		const char * morseCode = (const char *)sqlite3_column_text(stmt, 3);
		if (strcmp(language, "en") == 0){
			const char * translation = (const char *)sqlite3_column_text(stmt, 4);
			dicts.push_back({ value, morseCode, translation });
		}
		if (strcmp(language, "zh_CN") == 0){
			const char * translation = (const char *)sqlite3_column_text(stmt, 5);
			dicts.push_back({ value, morseCode, translation });
		}

		////插入数据用的
		//sqlite3_exec(sqlite, "BEGIN;", 0, 0, &errMsg);
		//log("errMsg=%s", errMsg);
		//sqlite3_exec(sqlite, ("update [dict] set code= '" + convertStr(value) + "' where ID=" + row).c_str(), 0, 0, &errMsg);
		//log("errMsg=%s", errMsg);
		//sqlite3_exec(sqlite, "COMMIT;", 0, 0, &errMsg);
		//log("errMsg=%s", errMsg);


	}
	sqlite3_finalize(stmt);
	sqlite3_close(sqlite);
	Sprite* di = Sprite::create("di.png");
	atomicHeight = di->getContentSize().height / scale;
	atomicWidth = di->getContentSize().width / scale;
}
void DictLayer::createDictLabel(const char* language, const char* level){
	if (strcmp(language, "en") == 0)
		dictValueLabel = Label::createWithSystemFont(
		((dict_struct)(dicts.at(dict_signal))).value, font_consolas, font_label_size);
	else if (strcmp(language, "zh_CN") == 0)
		dictValueLabel = Label::createWithTTF(((dict_struct)(dicts.at(dict_signal))).value,
		font_droid_sans_fallback, font_label_size);
	if (strlen(((dict_struct)(dicts.at(dict_signal))).translation.c_str()) > 0)
		dictValueLabel->setString(((dict_struct)(dicts.at(dict_signal))).value + "  " + ((dict_struct)(dicts.at(dict_signal))).translation);
	dictValueLabel->setPosition(
		Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));
	dictValueLabel->setColor(Color3B(0,0,0));
	dictValueLabel->setWidth(visibleSize.width * 4 / 5);
	this->addChild(dictValueLabel);
}
void DictLayer::createTimeBar(){
	LoadingBar* timeBar = LoadingBar::create("time.png");
	timeBar->setTag(timeBar_tag);
	timeBar->setColor(Color3B(0, 0, 0));
	timeBar->setDirection(LoadingBar::Direction::RIGHT);
	timeBar->setPosition(Vec2(visibleSize.width / 2, dictValueLabel->getPositionY() - dictValueLabel->getContentSize().height*3/5 - timeBar->getContentSize().height));
	timeBar->setPercent(100.0f);
	this->addChild(timeBar);
}
void DictLayer::createMorseCodeSprites(){
	signalPosition.clear();
	morseCodeSprites.clear();
	LoadingBar* timeBar = static_cast<LoadingBar*>(this->getChildByTag(timeBar_tag));
	for (int i = strlen(((dict_struct)(dicts.at(dict_signal))).morseCode.c_str())
		- 1; i >= 0; i--){
		if (((dict_struct)(dicts.at(dict_signal))).morseCode[i] == '.')
			signalPosition.push_back({ i, true });
		if (((dict_struct)(dicts.at(dict_signal))).morseCode[i] == '-')
			signalPosition.push_back({ i, false });
	}
	initGroupTime_Score();
	//sumTimes += signalPosition.size();
	for (int i = 0; (((dict_struct)(dicts.at(dict_signal))).morseCode)[i]; i++){
		if ((((dict_struct)(dicts.at(dict_signal))).morseCode)[i] == '.'){
			Sprite* di = Sprite::create("di.png");
			di->setScale(1 / scale);
			di->setAnchorPoint(Vec2(0.5, 0.5));
			di->setColor(Color3B(0,0,0));
			morseCodeSprites.pushBack(di);
		}
		if ((((dict_struct)(dicts.at(dict_signal))).morseCode)[i] == '/'){
			Sprite* halt = Sprite::create("halt.png");
			halt->setScale(1 / scale);
			halt->setAnchorPoint(Vec2(0.5, 0.5));
			halt->setColor(Color3B(0, 0, 0));
			morseCodeSprites.pushBack(halt);
		}
		if ((((dict_struct)(dicts.at(dict_signal))).morseCode)[i] == ' '){
			Sprite* space = Sprite::create("space.png");
			space->setScale(1 / scale);
			space->setAnchorPoint(Vec2(0.5, 0.5));
			space->setColor(Color3B(0, 0, 0));
			morseCodeSprites.pushBack(space);
		}
		if ((((dict_struct)(dicts.at(dict_signal))).morseCode)[i] == '-'){
			Sprite* da = Sprite::create("da.png");
			da->setScale(1 / scale);
			da->setAnchorPoint(Vec2(0.5, 0.5));
			da->setColor(Color3B(0, 0, 0));
			morseCodeSprites.pushBack(da);
		}
	}
	int rowNum = 0;//每行元素数
	//float sumWidth=0.0f;//元素总长
	float rowSumWidth = 0.0f;//行元素总长
	std::vector<int> vvs;//行数数组
	for (int i = 0; i < morseCodeSprites.size(); i++){
		rowNum++;//默认元素不为空，+1
		//sumWidth += morseCodeSprites.at(i)->getContentSize().width / scale + atomicWidth;
		rowSumWidth += morseCodeSprites.at(i)->getContentSize().width / scale + atomicWidth;
		if (rowSumWidth >= visibleSize.width - 6 * atomicWidth){//当行元素总长大于屏宽时
			vvs.push_back(rowNum);//填充数组第一个元素，值为元素数
			rowSumWidth = 0;//行元素总长置0，为重新计算下一行
		}
	}
	if (rowSumWidth < visibleSize.width - 6 * atomicWidth&&rowSumWidth != 0){//rowSumWidth与rowNum不为零则为不满一行，需要将其填充到数组
		vvs.push_back(rowNum);
	}

	int flg = 0;
	//for (auto jj : vvs){//遍历数组
	//	tt += jj;
	for (int kk = 0; kk < morseCodeSprites.size(); kk++){
		if (kk == 0)//如果元素为起始元素
			morseCodeSprites.at(kk)->setPosition(Vec2(6 * atomicWidth, timeBar->getPositionY() - timeBar->getContentSize().height - atomicHeight * 2));
		else if (kk == vvs.at(flg)){//如果元素为换行起始元素
			morseCodeSprites.at(kk)->setPosition(Vec2(6 * atomicWidth, timeBar->getPositionY() - timeBar->getContentSize().height - atomicHeight * 2 - (flg + 1)*atomicHeight * 4));
			flg++;
		}
		else
			morseCodeSprites.at(kk)->setPosition(Vec2(morseCodeSprites.at(kk - 1)->getPositionX() + (morseCodeSprites.at(kk - 1)->getContentSize().width / scale) / 2 + (morseCodeSprites.at(kk)->getContentSize().width / scale) / 2 + atomicWidth / 2, morseCodeSprites.at(kk - 1)->getPositionY()));
		this->addChild(morseCodeSprites.at(kk));
	}
	//}
}
void DictLayer::initGroupTime_Score(){
	groupID++;
	groupTimes = signalPosition.size();
	groupScore = 0;
	groupErrorTimes = 0;
	groupMissTimes = 0;
	groupLimitTime = float(strlen(((dict_struct)(dicts.at(dict_signal))).morseCode.c_str()))*timeSeed;
	groupFinishTime = 0.0f;
}
void DictLayer::execSignal(Ref * pObjc, Widget::TouchEventType type) {
	switch (type){
	case Widget::TouchEventType::BEGAN:
		if (signalPosition.size() > 0){
			if (((Button*)pObjc)->getTag() == 1)
			{
				if (signalPosition.back().isDit){
					auto ditScoreLabel = Label::createWithSystemFont("+1", font_consolas, font_label_size);
					ditScoreLabel->setPosition(ditNormalButton->getPosition());
					ditScoreLabel->setColor(Color3B(128, 128, 128));
					this->addChild(ditScoreLabel, 50);
					ditScoreLabel->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(ditNormalButton->getPosition().x, ditNormalButton->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::deleteObject, this)), NULL));
					groupScore += 1;
					SimpleAudioEngine::getInstance()->playEffect(ditEffect);
					morseCodeSprites.at(signalPosition.back().position)->runAction(Sequence::create(ScaleTo::create(0.1f, (1 / scale)*1.5), Spawn::create(ScaleTo::create(0.1f, 1 / scale), TintBy::create(0, 0, 113, 255), NULL), NULL));
					signalPosition.pop_back();
					if (signalPosition.size() <= 0){
						complete();
					}
					break;
				}
				else{
					auto ditScoreLabel = Label::createWithSystemFont("-1", font_consolas, font_label_size);
					ditScoreLabel->setPosition(ditNormalButton->getPosition());
					ditScoreLabel->setColor(Color3B(128, 128, 128));
					this->addChild(ditScoreLabel, 50);
					ditScoreLabel->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(ditNormalButton->getPosition().x, ditNormalButton->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::deleteObject, this)), NULL));
					groupScore -= 1;
					//sumError += 1;
					groupErrorTimes += 1;
					SimpleAudioEngine::getInstance()->playEffect(errorEffect);
					morseCodeSprites.at(signalPosition.back().position)->runAction(Sequence::create(ScaleTo::create(0.1f, (1 / scale)*1.5), Spawn::create(ScaleTo::create(0.1f, 1 / scale), TintBy::create(0, 255, 0, 0), NULL), NULL));
					signalPosition.pop_back();
					if (signalPosition.size() <= 0){
						complete();
					}
					break;
				}
			}
			if (((Button*)pObjc)->getTag() == 2){
				if (!signalPosition.back().isDit){
					auto dahScoreLabel = Label::createWithSystemFont("+1", font_consolas, font_label_size);
					dahScoreLabel->setPosition(dahNormalButton->getPosition());
					dahScoreLabel->setColor(Color3B(128, 128, 128));
					this->addChild(dahScoreLabel, 50);
					dahScoreLabel->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(dahNormalButton->getPosition().x, dahNormalButton->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::deleteObject, this)), NULL));
					groupScore += 1;
					SimpleAudioEngine::getInstance()->playEffect(dahEffect);
					morseCodeSprites.at(signalPosition.back().position)->runAction(Sequence::create(ScaleTo::create(0.1f, (1 / scale)*1.5), Spawn::create(ScaleTo::create(0.1f, 1 / scale), TintBy::create(0, 0, 113, 255), NULL), NULL));
					signalPosition.pop_back();
					if (signalPosition.size() <= 0){
						complete();
					}

					break;
				}
				else{
					auto dahScoreLabel = Label::createWithSystemFont("-1", font_consolas, font_label_size);
					dahScoreLabel->setPosition(dahNormalButton->getPosition());
					dahScoreLabel->setColor(Color3B(128, 128, 128));
					this->addChild(dahScoreLabel, 50);
					dahScoreLabel->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(dahNormalButton->getPosition().x, dahNormalButton->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::deleteObject, this)), NULL));
					groupScore -= 1;
					//sumError += 1;
					groupErrorTimes += 1;
					SimpleAudioEngine::getInstance()->playEffect(errorEffect);
					morseCodeSprites.at(signalPosition.back().position)->runAction(Sequence::create(ScaleTo::create(0.1f, (1 / scale)*1.5), Spawn::create(ScaleTo::create(0.1f, 1 / scale), TintBy::create(0, 255, 0, 0), NULL), NULL));
					signalPosition.pop_back();
					if (signalPosition.size() <= 0){
						complete();
					}
					break;
				}
			}
		}
	}
}
void DictLayer::complete(){
	this->unscheduleUpdate();
	ditNormalButton->setVisible(false);
	dahNormalButton->setVisible(false);
	float incorrectness = ((float)groupErrorTimes + (float)groupMissTimes) / (float)groupTimes;
	//log("error=%d,miss=%d,incorrectness=%f", error, miss, incorrectness);
	if (groupErrorTimes == 0 && groupMissTimes == 0){
		auto perfect = Label::createWithSystemFont("Perfect!", font_consolas, font_label_size);
		perfect->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		perfect->setColor(Color3B(128, 128, 128));
		this->addChild(perfect, 50);
		perfect->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.2), FadeIn::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::completeCallback, this)), NULL));
	}
	else if (incorrectness >= 0.5){
		auto great = Label::createWithSystemFont("Fingers Cramping?", font_consolas, font_label_size);
		great->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		great->setColor(Color3B(128, 128, 128));
		this->addChild(great, 50);
		great->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.2), FadeIn::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::completeCallback, this)), NULL));

	}
	else if (0 < incorrectness &&incorrectness <= 0.1){
		auto great = Label::createWithSystemFont("Great!", font_consolas, font_label_size);
		great->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		great->setColor(Color3B(128, 128, 128));
		this->addChild(great, 50);
		great->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.2), FadeIn::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::completeCallback, this)), NULL));
	}
	else if (incorrectness > 0.1 && incorrectness < 0.5){
		auto good = Label::createWithSystemFont("Good!", font_consolas, font_label_size);
		good->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		good->setColor(Color3B(128, 128, 128));
		this->addChild(good, 50);
		good->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.2), FadeIn::create(score_one_fadeout_duration)), CCCallFuncN::create(CC_CALLBACK_1(DictLayer::completeCallback, this)), NULL));
	}
}
void DictLayer::completeCallback(Node* sender){
	group_scores.push_back({ groupID, groupLimitTime, groupFinishTime, groupTimes, groupErrorTimes, groupMissTimes, groupScore });
	sender->removeFromParent();
	this->scheduleOnce(schedule_selector(DictLayer::createDict), 0.1f);
}
void DictLayer::deleteObject(Node* sender){
	sender->removeFromParent();
}
void DictLayer::update(float dt){
	groupFinishTime += dt;
	LoadingBar* timeBar = static_cast<LoadingBar*>(this->getChildByTag(timeBar_tag));
	if (groupFinishTime <= groupLimitTime)
		timeBar->setPercent(((groupLimitTime - groupFinishTime) / groupLimitTime)*100.0f);
	//_sumTimeLabel->setString(StringUtils::format("%.1f\"", _time - _sumTime));
	if (((groupLimitTime - groupFinishTime) / groupLimitTime)*100.0f < 30.0f)
		timeBar->setColor(Color3B(255, 0, 0));
	if (groupFinishTime >= groupLimitTime){
		timeBar->setPercent(0.0f);
		groupMissTimes = signalPosition.size();
		//sumMiss += signalPosition.size();
		complete();
		//_sumTimeLabel->setString("0.0\"");
	}
}
void DictLayer::createDict(float dt) {
	if (dict_signal >= dicts.size() - 1)
		gameOver();
	else{
		ditNormalButton->setVisible(true);
		dahNormalButton->setVisible(true);
		//log("基础分数     = %d\n该组字符次数 = %d\n该组错误次数 = %d\n该组漏掉次数 = %d\n总漏掉次数    = %d\n总错误次数    = %d\n总字符次数    = %d\n", score, times, error,miss, sumMiss, sumError, sumTimes);
		//for (auto s : limit_finish_time){
		//	log("limit=%f",s.limitTime);
		//	log("finish=%f", s.finishTime);
		//}
		LoadingBar* timeBar = static_cast<LoadingBar*>(this->getChildByTag(timeBar_tag));
		timeBar->setColor(Color3B(0, 0, 0));
		dict_signal++;
		dictValueLabel->setString(((dict_struct)(dicts.at(dict_signal))).value + "  " + ((dict_struct)(dicts.at(dict_signal))).translation);
		for (auto sprite : morseCodeSprites){
			sprite->removeFromParent();
		}
		createMorseCodeSprites();
		this->scheduleUpdate();
	}
}
void DictLayer::createButton(){
	ditNormalButton = Button::create("dit.png");
	ditNormalButton->setTag(1);
	dahNormalButton = Button::create("dah.png");
	dahNormalButton->setTag(2);
	ditNormalButton->addTouchEventListener(CC_CALLBACK_2(DictLayer::execSignal, this));
	dahNormalButton->addTouchEventListener(CC_CALLBACK_2(DictLayer::execSignal, this));
	ditNormalButton->setPosition(Vec2(visibleSize.width * 1 / 4, ditNormalButton->getContentSize().height));
	dahNormalButton->setPosition(Vec2(visibleSize.width * 3 / 4, dahNormalButton->getContentSize().height));
	this->addChild(ditNormalButton, 20);
	this->addChild(dahNormalButton, 20);

	//ditScore_seq = Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(ditScoreLabel->getPosition().x, ditScoreLabel->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), NULL);
	//dahScore_seq = Sequence::create(Spawn::createWithTwoActions(MoveTo::create(score_one_move_duration, Vec2(dahScoreLabel->getPosition().x, dahScoreLabel->getPosition().y + 50)), FadeOut::create(score_one_fadeout_duration)), NULL);
}
void DictLayer::gameOver(){
	this->unscheduleUpdate();
	storeScore();

	auto scene = GameOver::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);
}
void DictLayer::storeScore(){
	std::string filePath = FileUtils::getInstance()->getWritablePath() + "score.xml";
	tinyxml2::XMLDocument *pDoc = new tinyxml2::XMLDocument();
	if (NULL == pDoc) {
		return;
	}
	XMLElement *rootElement = pDoc->NewElement("root");
	pDoc->LinkEndChild(rootElement);
	for (auto group : group_scores){
		XMLElement *groupElement = pDoc->NewElement("group");
		groupElement->SetAttribute("groupID", (int)group.groupID);
		groupElement->SetAttribute("groupLimitTime", group.groupLimitTime);
		groupElement->SetAttribute("groupFinishTime", group.groupFinishTime);
		groupElement->SetAttribute("groupTimes", (int)group.groupTimes);
		groupElement->SetAttribute("groupErrorTimes", (int)group.groupErrorTimes);
		groupElement->SetAttribute("groupMissTimes", (int)group.groupMissTimes);
		groupElement->SetAttribute("groupScore", group.groupScore);
		rootElement->LinkEndChild(groupElement);
	}
	pDoc->SaveFile(filePath.c_str());
	delete pDoc;
}
std::string DictLayer::convertStr(const char* str) {
	std::string out = "";
	for (int i = 0; str[i]; i++) {
		switch (toupper(str[i])) {
		case 'A':
			out += ".- ";
			break;
		case 'B':
			out += "-... ";
			break;
		case 'C':
			out += "-.-. ";
			break;
		case 'D':
			out += "-.. ";
			break;
		case 'E':
			out += ". ";
			break;
		case 'F':
			out += "..-. ";
			break;
		case 'G':
			out += "--. ";
			break;
		case 'H':
			out += ".... ";
			break;
		case 'I':
			out += ".. ";
			break;
		case 'J':
			out += ".--- ";
			break;
		case 'K':
			out += "-.- ";
			break;
		case 'L':
			out += ".-.. ";
			break;
		case 'M':
			out += "-- ";
			break;
		case 'N':
			out += "-. ";
			break;
		case 'O':
			out += "--- ";
			break;
		case 'P':
			out += ".--. ";
			break;
		case 'Q':
			out += "--.- ";
			break;
		case 'R':
			out += ".-. ";
			break;
		case 'S':
			out += "... ";
			break;
		case 'T':
			out += "- ";
			break;
		case 'U':
			out += "..- ";
			break;
		case 'V':
			out += "...- ";
			break;
		case 'W':
			out += ".-- ";
			break;
		case 'X':
			out += "-..- ";
			break;
		case 'Y':
			out += "-.-- ";
			break;
		case 'Z':
			out += "--.. ";
			break;
		case '1':
			out += ".---- ";
			break;
		case '2':
			out += "..--- ";
			break;
		case '3':
			out += "...-- ";
			break;
		case '4':
			out += "....- ";
			break;
		case '5':
			out += "..... ";
			break;
		case '6':
			out += "-.... ";
			break;
		case '7':
			out += "--... ";
			break;
		case '8':
			out += "---.. ";
			break;
		case '9':
			out += "----. ";
			break;
		case '10':
			out += "----- ";
			break;
		case ' ':
			out += "/";
			break;
		case '$':
			out += "...-. ";
			break;
		case '!':
			out += "-.-.-- ";
			break;
		case ';':
			out += "-..-. ";
			break;
		case ',':
			out += "--..-- ";
			break;
		case '_':
			out += "..--.- ";
			break;
		case '@':
			out += "--.-. ";
			break;
		case '#':
			out += "..-- ";
			break;
		case '*':
			out += "---- ";
			break;
		case '(':
			out += "-.--. ";
			break;
		case ')':
			out += "-.--.- ";
			break;
		case '?':
			out += "..--.. ";
			break;
		case ':':
			out += "---... ";
			break;
		case '&':
			out += ". ... ";
			break;
		case '=':
			out += "-...- ";
			break;
		case '\x27':
			out += ".----. ";
			break;
		case '.':
			out += ".-.-.- ";
			break;
		case '-':
			out += "-....- ";
			break;
		default:
			out;
			break;
		}
	}
	out = out.substr(out.find_first_not_of(' '), out.find_last_not_of(' ') + 1);
	std::string searchString(" /");
	std::string replaceString("/");
	std::string::size_type pos = 0;
	while ((pos = out.find(searchString, pos)) != std::string::npos) {
		out.replace(pos, searchString.size(), replaceString);
		pos++;
	}
	return out;
}
void DictLayer::preloadEffect(){
	SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);
	//缓存音效
	SimpleAudioEngine::getInstance()->preloadEffect(ditEffect);
	SimpleAudioEngine::getInstance()->preloadEffect(dahEffect);
	SimpleAudioEngine::getInstance()->preloadEffect(errorEffect);
}

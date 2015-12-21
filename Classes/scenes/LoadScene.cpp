#include "LoadScene.h"
#include "MenuScene.h"
#include "resources.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
//using namespace tinyxml2;
using namespace CocosDenshion;
LoadScene::LoadScene()
{
}
LoadScene::~LoadScene()
{
}
Scene* LoadScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadScene::create();
	scene->addChild(layer);
	return scene;
}
bool LoadScene::init()
{
	if (!LayerGradient::initWithColor(Color4B(25, 120, 187, 200), Color4B(133, 81, 179, 255), Point(1.0f, -1.0f)))
	{
		return false;
	}
	//initMorseCode();
	//initDict();
	copyData();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto label = Label::createWithTTF("Toparchy", font_marker_felt, font_loadscene_size);
	label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	label->enableShadow(font_loadscene_color);
	label->setSkewY(-10);
	this->addChild(label, 0, loadTag);
	menuScene();
	return true;
}
void LoadScene::menuScene(){
	auto pSprite = (Sprite*)getChildByTag(loadTag);
	pSprite->runAction(Sequence::create(DelayTime::create(switchTime), CallFunc::create(
		[&](){
		auto scene = MenuScene::createScene();
		auto director = Director::getInstance();
		//auto transitionScene = TransitionJumpZoom::create(transitionTime, scene);
		director->replaceScene(scene);
	}), NULL));
}

void LoadScene::initMorseCode(){
	if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + morseData)){
		int open = sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
		if (open != SQLITE_OK){
			log("msg=%s", "创建数据库出错.");
		}
		else{
			log("msg=%s", "创建数据库成功.");
		}
		char* errMsg;
		std::string create_dict_language = "create table morse_code (ID integer primary key autoincrement,language char(10), name char(5),value char(8));";
		int exec_create_dict_language = sqlite3_exec(sqlite, create_dict_language.c_str(), 0, 0, &errMsg);
		if (exec_create_dict_language != SQLITE_OK)
		{
			log("msg=%s,errMsg=%s", "执行创建table的SQL 出错.", errMsg);
		}
		else
		{
			log("msg=%s", "创建table的SQL成功执行.");
		}
		int exec_insert_morse_code = sqlite3_exec(sqlite, "BEGIN;", 0, 0, &errMsg);
		typedef struct{ std::string name; std::string value; } custom_s;
		std::vector<custom_s> morse_codes = {
				{ "A", ".-" }, { "B", "-..." }, { "C", "-.-." },
				{ "D", "-.." }, { "E", "." }, { "F", "..-." },
				{ "G", "--." }, { "H", "...." }, { "I", ".." },
				{ "J", ".---" }, { "K", "-.-" }, { "L", ".-.." },
				{ "M", "--" }, { "N", "-." }, { "O", "---" },
				{ "P", ".--." }, { "Q", "--.-" }, { "R", ".-." },
				{ "S", "..." }, { "T", "-" }, { "U", "..-" },
				{ "V", "...-" }, { "W", ".--" }, { "X", "-..-" },
				{ "Y", "-.--" }, { "Z", "--.." }, { "1", ".----" },
				{ "2", "..---" }, { "3", "...--" }, { "4", "....-" },
				{ "5", "....." }, { "6", "-...." }, { "7", "--..." },
				{ "8", "---.." }, { "9", "----." }, { "10", "-----" },
				{ "AR", ".-.-." }, { "AS", ".-..." }, { "SK", "...-.-" },
				{ "BT", "-...-" }, { "ERROR", "........" }
		};
		std::vector<custom_s>::iterator it;
		for (it = morse_codes.begin(); it != morse_codes.end(); ++it){
			int exec_insert_morse_code = sqlite3_exec(sqlite, ("insert into morse_code(language,name,value) values ('en','" + (*it).name + "','" + (*it).value + "')").c_str(), 0, 0, &errMsg);
		}
		exec_insert_morse_code = sqlite3_exec(sqlite, "COMMIT;", 0, 0, &errMsg);
		sqlite3_close(sqlite);
	}
}
void LoadScene::initDict(){
	int create = sqlite3_open((FileUtils::getInstance()->getWritablePath() + morseData).c_str(), &sqlite);
	if (create != SQLITE_OK){
		log("msg=%s", "创建数据库出错.");
	}
	else{
		log("msg=%s", "创建数据库成功.");
	}
	char* errMsg;
	std::string create_dict_language = "create table dict_language (ID integer primary key autoincrement,language char(10));";
	int exec_create_dict_language = sqlite3_exec(sqlite, create_dict_language.c_str(), 0, 0, &errMsg);
	if (exec_create_dict_language != SQLITE_OK)
	{
		log("msg=%s,errMsg=%s", "执行创建table的SQL 出错.", errMsg);
	}
	else
	{
		log("msg=%s", "创建table的SQL成功执行.");
	}
	int exec_insert_dict_language = sqlite3_exec(sqlite, "BEGIN;", 0, 0, &errMsg);
	for (std::string language : dictLanguages){
		int exec_insert_dict_language = sqlite3_exec(sqlite, ("insert into dict_language(language) values ('" + language + "')").c_str(), 0, 0, &errMsg);
	}
	exec_insert_dict_language = sqlite3_exec(sqlite, "COMMIT;", 0, 0, &errMsg);
	sqlite3_close(sqlite);
}
void LoadScene::copyData(){
	if (!FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + morseData)){
		std::string strPath = FileUtils::getInstance()->fullPathForFilename(morseData);
		Data data = FileUtils::getInstance()->getDataFromFile(strPath);
		std::string destPath = FileUtils::getInstance()->getWritablePath() + morseData;
		FILE *pFp = fopen(destPath.c_str(), "wb+");
		fwrite(data.getBytes(), data.getSize(), 1, pFp);
		fclose(pFp);
		data.clear();
	}
}

#ifndef _GAMEOVER_SCENE_H_
#define _GAMEOVER_SCENE_H_

#include "cocos2d.h"
#include "sqlite3.h"
USING_NS_CC;

class GameOver : public cocos2d::LayerColor
{
	sqlite3 * sqlite;//sqlite3
	std::string mode;
	std::string level;
	ssize_t baseScore;//基础分数
	float sumTime;//总时间
	float finishTime;//完成时间
	size_t sumTimes;//总字符
	size_t errorTimes;//总错误次数
	size_t missTimes;//总漏掉次数
	void readScore();
	void readConfig();//读取运行配置
	float readHighest(std::string mode, std::string level);//读取高分
	void storeHighest(std::string mode, std::string level, float highest);//存储高分
public:
	GameOver();
	~GameOver();
	static Scene* createScene();
	virtual bool init();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	CREATE_FUNC(GameOver);
	Size visibleSize;
	__Dictionary *strings;
	std::string font_type;
	int font_size;

};

#endif

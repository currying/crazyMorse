#ifndef _MORSECODE_SCENE_H_
#define _MORSECODE_SCENE_H_

#include "cocos2d.h"
#include "sqlite3.h"
USING_NS_CC;

class MorseCode : public LayerColor
{
public:
	MorseCode();
	~MorseCode();
	static Scene* createScene(/*const char* language*/);
	static MorseCode* create(/*const char* language*/);
	bool init(/*const char* language*/);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Vec2 origin;
	//virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	//virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	void readMorseCode(/*const char* language*/);
	sqlite3 * sqlite;
};

#endif

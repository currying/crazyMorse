#ifndef _DICT_SCENE_H_
#define _DICT_SCENE_H_

#include "cocos2d.h"
#include "sqlite3.h"
#include "ScreenShot.h"
USING_NS_CC;

class Dict : public LayerColor
{
public:
	Dict();
	~Dict();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Dict);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Vec2 origin;
	ScreenShot* screenShot;
	void readDictIndex();
	//void createDictIndex();
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	Sprite* createButton(Size size, Color3B color, Label* blockLabel);
	sqlite3 * sqlite;
};

#endif

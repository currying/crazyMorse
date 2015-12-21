#ifndef _MENU_SCENE_H_
#define _MENU_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public LayerColor
{
	Size visibleSize;
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	std::string buttonNormal;
	std::string buttonSelect;
	Sprite* createBlock(std::string buttonName, Point anchor, Label* label);
public:
	MenuScene();
	~MenuScene();
	static Scene* createScene();
	virtual bool init();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	CREATE_FUNC(MenuScene);
	void menuMusicToggleCallback(Ref* sender);
	bool sound_sign;
};

#endif

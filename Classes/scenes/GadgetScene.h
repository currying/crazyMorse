#ifndef _GADGET_SCENE_H_
#define _GADGET_SCENE_H_

#include "cocos2d.h"
#include "resources.h"

USING_NS_CC;
class GadgetScene : public LayerColor
{
	Size visibleSize;
	Point origin;
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	std::string buttonNormal;
	std::string buttonSelect;
	void createMenu();
	Sprite* createBlock(std::string buttonName, Point anchor, Label* label);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GadgetScene);
};
#endif 

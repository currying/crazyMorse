#ifndef _LEISUREMODE_SCENE_H_
#define _LEISUREMODE_SCENE_H_

#include "cocos2d.h"
#include "DictLayer.h"
USING_NS_CC;
class LeisureMode : public LayerColor
{
public:
	static cocos2d::Scene* createScene(const char* language, const char* level);
	bool init(const char* language, const char* level);
	DictLayer* dictLayer;
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Point origin;
};
#endif 

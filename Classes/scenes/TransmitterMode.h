#ifndef _TRANSMITTERMODE_SCENE_H_
#define _TRANSMITTERMODE_SCENE_H_

#include "cocos2d.h"
#include "DictLayer2.h"
USING_NS_CC;
class TransmitterMode : public LayerColor
{
public:
	static cocos2d::Scene* createScene(const char* language, const char* level);
	bool init(const char* language, const char* level);
	DictLayer2* dictLayer;
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Point origin;
};
#endif 
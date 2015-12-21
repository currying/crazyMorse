#ifndef _CODEBOOK_SCENE_H_
#define _CODEBOOK_SCENE_H_

#include "cocos2d.h"
#include "ScreenShot.h"
USING_NS_CC;

class CodeBook : public LayerColor
{
public:
	CodeBook();
	~CodeBook();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(CodeBook);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Vec2 origin;
	ScreenShot* screenShot;
	//virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	//virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	void readCodeBook();
	void createCodeBook();
	Sprite* createButton(Size size, Color3B color, Label* blockLabel);
};

#endif

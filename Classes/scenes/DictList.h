#ifndef _DICTLIST_SCENE_H_
#define _DICTLIST_SCENE_H_

#include "cocos2d.h"
#include "ScreenShot.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class DictList : public LayerColor
{
public:
	DictList();
	~DictList();
	static Scene* createScene(std::string dictListName);
	static DictList* create(std::string dictListName);
	bool init(std::string dictListName);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	Size visibleSize;
	Vec2 origin;
	ScreenShot* screenShot;
	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	std::string dictListPath;
	void createDictList();
	void readDictList(std::string dictListPath);
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	ListView* listView;
};

#endif

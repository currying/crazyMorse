#ifndef __SHARE_SCENE_H__
#define __SHARE_SCENE_H__
#include "cocos2d.h"
#include "resources.h"
#include "sqlite3.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class ShareScene : public cocos2d::LayerColor
{
	Size visibleSize;
	Vec2 origin;
	std::string font_type;
	std::vector<common_class> commons;
	sqlite3 * sqlite;
	ssize_t currentIndex;
	void initData();
	void createListView();
	void selectedItemEvent(Ref* pSender, ListView::EventType type);
	void selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
	void share();
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ShareScene);
};
#endif
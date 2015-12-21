#ifndef _GAMECONFIGURE_SCENE_H_
#define _GAMECONFIGURE_SCENE_H_

#include "cocos2d.h"
#include "resources.h"
#include "sqlite3.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "CustomPageView.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class GameConfigureScene : public LayerColor
{
public:
	GameConfigureScene();
	~GameConfigureScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameConfigureScene);
	void onKeyReleased(EventKeyboard::KeyCode keycode, Event *event);
	void pageViewEvent(Ref *pSender, PageView::EventType type);
	Size visibleSize;
	Vec2 origin;
	__Dictionary *strings;
	std::string font_type;
	int font_size;
	CustomPageView* pageView;
	Layout* identitySelectlayout;
	Layout* dictSelectlayout;
	Layout* levelSelectlayout;
	void createCustomPage();
	void createIdentitySelectPage();
	void createDictSelectPage();
	void createLevelSelectPage();
	void readDictIndex();
	void readLevel();
	void createConfigure();
	void modifyConfigure(const char* identity, const char* language, const char* level);
	void onFocus(Ref * pObjc, Widget::TouchEventType type);
	Vector<Button*> buttons;
	Button* startButton;
	void startGame(Ref * pObjc, Widget::TouchEventType type);
	sqlite3 * sqlite;
};
#endif
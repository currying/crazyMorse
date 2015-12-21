#ifndef _CUSTOMPAGEVIEW_VIEW_H_
#define _CUSTOMPAGEVIEW_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
class CustomPageView : public PageView{
public:
	static CustomPageView* create();
	virtual void handleMoveLogic(Touch* touch) override;
};
#endif
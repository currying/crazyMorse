#ifndef _SCREENSHOT_LAYER_H_
#define _SCREENSHOT_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class ScreenShot : public Node
{
	Layer* mLayer;
public:
	ScreenShot();
	~ScreenShot();
	Size visibleSize;
	static ScreenShot* create();
	bool init();
	void saveImage();
	void saveScreenshot(const std::string& fileName, const std::function<void(const char*)>& screenShotCallback);
};

#endif
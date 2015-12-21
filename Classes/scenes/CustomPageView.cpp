#include "CustomPageView.h"
CustomPageView* CustomPageView::create(){
	CustomPageView* widget = new CustomPageView();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}
void CustomPageView::handleMoveLogic(Touch* touch){
}
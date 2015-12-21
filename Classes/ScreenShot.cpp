#include "ScreenShot.h"
ScreenShot::ScreenShot()
{
}
ScreenShot::~ScreenShot()
{
}
ScreenShot* ScreenShot::create()
{
	auto screenShot = new ScreenShot();
	if (screenShot && screenShot->init()) {
		screenShot->autorelease();
		return screenShot;
	}
	else
	{
		delete screenShot;
		screenShot = NULL;
		return NULL;
	}
}
bool ScreenShot::init()
{
	if (!Node::init()) {
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	return true;
}

void ScreenShot::saveScreenshot(const std::string& fileName, const std::function<void(const char*)>& screenShotCallback)
{
	Image::Format format;
	//进行后缀判断
	if (std::string::npos != fileName.find_last_of(".")){
		auto extension = fileName.substr(fileName.find_last_of("."), fileName.length());
		if (!extension.compare(".png")) {
			format = Image::Format::PNG;
		}
		else if (!extension.compare(".jpg")) {
			format = Image::Format::JPG;
		}
		else{
			log("cocos2d: the image can only be saved as JPG or PNG format");
			return;
		}
	}
	else {
		log("cocos2d: the image can only be saved as JPG or PNG format");
		return;
	}
	//获取屏幕尺寸，初始化一个空的渲染纹理对象
	auto renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
	//清空并开始获取
	renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	//遍历场景节点对象，填充纹理到RenderTexture中
	Director::getInstance()->getRunningScene()->visit();
	//结束获取
	renderTexture->end();
	//保存文件
	renderTexture->saveToFile(fileName, format);
	//log("path=%s", FileUtils::getInstance()->getWritablePath().append(fileName).c_str());
	//使用schedule在下一帧中调用callback函数
	auto fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
	auto scheduleCallback = [&, fullPath, screenShotCallback](float dt){
		screenShotCallback(fullPath.c_str());
	};
	auto _schedule = Director::getInstance()->getRunningScene()->getScheduler();
	_schedule->schedule(scheduleCallback, this, 0.0f, 0, 0.0f, false, "screenshot");
}

#include "AppDelegate.h"
#include "scenes/LoadScene.h"
#include "SimpleAudioEngine.h"
#include "resources.h"
USING_NS_CC;
using namespace CocosDenshion;
AppDelegate::AppDelegate() {
}
AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}
void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}
bool AppDelegate::applicationDidFinishLaunching() {
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::create("Crazy Morse");
        director->setOpenGLView(glview);
    }
    //director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
    glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::FIXED_HEIGHT);
    preloadMusic();
    auto scene = LoadScene::createScene();
    // run
    director->runWithScene(scene);
    
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
void AppDelegate::preloadMusic() {
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(morseMusic);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);
}

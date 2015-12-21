#ifndef _LOAD_SCENE_H_
#define _LOAD_SCENE_H_

#include "cocos2d.h"
#include "sqlite3.h"
class LoadScene : public cocos2d::LayerGradient
{
	int loadTag = 1;
public:
	LoadScene();
	~LoadScene();
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadScene);
	void menuScene();
	void initMorseCode();
	void initDict();
	sqlite3 * sqlite;
	void copyData();
};
#endif
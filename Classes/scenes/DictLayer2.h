#ifndef _DICT2_LAYER_H_
#define _DICT2_LAYER_H_

#include "cocos2d.h"
#include "sqlite3.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "resources.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace CocosDenshion;

class DictLayer2 : public Layer
{
	Size visibleSize;
	Vec2 origin;
public:
	DictLayer2();
	~DictLayer2();
	virtual bool init();
	CREATE_FUNC(DictLayer2);
private:
	sqlite3 * sqlite;//sqlite3
	void preloadEffect();//预加载音效
	void execSignal(Ref * pObjc, Widget::TouchEventType type);//按钮触发事件
	std::string convertStr(const char* str);//摩电码转换函数
	std::vector<dict_struct> dicts;//文字与摩斯密码键值列表
	int dict_signal;//键值列表序列号
	std::vector<dict_code> signalPosition;//dit与dah序列，用于判断按钮对错
	Vector<Sprite*> morseCodeSprites;//摩斯精灵，每个dit与dah都会创建一个精灵，dict滚动重新载入精灵
	void initLanguage(const char* level);//语言、字体初始化
	void initDict(const char* language, const char* level);//初始化字典
	void createDictLabel(const char* language, const char* level);
	void createTimeBar();//创建时间条
	void createMorseCodeSprites();
	void createDict(float dt);
	void createButton();
	void deleteObject(Node* sender);//删除分数显示精灵
	void complete();//完成后的显示精灵good、greate
	void completeCallback(Node* sender);//显示精灵回调
	void update(float dt);//计时器
	void initGroupTime_Score();
	void gameOver();//游戏结束
	void storeScore();
	//virtual void onToucheBegan(Touch* touche, Event *unused_event);
	//virtual void onToucheEnded(Touch* touche, Event *unused_event);
	float atomicWidth;
	float atomicHeight;
	float scale;
	Label* dictValueLabel;
	__Dictionary *strings;
	std::string font_type;
	int font_label_size;
	int font_code_size;
	//Label* ditScoreLabel;
	//Label* dahScoreLabel;
	Button* ditNormalButton;
	Button* dahNormalButton;
	float timeSeed;
	//Sequence* ditScore_seq;
	//Sequence* dahScore_seq;
public:
	static std::string language;//词汇显示语言
	static std::string level;
	size_t groupID;//组ID
	float groupLimitTime;//每组限定时间
	float groupFinishTime;//每组即时时间
	int groupScore;//每组基础分数
	size_t groupTimes;//每组次数
	size_t groupErrorTimes;//每组错误
	size_t groupMissTimes;//每组漏打
	//size_t sumMiss;//总漏打次数
	//size_t sumError;//总错误次数
	//size_t sumTimes;//总次数
	std::vector<group_score> group_scores;//每组限定时间与完成时间组
};

#endif

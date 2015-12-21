#ifndef _RESOURCES_H_
#define _RESOURCES_H_

//设计大小
//#define DESIGN_WIDTH 960
//#define DESIGN_HEIGHT 640

#define DESIGN_WIDTH 640
#define DESIGN_HEIGHT 960

//字体定义
#define font_marker_felt "fonts/Marker Felt.ttf" //Marker Felt 字体
#define font_droid_sans_fallback "fonts/Droid Sans Fallback.ttf"//中文
#define font_consolas "Consolas"//系统字体
#define font_loadscene_size 96
#define font_loadscene_color Color4B(0, 128, 255, 128), Size(4, -4)
#define font_menuitem_size 36
#define font_morsecode_size 28//电码表字体大小
#define font_dictmenu_size 48//电码本字体大小
#define font_dictlist_size 36//字典详细字体大小
#define font_codebook_size 48//电码本字体大小
#define font_gameover_size 40
#define font_game_size 48
#define font_highest_size 24

#define font_1_2_3_label_size 34
#define font_4_label_size 32
#define font_5_6_label_size 30

#define font_1_2_3_code_size 72
#define font_4_code_size 56
#define font_5_6_code_size 36

//场景切换
#define switchTime  2.0f//飞屏切换菜单场景时间
#define transitionTime 1.0f//飞屏切换菜单场景过渡效果时间
#define switchGameTime 0.3f//进入游戏配置时间
#define switchMenuTime 0.5f//进入主菜单时间
#define startGameTime 1.0f//游戏配置后进入游戏时间
#define gameOverSwitchMenuTime 1.0f//游戏结束返回主菜单时间

//国际化
#define il18n_chinese "chinese.xml"
#define il18n_english "english.xml"

//主菜单按钮
#define black_button "black_button.png"
#define black_select_button "black_select_button.png"

//音效
#define ditEffect "music/dit.mp3"
#define dahEffect "music/dah.mp3"
#define errorEffect "music/error.mp3"
//背景乐
#define morseMusic "music/morse.mp3"

//字典语言
#define dictLanguages {"en","zh_CN"}

//游戏难度
#define levels_ {"one","two","three","four","five","six"}

//字典、摩斯电码表
//#define morseCode  "morseCode.xml"
#define morseData  "data"
#define run_configure  "run_configure.xml"

#define timeBar_tag 123

//分数
#define score_one_fadein_duration 0.1f//单个分数渐隐时间
#define score_one_fadeout_duration 1.5f//单个分数渐隐时间
#define score_one_move_duration 0.6f//单个分数移动时间
#define score_one_rotation -20.0f//单个分数旋转角度

//闪光灯基准时间
#define base_time 0.6f

//游戏难度选择
class LevelItem{
public:
	std::string name;
	bool lock;
};
class common_class{
public:
	std::string value;
	std::string code;
	std::string translation;
};
class dict_struct{
public:
	std::string value;
	std::string morseCode;
	std::string translation;
};
class dict_code{
public:
	int position;
	bool isDit;
};
class group_score{
public:
	size_t groupID;
	float groupLimitTime;
	float groupFinishTime;
	size_t groupTimes;
	size_t groupErrorTimes;
	size_t groupMissTimes;
	int groupScore;
};
#endif

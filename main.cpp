#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#pragma comment (lib,"winmm.lib")
#include<graphics.h>
#include<conio.h>
#include<mmsystem.h>
#include<winuser.h>
#include <string.h>

#define NO 0 //不在拉回范围内
#define ud 1  //检查路径与转角
#define lr 2//u=up,d=down,l=left,r=right
#define udl 3//节点
#define udr 4
#define lru 5
#define lrd 6
#define dr 7
#define dl 8
#define ur 9
#define ul 10
#define all 11
#define bug 12

#define u 0//面部朝向
#define d 1
#define l 2
#define r 3

#define empty 0//食物种类
#define bean 1
#define big_bean 2
#define cherry 3

#define hunter 1//鬼状态：猎人/猎物
#define prey 0
#define alive 1//鬼生命值
#define dead 0

#define DEADTIME 1000//鬼死亡时间
#define PREYTIME 700  //鬼变猎物时间

#define PEACEFUL 0//游戏难度
#define EASY 1
#define NORMAL 2
#define HARD 3
#define HELL 4

int flag = 1;
int b = 15;//交界区块半径
int bb = 15;//四角交界区块半径
int selflock = 0;//自锁参数
int lockcnt = 0;//自锁计数
int difficulty = HELL;
int volume = 250;
int NameNum = 0;
char Name[100] = "";
int win;

char GetInputChar()
{
	char c;
	if (_kbhit()) // 如果有键盘输入
	{
		c = _getch(); // 获取输入的字符
		return c; // 返回该字符
	}
	else // 如果没有键盘输入
	{
		return 0; // 返回0
	}
}

typedef struct PlayerInfo
{
	int _num = 0;
	char _name[1000] = "";
	int _score = 0;
}PInfo;
PInfo playerinfo;

typedef struct PlayerScore
{
	char _name[100] = "";
	int _score = 0;
}PScore;

struct junctionpostion
{
	int x = 0;
	int y = 0;
}junctionpos[64];
void initjunctionpostion()
{
	int i;
	for (i = 0; i < 6; i++)
	{
		junctionpos[i].y = 120;
	}
	for (i = 6; i < 14; i++)
	{
		junctionpos[i].y = 195;
	}
	for (i = 14; i < 22; i++)
	{
		junctionpos[i].y = 245;
	}
	for (i = 22; i < 26; i++)
	{
		junctionpos[i].y = 285;
	}
	for (i = 26; i < 30; i++)
	{
		junctionpos[i].y = 340;
	}
	for (i = 30; i < 32; i++)
	{
		junctionpos[i].y = 375;
	}
	for (i = 32; i < 40; i++)
	{
		junctionpos[i].y = 430;
	}
	for (i = 40; i < 50; i++)
	{
		junctionpos[i].y = 465;
	}
	for (i = 50; i < 60; i++)
	{
		junctionpos[i].y = 520;
	}
	for (i = 60; i < 64; i++)
	{
		junctionpos[i].y = 555;
	}
	junctionpos[0].x = 80; //row1
	junctionpos[1].x = 145;
	junctionpos[2].x = 220;
	junctionpos[3].x = 260;
	junctionpos[4].x = 330;
	junctionpos[5].x = 400;

	junctionpos[6].x = 80;//row2
	junctionpos[7].x = 145;
	junctionpos[8].x = 185;
	junctionpos[9].x = 220;
	junctionpos[10].x = 260;
	junctionpos[11].x = 290;
	junctionpos[12].x = 330;
	junctionpos[13].x = 400;

	junctionpos[14].x = 80;//row3
	junctionpos[15].x = 145;
	junctionpos[16].x = 185;
	junctionpos[17].x = 220;
	junctionpos[18].x = 260;
	junctionpos[19].x = 290;
	junctionpos[20].x = 330;
	junctionpos[21].x = 400;

	junctionpos[22].x = 185;//row4
	junctionpos[23].x = 220;
	junctionpos[24].x = 260;
	junctionpos[25].x = 290;

	junctionpos[26].x = 145;//row5
	junctionpos[27].x = 185;
	junctionpos[28].x = 290;
	junctionpos[29].x = 330;

	junctionpos[30].x = 185;//row6
	junctionpos[31].x = 290;

	junctionpos[32].x = 80;//row7
	junctionpos[33].x = 145;
	junctionpos[34].x = 185;
	junctionpos[35].x = 220;
	junctionpos[36].x = 260;
	junctionpos[37].x = 290;
	junctionpos[38].x = 330;
	junctionpos[39].x = 400;

	junctionpos[40].x = 80;//row8
	junctionpos[41].x = 115;
	junctionpos[42].x = 145;
	junctionpos[43].x = 185;
	junctionpos[44].x = 220;
	junctionpos[45].x = 260;
	junctionpos[46].x = 290;
	junctionpos[47].x = 330;
	junctionpos[48].x = 365;
	junctionpos[49].x = 400;

	junctionpos[50].x = 80;//row9
	junctionpos[51].x = 115;
	junctionpos[52].x = 145;
	junctionpos[53].x = 185;
	junctionpos[54].x = 215;
	junctionpos[55].x = 260;
	junctionpos[56].x = 290;
	junctionpos[57].x = 330;
	junctionpos[58].x = 365;
	junctionpos[59].x = 400;

	junctionpos[60].x = 80;//row10
	junctionpos[61].x = 215;
	junctionpos[62].x = 260;
	junctionpos[63].x = 400;


}

struct food
{
	int sort = empty;
	int x = 0;
	int y = 0;
	int points = 0;
} Food[300];
void initfood()
{

	for (int i = 0; i < 270; i++)//全部设置为小豆子
	{
		Food[i].sort = bean;
		Food[i].points = 10;
	}
	int interval = 15;

	//设置食物坐标
	//先画横线
	for (int i = 0; i <= 9; i++)//l 1-3
	{
		Food[i].y = junctionpos[1 - 1].y;
		Food[i].x = junctionpos[1 - 1].x + i * interval;
	}
	for (int i = 0; i <= 9; i++)//l 4-6
	{
		Food[i + 10].y = junctionpos[4 - 1].y;
		Food[i + 10].x = junctionpos[4 - 1].x + i * interval;
	}
	for (int i = 0; i <= 21; i++)//l 7-14
	{
		Food[i + 20].y = junctionpos[7 - 1].y;
		Food[i + 20].x = junctionpos[7 - 1].x + i * interval;
	}
	for (int i = 0; i <= 4; i++)//l 15-16
	{
		Food[i + 42].y = junctionpos[15 - 1].y;
		Food[i + 42].x = junctionpos[15 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 17-18
	{
		Food[i + 47].y = junctionpos[17 - 1].y;
		Food[i + 47].x = junctionpos[17 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 19-20
	{
		Food[i + 50].y = junctionpos[19 - 1].y;
		Food[i + 50].x = junctionpos[19 - 1].x + i * interval;
	}
	for (int i = 0; i <= 4; i++)//l 21-22
	{
		Food[i + 53].y = junctionpos[21 - 1].y;
		Food[i + 53].x = junctionpos[21 - 1].x + i * interval;
	}
	for (int i = 0; i <= 7; i++)//l 23-26
	{
		Food[i + 58].y = junctionpos[23 - 1].y;
		Food[i + 58].x = junctionpos[23 - 1].x + i * interval;
	}
	for (int i = 0; i <= 8; i++)//l *-28
	{
		Food[i + 66].y = junctionpos[28 - 1].y;
		Food[i + 66].x = junctionpos[28 - 1].x - i * interval;
	}
	for (int i = 0; i <= 8; i++)//l 29-*
	{
		Food[i + 75].y = junctionpos[29 - 1].y;
		Food[i + 75].x = junctionpos[29 - 1].x + i * interval;
	}
	for (int i = 0; i <= 7; i++)//l 31-32
	{
		Food[i + 84].y = junctionpos[31 - 1].y;
		Food[i + 84].x = junctionpos[31 - 1].x + i * interval;
	}
	for (int i = 0; i <= 9; i++)//l 33-36
	{
		Food[i + 92].y = junctionpos[33 - 1].y;
		Food[i + 92].x = junctionpos[33 - 1].x + i * interval;
	}
	for (int i = 0; i <= 9; i++)//l 37-40
	{
		Food[i + 102].y = junctionpos[37 - 1].y;
		Food[i + 102].x = junctionpos[37 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 41-42
	{
		Food[i + 112].y = junctionpos[41 - 1].y;
		Food[i + 112].x = junctionpos[41 - 1].x + i * interval;
	}
	for (int i = 0; i <= 12; i++)//l 43-48
	{
		Food[i + 115].y = junctionpos[43 - 1].y;
		Food[i + 115].x = junctionpos[43 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 49-50
	{
		Food[i + 128].y = junctionpos[49 - 1].y;
		Food[i + 128].x = junctionpos[49 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 49-50
	{
		Food[i + 128].y = junctionpos[49 - 1].y;
		Food[i + 128].x = junctionpos[49 - 1].x + i * interval;
	}
	for (int i = 0; i <= 4; i++)//l 51-53
	{
		Food[i + 131].y = junctionpos[51 - 1].y;
		Food[i + 131].x = junctionpos[51 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 54-55
	{
		Food[i + 136].y = junctionpos[54 - 1].y;
		Food[i + 136].x = junctionpos[54 - 1].x + i * interval;
	}
	for (int i = 0; i <= 2; i++)//l 56-57
	{
		Food[i + 139].y = junctionpos[56 - 1].y;
		Food[i + 139].x = junctionpos[56 - 1].x + i * interval;
	}
	for (int i = 0; i <= 4; i++)//l 58-60
	{
		Food[i + 142].y = junctionpos[58 - 1].y;
		Food[i + 142].x = junctionpos[58 - 1].x + i * interval;
	}
	for (int i = 0; i <= 21; i++)//l 61-64
	{
		Food[i + 147].y = junctionpos[61 - 1].y;
		Food[i + 147].x = junctionpos[61 - 1].x + i * interval;
	}

	//再画竖线
	for (int i = 1; i <= 4; i++)//r 1-7
	{
		Food[i + 169].y = junctionpos[1 - 1].y + i * interval;
		Food[i + 169].x = junctionpos[1 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 7-15
	{
		Food[i + 173].y = junctionpos[7 - 1].y + i * interval;
		Food[i + 173].x = junctionpos[7 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 33-41
	{
		Food[i + 175].y = junctionpos[33 - 1].y + i * interval;
		Food[i + 175].x = junctionpos[33 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 51-61
	{
		Food[i + 176].y = junctionpos[51 - 1].y + i * interval;
		Food[i + 176].x = junctionpos[51 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 42-52
	{
		Food[i + 177].y = junctionpos[42 - 1].y + i * interval;
		Food[i + 177].x = junctionpos[42 - 1].x;
	}
	for (int i = 1; i <= 4; i++)//r 2-8
	{
		Food[i + 179].y = junctionpos[2 - 1].y + i * interval;
		Food[i + 179].x = junctionpos[2 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 8-16
	{
		Food[i + 183].y = junctionpos[8 - 1].y + i * interval;
		Food[i + 183].x = junctionpos[8 - 1].x;
	}
	for (int i = 1; i <= 5; i++)//r 16-27
	{
		Food[i + 185].y = junctionpos[16 - 1].y + i * interval;
		Food[i + 185].x = junctionpos[16 - 1].x;
	}
	for (int i = 1; i <= 5; i++)//r 27-34
	{
		Food[i + 190].y = junctionpos[27 - 1].y + i * interval;
		Food[i + 190].x = junctionpos[27 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 34-43
	{
		Food[i + 195].y = junctionpos[34 - 1].y + i * interval;
		Food[i + 195].x = junctionpos[34 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 43-53
	{
		Food[i + 196].y = junctionpos[43 - 1].y + i * interval;
		Food[i + 196].x = junctionpos[43 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 9-17
	{
		Food[i + 200].y = junctionpos[9 - 1].y + i * interval;
		Food[i + 200].x = junctionpos[9 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 23-28
	{
		Food[i + 202].y = junctionpos[23 - 1].y + i * interval;
		Food[i + 202].x = junctionpos[23 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 28-31
	{
		Food[i + 204].y = junctionpos[28 - 1].y + i * interval;
		Food[i + 204].x = junctionpos[28 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 31-35
	{
		Food[i + 205].y = junctionpos[31 - 1].y + i * interval;
		Food[i + 205].x = junctionpos[31 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 44-54
	{
		Food[i + 208].y = junctionpos[44 - 1].y + i * interval;
		Food[i + 208].x = junctionpos[44 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 18-24
	{
		Food[i + 210].y = junctionpos[18 - 1].y + i * interval;
		Food[i + 210].x = junctionpos[18 - 1].x - 5;////////////////////////修改过！！！
	}
	for (int i = 1; i <= 1; i++)//r 36-45
	{
		Food[i + 212].y = junctionpos[36 - 1].y + i * interval;
		Food[i + 212].x = junctionpos[36 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 55-62
	{
		Food[i + 213].y = junctionpos[55 - 1].y + i * interval;
		Food[i + 213].x = junctionpos[55 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 19-25
	{
		Food[i + 214].y = junctionpos[19 - 1].y + i * interval;
		Food[i + 214].x = junctionpos[19 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 37-46
	{
		Food[i + 216].y = junctionpos[37 - 1].y + i * interval;
		Food[i + 216].x = junctionpos[37 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 56-63
	{
		Food[i + 217].y = junctionpos[56 - 1].y + i * interval;
		Food[i + 217].x = junctionpos[56 - 1].x;
	}
	for (int i = 1; i <= 4; i++)//r 3-10
	{
		Food[i + 218].y = junctionpos[3 - 1].y + i * interval;
		Food[i + 218].x = junctionpos[3 - 1].x;
	}
	for (int i = 1; i <= 4; i++)//r 4-11
	{
		Food[i + 222].y = junctionpos[4 - 1].y + i * interval;
		Food[i + 222].x = junctionpos[4 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 12-20
	{
		Food[i + 226].y = junctionpos[12 - 1].y + i * interval;
		Food[i + 226].x = junctionpos[12 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 26-29
	{
		Food[i + 228].y = junctionpos[26 - 1].y + i * interval;
		Food[i + 228].x = junctionpos[26 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 29-32
	{
		Food[i + 231].y = junctionpos[29 - 1].y + i * interval;
		Food[i + 231].x = junctionpos[29 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 32-38
	{
		Food[i + 232].y = junctionpos[32 - 1].y + i * interval;
		Food[i + 232].x = junctionpos[32 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 47-57
	{
		Food[i + 235].y = junctionpos[47 - 1].y + i * interval;
		Food[i + 235].x = junctionpos[47 - 1].x;
	}
	for (int i = 1; i <= 4; i++)//r 5-13
	{
		Food[i + 238].y = junctionpos[5 - 1].y + i * interval;
		Food[i + 238].x = junctionpos[5 - 1].x;
	}
	for (int i = 1; i <= 2; i++)//r 13-21
	{
		Food[i + 242].y = junctionpos[13 - 1].y + i * interval;
		Food[i + 242].x = junctionpos[13 - 1].x;
	}
	for (int i = 1; i <= 5; i++)//r 21-30
	{
		Food[i + 244].y = junctionpos[21 - 1].y + i * interval;
		Food[i + 244].x = junctionpos[21 - 1].x;
	}
	for (int i = 1; i <= 5; i++)//r 30-39
	{
		Food[i + 249].y = junctionpos[30 - 1].y + i * interval;
		Food[i + 249].x = junctionpos[30 - 1].x;
	}
	for (int i = 1; i <= 1; i++)//r 39-48
	{
		Food[i + 254].y = junctionpos[39 - 1].y + i * interval;
		Food[i + 254].x = junctionpos[39 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 48-58
	{
		Food[i + 255].y = junctionpos[48 - 1].y + i * interval;
		Food[i + 255].x = junctionpos[48 - 1].x;
	}
	for (int i = 1; i <= 3; i++)//r 49-59
	{
		Food[i + 258].y = junctionpos[49 - 1].y + i * interval;
		Food[i + 258].x = junctionpos[49 - 1].x;
	}
	for (int i = 1; i <= 4; i++)//r 6-14
	{
		Food[i + 261].y = junctionpos[6 - 1].y + i * interval;
		Food[i + 261].x = junctionpos[6 - 1].x - 5;////////////////////
	}
	for (int i = 1; i <= 3; i++)//r 14-22
	{
		Food[i + 265].y = junctionpos[14 - 1].y + i * interval;
		Food[i + 265].x = junctionpos[14 - 1].x - 5;////////////////////
	}
	for (int i = 1; i <= 1; i++)//r 40-50
	{
		Food[i + 268].y = junctionpos[40 - 1].y + i * interval;
		Food[i + 268].x = junctionpos[40 - 1].x - 5;/////////////////////
	}
	for (int i = 1; i <= 1; i++)//r 60-64
	{
		Food[i + 269].y = junctionpos[60 - 1].y + i * interval;
		Food[i + 269].x = junctionpos[60 - 1].x - 5;/////////////////////
	}

	Food[87].sort = empty;/////////吃豆人出生点不放豆子
	Food[87].points = 0;
	Food[88].sort = empty;
	Food[88].points = 0;

	//////////////////////////////随机生成///////////////////////////////

	static unsigned int seq = 0;
	srand(time(0) + (seq++));
	int bbn = rand() % 4 + 5;
	int cn = rand() % 3 + 3;
	int* bigbeannum = (int*)malloc(sizeof(int) * bbn);
	int* cherrynum = (int*)malloc(sizeof(int) * cn);

	for (int i = 0; i < bbn; i++)
	{
		bigbeannum[i] = rand() % 268;
	}
	for (int i = 0; i < cn; i++)
	{
		cherrynum[i] = rand() % 268;
	}

	for (int i = 0; i < bbn; i++)
	{
		Food[bigbeannum[i]].sort = big_bean;
		Food[bigbeannum[i]].points = 50;
	}
	for (int i = 0; i < cn; i++)
	{
		Food[cherrynum[i]].sort = cherry;
		Food[cherrynum[i]].points = 200;
	}
}

struct pacman//创建吃豆人即玩家
{
	int pos[2] = { 235,375 };
	int speed = 1;
	int health = 3;
	int face = l;
	int score = 0;
}player;
void initplayer()
{
	player.pos[0] = 235;
	player.pos[1] = 375;
	player.speed = 1;
	player.health = 3;
	player.face = l;
	player.score = 0;
}
void eatfood();
void GhostsEatMan();
void ManEatGhosts();

struct ghost
{
	int x = 0;
	int y = 0;
	int state = hunter;
	int face = r;
	const int points = 500;
	int atHome = 1;
	int speed = 1;
	int life = alive;
	int deadtime = 0;
	int orix = 0;
	int oriy = 0;
	int randseed = 0;
	int preytime = 0;
}ghostr, ghostp, ghosty, ghostb;
void initghosts()
{

	ghostb.x = 215;
	ghostb.y = 305;
	ghostb.orix = 215;
	ghostb.oriy = 305;
	ghostb.randseed = 1;

	ghostr.x = 245;
	ghostr.y = 305;
	ghostr.orix = 245;
	ghostr.oriy = 305;
	ghostr.randseed = 2;

	ghosty.x = 215;
	ghosty.y = 330;
	ghosty.orix = 215;
	ghosty.oriy = 330;
	ghosty.randseed = 3;

	ghostp.x = 245;
	ghostp.y = 330;
	ghostp.orix = 245;
	ghostp.oriy = 330;
	ghostp.randseed = 4;
}

void eatfood()
{
	{
		for (int i = 0; i < 270; i++)
		{
			if (Food[i].x >= player.pos[0] - 10 && Food[i].x <= player.pos[0] + 10 && Food[i].y >= player.pos[1] - 10 && Food[i].y <= player.pos[1] + 10)
			{
				if (Food[i].sort == big_bean)
				{
					ghostb.state = prey;
					ghostr.state = prey;
					ghosty.state = prey;
					ghostp.state = prey;

				}
				Food[i].sort = empty;
				player.score += Food[i].points;
				Food[i].points = 0;
			}
		}
	}
}

struct junc
{
	int junction = 0;
	int type = udr;
};
junc checkjunctionarea(int x, int y)
{
	junc junc1;
	junc1.junction = 0;
	junc1.type = 0;
	//先检查三向点
	if (x > junctionpos[2 - 1].x - b && x < junctionpos[2 - 1].x + b && y == junctionpos[2 - 1].y)//2
	{
		junc1.junction = 2 - 1;
		junc1.type = lrd;
	}
	else if (x > junctionpos[5 - 1].x - b && x < junctionpos[5 - 1].x + b && y == junctionpos[5 - 1].y)//5
	{
		junc1.junction = 5 - 1;
		junc1.type = lrd;
	}
	else if (y > junctionpos[7 - 1].y - b && y < junctionpos[7 - 1].y + b && x == junctionpos[7 - 1].x)//7
	{
		junc1.junction = 7 - 1;
		junc1.type = udr;
	}
	else if (x > junctionpos[9 - 1].x - b && x < junctionpos[9 - 1].x + b && y == junctionpos[9 - 1].y)//9
	{
		junc1.junction = 9 - 1;
		junc1.type = lrd;
	}
	else if (x > junctionpos[10 - 1].x - b && x < junctionpos[10 - 1].x + b && y == junctionpos[10 - 1].y)//10
	{
		junc1.junction = 10 - 1;
		junc1.type = lru;
	}
	else if (x > junctionpos[11 - 1].x - b && x < junctionpos[11 - 1].x + b && y == junctionpos[11 - 1].y)//11
	{
		junc1.junction = 11 - 1;
		junc1.type = lru;
	}
	else if (y > junctionpos[14 - 1].y - b && y < junctionpos[14 - 1].y + b && x == junctionpos[14 - 1].x)//14
	{
		junc1.junction = 14 - 1;
		junc1.type = udl;
	}
	else if (y > junctionpos[16 - 1].y - b && y < junctionpos[16 - 1].y + b && x == junctionpos[16 - 1].x)//16
	{
		junc1.junction = 16 - 1;
		junc1.type = udl;
	}
	//21
	else if (y > junctionpos[21 - 1].y - b && y < junctionpos[21 - 1].y + b && x == junctionpos[21 - 1].x)
	{
		junc1.junction = 21 - 1;
		junc1.type = udr;
	}
	//24 
	else if (x > junctionpos[24 - 1].x - b && x < junctionpos[24 - 1].x + b && y == junctionpos[24 - 1].y)
	{
		junc1.junction = 24 - 1;
		junc1.type = lru;
	}
	//25
	else if (x > junctionpos[25 - 1].x - b && x < junctionpos[25 - 1].x + b && y == junctionpos[25 - 1].y)
	{
		junc1.junction = 25 - 1;
		junc1.type = lru;
	}
	//28
	else if (y > junctionpos[28 - 1].y - b && y < junctionpos[28 - 1].y + b && x == junctionpos[28 - 1].x)
	{
		junc1.junction = 28 - 1;
		junc1.type = udl;
	}
	//29
	else if (y > junctionpos[29 - 1].y - b && y < junctionpos[29 - 1].y + b && x == junctionpos[29 - 1].x)
	{
		junc1.junction = 29 - 1;
		junc1.type = udr;
	}
	//31
	else if (y > junctionpos[31 - 1].y - b && y < junctionpos[31 - 1].y + b && x == junctionpos[31 - 1].x)
	{
		junc1.junction = 31 - 1;
		junc1.type = udr;
	}
	//32
	else if (y > junctionpos[32 - 1].y - b && y < junctionpos[32 - 1].y + b && x == junctionpos[32 - 1].x)
	{
		junc1.junction = 32 - 1;
		junc1.type = udl;
	}
	//35
	else if (x > junctionpos[35 - 1].x - b && x < junctionpos[35 - 1].x + b && y == junctionpos[35 - 1].y)
	{
		junc1.junction = 35 - 1;
		junc1.type = lru;
	}
	//38
	else if (x > junctionpos[38 - 1].x - b && x < junctionpos[38 - 1].x + b && y == junctionpos[38 - 1].y)
	{
		junc1.junction = 38 - 1;
		junc1.type = lru;
	}
	//43
	else if (y > junctionpos[43 - 1].y - b && y < junctionpos[43 - 1].y + b && x == junctionpos[43 - 1].x)
	{
		junc1.junction = 43 - 1;
		junc1.type = udr;
	}
	//44
	else if (x > junctionpos[44 - 1].x - b && x < junctionpos[44 - 1].x + b && y == junctionpos[44 - 1].y)
	{
		junc1.junction = 44 - 1;
		junc1.type = lrd;
	}
	//45
	else if (x > junctionpos[45 - 1].x - b && x < junctionpos[45 - 1].x + b && y == junctionpos[45 - 1].y)
	{
		junc1.junction = 45 - 1;
		junc1.type = lru;
	}
	//46
	else if (x > junctionpos[46 - 1].x - b && x < junctionpos[46 - 1].x + b && y == junctionpos[46 - 1].y)
	{
		junc1.junction = 46 - 1;
		junc1.type = lru;
	}
	//47
	else if (x > junctionpos[47 - 1].x - b && x < junctionpos[47 - 1].x + b && y == junctionpos[47 - 1].y)
	{
		junc1.junction = 47 - 1;
		junc1.type = lrd;
	}
	//48
	else if (y > junctionpos[48 - 1].y - b && y < junctionpos[48 - 1].y + b && x == junctionpos[48 - 1].x)
	{
		junc1.junction = 48 - 1;
		junc1.type = udl;
	}
	//52
	else if (x > junctionpos[52 - 1].x - b && x < junctionpos[52 - 1].x + b && y == junctionpos[52 - 1].y)
	{
		junc1.junction = 52 - 1;
		junc1.type = lru;
	}
	//59
	else if (x > junctionpos[59 - 1].x - b && x < junctionpos[59 - 1].x + b && y == junctionpos[59 - 1].y)
	{
		junc1.junction = 59 - 1;
		junc1.type = lru;
	}
	//62
	else if (x > junctionpos[62 - 1].x - b && x < junctionpos[62 - 1].x + b && y == junctionpos[62 - 1].y)
	{
		junc1.junction = 62 - 1;
		junc1.type = lru;
	}
	//63
	else if (x > junctionpos[63 - 1].x - b && x < junctionpos[63 - 1].x + b && y == junctionpos[63 - 1].y)
	{
		junc1.junction = 63 - 1;
		junc1.type = lru;
	}


	//再绘制四向点（区域）
	else if (x >= junctionpos[8 - 1].x - bb && x <= junctionpos[8 - 1].x + bb && y >= junctionpos[8 - 1].y - bb && y <= junctionpos[8 - 1].y + bb)//j 8
	{
		junc1.junction = 8 - 1;
		junc1.type = all;
	}
	else if (x >= junctionpos[13 - 1].x - bb && x <= junctionpos[13 - 1].x + bb && y >= junctionpos[13 - 1].y - bb && y <= junctionpos[13 - 1].y + bb)//j 13
	{
		junc1.junction = 13 - 1;
		junc1.type = all;
	}
	else if (x >= junctionpos[27 - 1].x - bb && x <= junctionpos[27 - 1].x + bb && y >= junctionpos[27 - 1].y - bb && y <= junctionpos[27 - 1].y + bb)//j 27 
	{
		junc1.junction = 27 - 1;
		junc1.type = all;
	}
	else if (x >= junctionpos[30 - 1].x - bb && x <= junctionpos[30 - 1].x + bb && y >= junctionpos[30 - 1].y - bb && y <= junctionpos[30 - 1].y + bb)//j 30
	{
		junc1.junction = 30 - 1;
		junc1.type = all;
	}
	else if (x >= junctionpos[34 - 1].x - bb && x <= junctionpos[34 - 1].x + bb && y >= junctionpos[34 - 1].y - bb && y <= junctionpos[34 - 1].y + bb)//j 34
	{
		junc1.junction = 34 - 1;
		junc1.type = all;
	}
	else if (x >= junctionpos[39 - 1].x - bb && x <= junctionpos[39 - 1].x + bb && y >= junctionpos[39 - 1].y - bb && y <= junctionpos[39 - 1].y + bb)//j 39 
	{
		junc1.junction = 39 - 1;
		junc1.type = all;
	}
	else
	{
		junc1.junction = 0;
		junc1.type = 0;
	}
	return junc1;
}
int checkpath(int x, int y)
{

	//先判断单线
	if (x > 80 && x < 400 && y == 555 && x != 215 && x != 260)//l 1
		return lr;
	if (x == 80 && y > 520 && y < 555)//l 2
		return ud;
	if (x != 115 && x > 80 && x < 145 && y == 520)//l 3
		return lr;
	if (x == 115 && y > 465 && y < 520)//l 4
		return ud;
	if (y != 465 && y != 430 && y != 340 && y != 245 && y != 195 && x == 145 && y > 120 && y < 520)//l 5
		return ud;
	if (x > 80 && x < 115 && y == 465)//l 6
		return lr;
	if (x == 80 && y > 430 && y < 465)//l 7
		return ud;
	if (x != 145 && x != 185 && x > 80 && x < 220 && y == 430)//l 8
		return lr;
	if (x == 220 && y > 430 && y < 465)//l 9
		return ud;
	if (x != 290 && x != 185 && x != 220 && x != 260 && x > 145 && x < 330 && y == 465)//l 10
		return lr;
	if (x == 185 && y > 465 && y < 520)//l 11
		return ud;
	if (x > 185 && x < 215 && y == 520)//l 12
		return lr;
	if (x == 215 && y > 520 && y < 555)//l 13
		return ud;
	if (x == 260 && y > 520 && y < 555)//l 14
		return ud;
	if (x > 260 && x < 290 && y == 520)//l 15
		return lr;
	if (x == 290 && y > 465 && y < 520)//l 16
		return ud;
	if (y != 465 && y != 430 && y != 340 && y != 245 && y != 195 && x == 330 && y > 120 && y < 520)//l 17
		return ud;
	if (x != 365 && x > 330 && x < 400 && y == 520)//l 18
		return lr;
	if (x == 400 && y > 520 && y < 555)//l 19
		return ud;
	if (x == 365 && y > 465 && y < 520)//l 20
		return ud;
	if (x > 365 && x < 400 && y == 465)//l 21
		return lr;
	if (x == 400 && y > 430 && y < 465)//l 22
		return ud;
	if (x != 290 && x != 330 && x > 260 && x < 400 && y == 430)//l 23
		return lr;
	if (x == 260 && y > 430 && y < 465)//l 24
		return ud;
	if (y != 375 && y != 340 && x == 185 && y > 285 && y < 430)//l 25
		return ud;
	if (y != 375 && y != 340 && x == 290 && y > 285 && y < 430)//l 26
		return ud;
	if (x > 185 && x < 290 && y == 375)//l 27
		return lr;
	if (x != 145 && x > 60 && x < 185 && y == 340)//l 28
		return lr;
	if (x != 330 && x > 290 && x < 420 && y == 340)//l 29
		return lr;
	if (x != 220 && x != 260 && x > 185 && x < 290 && y == 285)//l 30
		return lr;
	if (x > 80 && x < 145 && y == 245)//l 31
		return lr;
	if (x > 185 && x < 220 && y == 245)//l 32
		return lr;
	if (x == 220 && y > 245 && y < 285)//l 33
		return ud;
	if (x == 260 && y > 245 && y < 285)//l 34
		return ud;
	if (x > 260 && x < 290 && y == 245)//l 35
		return lr;
	if (x > 330 && x < 400 && y == 245)//l 36
		return lr;
	if (y != 195 && x == 80 && y > 120 && y < 245)//l 37
		return ud;
	if (y != 195 && x == 400 && y > 120 && y < 245)//l 38
		return ud;
	if (x == 185 && y > 195 && y < 245)//l 39
		return ud;
	if (x == 290 && y > 195 && y < 245)//l 40
		return ud;
	if (x != 145 && x != 185 && x != 220 && x != 260 && x != 290 && x != 330 && x > 80 && x < 400 && y == 195)//l 41
		return lr;
	if (x == 220 && y > 120 && y < 195)//l 42
		return ud;
	if (x == 260 && y > 120 && y < 195)//l 43
		return ud;
	if (x != 145 && x > 80 && x < 220 && y == 120)//l 44
		return lr;
	if (x != 330 && x > 260 && x < 400 && y == 120)//l 45
		return lr;

	//再判断节点
	if (y == 120)//row1
	{
		if (x == 80)//j 1
			return dr;
		if (x == 145)//j 2
			return lrd;
		if (x == 220)//j 3
			return dl;
		if (x == 260)//j 4
			return dr;
		if (x == 330)//j 5
			return lrd;
		if (x == 400)//j 6
			return dl;
	}
	if (y == 195)//row2
	{
		if (x == 80)//j 7
			return udr;
		if (x == 145)//j 8
			return all;
		if (x == 185)//j 9
			return lrd;
		if (x == 220)//j 10
			return lru;
		if (x == 260)//j 11
			return lru;
		if (x == 290)//j 12
			return lrd;
		if (x == 330)//j 13
			return all;
		if (x == 400)//j 14
			return udl;
	}
	if (y == 245)//row3
	{
		if (x == 80)//j 15
			return ur;
		if (x == 145)//j 16
			return udl;
		if (x == 185)//j 17
			return ur;
		if (x == 220)//j 18
			return dl;
		if (x == 260)//j 19
			return dr;
		if (x == 290)//j 20
			return ul;
		if (x == 330)//j 21
			return udr;
		if (x == 400)//j 22
			return ul;
	}
	if (y == 285)//row4
	{
		if (x == 185)//j 23
			return dr;
		if (x == 220)//j 24
			return lru;
		if (x == 260)//j 25
			return lru;
		if (x == 290)//j 26
			return dl;
	}
	if (y == 340)//row5
	{
		if (x == 145)//j 27
			return all;
		if (x == 185)//j 28
			return udl;
		if (x == 290)//j 29
			return udr;
		if (x == 330)//j 30
			return all;
	}
	if (y == 375)//row6
	{
		if (x == 185)//j 31
			return udr;
		if (x == 290)//j 32
			return udl;
	}
	if (y == 430)//row7
	{
		if (x == 80)//j 33
			return dr;
		if (x == 145)//j 34
			return all;
		if (x == 185)//j 35
			return lru;
		if (x == 220)//j 36
			return dl;
		if (x == 260)//j 37
			return dr;
		if (x == 290)//j 38
			return lru;
		if (x == 330)//j 39
			return all;
		if (x == 400)//j 40
			return dl;
	}
	if (y == 465)//row8
	{
		if (x == 80)//j 41
			return ur;
		if (x == 115)//j 42
			return dl;
		if (x == 145)//j 43
			return udr;
		if (x == 185)//j 44
			return lrd;
		if (x == 220)//j 45
			return lru;
		if (x == 260)//j 46
			return lru;
		if (x == 290)//j 47
			return lrd;
		if (x == 330)//j 48
			return udl;
		if (x == 365)//j 49
			return dr;
		if (x == 400)//j 50
			return ul;
	}
	if (y == 520)//row9
	{
		if (x == 80)//j 51
			return dr;
		if (x == 115)//j 52
			return lru;
		if (x == 145)//j 53
			return ul;
		if (x == 185)//J 54
			return ur;
		if (x == 215)//j 55
			return dl;
		if (x == 260)//j 56
			return dr;
		if (x == 290)//j 57
			return ul;
		if (x == 330)//j 58
			return ur;
		if (x == 365)//j 59
			return lru;
		if (x == 400)//j 60
			return dl;
	}
	if (y == 555)//row10
	{
		if (x == 80)//j 61
			return ur;
		if (x == 215)//j 62
			return lru;
		if (x == 260)//j 63
			return lru;
		if (x == 400)//j 64
			return ul;
	}
	return NO;



}
void unlock()
{
	if (selflock == 1 && lockcnt > 2)
	{
		selflock = 0;
		lockcnt = 0;
	}
}

void title()
{
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "吃豆人！");
}
void PlaystartBGM()
{
	char arr[100]= "setaudio start volume to ";
    char temp[10];
	_itoa_s(volume, temp, 10);
	strcat_s(arr,temp);
	char end[] = "";
	strcat_s(arr, end);

	mciSendString("open ./bgmstart.mp3 alias start", 0, 0, 0);//起别名，以后调整音量时就使用这个别名来定位音乐
	mciSendString((LPCSTR)arr, 0, 0, 0);
	mciSendString("play start repeat", 0, 0, 0);
}
void StopstartBGM()
{
	mciSendString("stop start", 0, 0, 0);
}
void PlaygameBGM()
{
	char arr[100] = "setaudio game volume to ";
	char temp[10];
	_itoa_s(volume, temp, 10);
	strcat_s(arr, temp);
	mciSendString("open ./bgmgame.mp3 alias game", 0, 0, 0);
	mciSendString((LPCSTR)arr, 0, 0, 0);
	mciSendString("play game repeat", 0, 0, 0);
}
void StopgameBGM()
{
	mciSendString("stop game", 0, 0, 0);
}
void SetVolume(int num)
{
	if (num == 1)
	{
		char arr[100] = "setaudio start volume to ";
		char temp[10];
		_itoa_s(volume, temp, 10);
		strcat_s(arr, temp);
		mciSendString((LPCSTR)arr, 0, 0, 0);
	}
	else if (num == 2)
	{
		char arr[100] = "setaudio game volume to ";
		char temp[10];
		_itoa_s(volume, temp, 10);
		strcat_s(arr, temp);
		mciSendString((LPCSTR)arr, 0, 0, 0);
	}
}
int Guide();
int Options();
int FOutInfo()//将玩家信息写入文件
{
	int i = 0;
	char str[100] = "", score[100] = "", enter[2] ="\n",space[2]=" ";
	FILE* file;
	fopen_s(&file, "./player.txt", "r");

	while (fgets(str, 100, file) != NULL)
	{
		i++;
	}
	fclose(file);
	fopen_s(&file, "./player.txt", "a");

	playerinfo._num = i+1;
	_itoa_s(playerinfo._num,str,10);
	strcat_s(str, space);
	strcat_s(str, playerinfo._name);
	strcat_s(str, space);
	_itoa_s(playerinfo._score, score, 10);
	strcat_s(str, score);
	strcat_s(str, enter);
	fprintf_s(file,str);
	fclose(file);
	return 0;
}
PInfo FInInfo(int key)
{
	FILE* file;
	PInfo info[100], pinfo;
	char str[100] = "";
	char num[100] = "", name[100] = "", score[100] = "";
	int space = 0, a = 0;
	fopen_s(&file, "./player.txt", "r");

	while (fgets(str, 100, file) != NULL)
	{
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] != ' ')
			{
				if (space == 0)
				{
					for (; str[i] != ' '; i++)
					{
						num[i] = str[i];
					}
					space++;
				}
				else if (space == 1)
				{
					for (int j = 0; str[i] != ' '; j++)
					{
						name[j] = str[i];
						i++;
					}
					space++;
				}
				else if (space == 2)
				{
					for (int j = 0; str[i] != '\n'; j++)
					{
						score[j] = str[i];
						i++;
					}
				}
			}
			else
			{
				space++;
			}


		}


		space = 0;
		info[a]._num = atoi(num);
		strcat_s(info[a]._name, name);
		info[a]._score = atoi(score);
		a++;

		memset(name, '\0', 100);
		memset(num, '\0', 100);
		memset(score, '\0', 100);
	}
	fclose(file);

	return info[key];
}
PScore MaxScroe()
{
	FILE* file;
	PInfo info[100], pinfo;
	char str[100] = "";
	char num[100] = "", name[100] = "", score[100] = "";
	int space = 0, a = 0;
	fopen_s(&file, "./player.txt", "r");

	while (fgets(str, 100, file) != NULL)
	{
		for (int i = 0; i < strlen(str); i++)
		{
			if (str[i] != ' ')
			{
				if (space == 0)
				{
					for (; str[i] != ' '; i++)
					{
						num[i] = str[i];
					}
					space++;
				}
				else if (space == 1)
				{
					for (int j = 0; str[i] != ' '; j++)
					{
						name[j] = str[i];
						i++;
					}
					space++;
				}
				else if (space == 2)
				{
					for (int j = 0; str[i] != '\n'; j++)
					{
						score[j] = str[i];
						i++;
					}
				}
			}
			else
			{
				space++;
			}


		}


		space = 0;
		info[a]._num = atoi(num);
		strcat_s(info[a]._name, name);
		info[a]._score = atoi(score);
		a++;

		memset(name, '\0', 100);
		memset(num, '\0', 100);
		memset(score, '\0', 100);
	}
	fclose(file);
	int max = 0;
	char n[100] = "";
	for (int i = 0; i <= a; i++)
	{
		if (info[i]._score > max)
		{
			max = info[i]._score;
			memset(n, '\0', 100);
			strcat_s(n, info[i]._name);
		}

	}
	PScore ps;
	strcat_s(ps._name,n);
	ps._score = max;
	return ps;
}
int NewFile()
{
	FILE* file;
	if (fopen_s(&file, "./player.txt", "r") != 0)
	{
		fclose(file);
		fopen_s(&file, "./player.txt", "w");
		fclose(file);
	}

	return 0;
}

int Login()
{
	cleardevice();
	BeginBatchDraw();


	char name[100]="";
	char  ch[2];
	int name_cursor_x, name_cursor_y;
	int num = 0;
	bool name_cursor = false;
	int len;
	int cursorani = 0;

	IMAGE bk,ok,bigok;
	loadimage(&bk, "./login.jpg");
	loadimage(&ok, "./ok.png",150,75);
	loadimage(&bigok, "./ok.png", 180, 90);
	while (1)
	{
		cleardevice();
		BeginBatchDraw();
		cleardevice();



		putimage(0, 0,&bk);
		putimage(160, 550, &ok);

		//line(40, 175, 453, 175);//LineEdit01
		//line(40, 175, 40, 225);




		setlinecolor(RGB(0, 0, 0));
		setlinestyle(PS_SOLID, 2);

		name_cursor_x = 55 + strlen(name) * 15;
		name_cursor_y = 185;



		cursorani += 1;

		settextcolor(RGB(0, 0, 0));
		settextstyle(30, 0, PS_SOLID);
		outtextxy(50, 185, name);

	
		if (name_cursor == true)
		{
			if (cursorani % 3600 < 1800)
			    line(name_cursor_x, name_cursor_y, name_cursor_x, name_cursor_y + 30);
			ch[0] = GetInputChar();
			ch[1] = '\0';
			if (ch[0] != 0)
			{
				if (ch[0] == '\b')
				{
					len = strlen(name);
					name[len - 1] = name[len];
					name[len] = '\0';
				}
				else
					strcat_s(name, ch);
			}
		}




		ExMessage msgb;
		peekmessage(&msgb, EM_MOUSE);
	    if (msgb.x >= 40 && msgb.x <= 453 && msgb.y >= 175 && msgb.y <= 225)
		{
			if (msgb.lbutton )
			{
				name_cursor = true;
			}
		}
		else if (msgb.x >= 160 && msgb.x <= 310 && msgb.y >= 550 && msgb.y <= 625)
		{
			putimage( 150, 545 ,&bigok);
			if (msgb.lbutton)
			{
				strcat_s(playerinfo._name, name);
				msgb.lbutton = false;
				return 0;
			}
		}
		FlushBatchDraw();
	}
	cleardevice();
	FlushBatchDraw();
}



void MainMenu()
{
	IMAGE img;
	PlaystartBGM();
	int wid1, wid2, hei, x1, x2, y, xt, yt, widt, heit;
	int logined = 0;
	PScore ps = MaxScroe();
	char score[100] = "";

	while (flag)
	{
		cleardevice();
		BeginBatchDraw();

		loadimage(&img, "./bkpic.jpg");//背景图片
        putimage(0, 0, &img);
		settextcolor(RGB(240, 84, 27));
		settextstyle(40, 0, PS_SOLID); 
		_itoa_s(ps._score, score, 10);
		outtextxy(250, 10, score);
		outtextxy(250, 50, ps._name);

		if (logined == 1)
    	{
			//----------------------开始按钮------------------------//
			setlinecolor(RGB(106, 26, 19));//按钮样式
			setlinestyle(PS_SOLID, 5);
			
			setfillcolor(RGB(252, 128, 57));//按钮边框填充样式
			fillroundrect(140, 280, 340, 340, 10, 10);//按钮绘制

			settextstyle(25, 0, "黑体");//字体绘制
			widt = textwidth("开始摇滚吧！");
			xt = 140 + (200 - widt) / 2;
			heit = textheight("开始摇滚吧！");
			yt = 280 + (60 - heit) / 2;
			setbkmode(TRANSPARENT);
			settextcolor(RGB(220, 220, 2));
			outtextxy(xt + 10, yt, "开始摇滚吧！");

			//-----------------------设置按钮------------------------//
			setfillcolor(RGB(252, 128, 57));//按钮边框填充样式
			fillroundrect(140, 380, 340, 440, 10, 10);//按钮绘制

			settextstyle(25, 0, "黑体");//字体绘制
			widt = textwidth("设置");
			xt = 140 + (200 - widt) / 2;
			heit = textheight("设置");
			yt = 380 + (60 - heit) / 2;
			setbkmode(TRANSPARENT);
			settextcolor(RGB(220, 220, 2));
			outtextxy(xt , yt, "设置");


			//-----------------------退出按钮-------------------------//
			setfillcolor(RGB(252, 128, 57));//按钮边框填充样式
			fillroundrect(140, 480, 340, 540, 10, 10);//按钮绘制

			settextstyle(25, 0, "黑体");//字体绘制
			widt = textwidth("退出");
			xt = 140 + (200 - widt) / 2;
			heit = textheight("退出");
			yt = 480 + (60 - heit) / 2;
			setbkmode(TRANSPARENT);
			settextcolor(RGB(220, 220, 2));
			outtextxy(xt , yt, "退出");
			settextstyle(40, 0, PS_SOLID);
			outtextxy(100, 220, "Hello!");
			outtextxy(250, 220, playerinfo._name);
			

			//-------------------------------按键触发---------------------------------//
			ExMessage msgsb;//startbutton

			if (1)
			{
				peekmessage(&msgsb, EM_MOUSE);
				if (msgsb.x >= 140 && msgsb.x <= 340 && msgsb.y >= 280 && msgsb.y <= 340)
				{
					setlinecolor(RGB(106, 26, 19));//开始按钮
					setlinestyle(PS_SOLID, 5);
					setfillcolor(RGB(252, 128, 57));
					fillroundrect(130, 270, 350, 350, 10, 10);
					settextstyle(30, 0, "黑体");
					widt = textwidth("开始摇滚吧！");
					xt = 140 + (200 - widt) / 2;
					heit = textheight("开始摇滚吧！");
					yt = 280 + (60 - heit) / 2;
					setbkmode(TRANSPARENT);
					settextcolor(RGB(220, 220, 2));
					outtextxy(xt + 10, yt, "开始摇滚吧！");

					if (msgsb.lbutton)
					{
						Guide();
						cleardevice();
						StopstartBGM();
						flag = 0;
					}
				}
				else if (msgsb.x >= 140 && msgsb.x <= 340 && msgsb.y >= 380 && msgsb.y <= 440)
				{
					setlinecolor(RGB(106, 26, 19));//设置按钮
					setlinestyle(PS_SOLID, 5);
					setfillcolor(RGB(252, 128, 57));
					fillroundrect(130, 370, 350, 450, 10, 10);
					settextstyle(30, 0, "黑体");
					widt = textwidth("设置");
					xt = 140 + (200 - widt) / 2;
					heit = textheight("设置");
					yt = 380 + (60 - heit) / 2;
					setbkmode(TRANSPARENT);
					settextcolor(RGB(220, 220, 2));
					outtextxy(xt , yt, "设置");

					if (msgsb.lbutton )
					{
						
						Options();
						msgsb.lbutton = false;//清除冗余鼠标信息
					}
				}
				else if (msgsb.x >= 140 && msgsb.x <= 340 && msgsb.y >= 480 && msgsb.y <= 540)
				{
					setlinecolor(RGB(106, 26, 19));//退出按钮
					setlinestyle(PS_SOLID, 5);
					setfillcolor(RGB(252, 128, 57));
					fillroundrect(130, 470, 350, 550, 10, 10);
					settextstyle(30, 0, "黑体");
					widt = textwidth("退出");
					xt = 140 + (200 - widt) / 2;
					heit = textheight("退出");
					yt = 480 + (60 - heit) / 2;
					setbkmode(TRANSPARENT);
					settextcolor(RGB(220, 220, 2));
					outtextxy(xt, yt, "退出");

					if (msgsb.lbutton)
					{
						exit(0);
					}
				}
			}
		}
		else
		{
			//----------------------登录按钮------------------------//
			setlinecolor(RGB(106, 26, 19));//按钮字体样式
			setlinestyle(PS_SOLID, 5);
			
			setfillcolor(RGB(252, 128, 57));//按钮边框填充样式
			fillroundrect(140, 280, 340, 340, 10, 10);//按钮绘制

			settextstyle(25, 0, "黑体");//字体绘制
			widt = textwidth("登录");
			xt = 140 + (200 - widt) / 2;
			heit = textheight("登录");
			yt = 280 + (60 - heit) / 2;
			setbkmode(TRANSPARENT);
			settextcolor(RGB(220, 220, 2));
			outtextxy(xt , yt, "登录");




			ExMessage msgsb;//startbutton

			if (1)
			{
				peekmessage(&msgsb, EM_MOUSE);
				if (msgsb.x >= 140 && msgsb.x <= 340 && msgsb.y >= 280 && msgsb.y <= 340)
				{
					setlinecolor(RGB(106, 26, 19));//开始按钮
					setlinestyle(PS_SOLID, 5);
					setfillcolor(RGB(252, 128, 57));
					fillroundrect(130, 270, 350, 350, 10, 10);
					settextstyle(30, 0, "黑体");
					widt = textwidth("登录");
					xt = 140 + (200 - widt) / 2;
					heit = textheight("登录");
					yt = 280 + (60 - heit) / 2;
					setbkmode(TRANSPARENT);
					settextcolor(RGB(220, 220, 2));
					outtextxy(xt , yt, "登录");

					if (msgsb.lbutton)
					{
						NameNum = Login();
						FInInfo(NameNum);
						logined = 1;

						msgsb.lbutton = false;
						
						Sleep(100);
					}
				}
			

			}
		}




		

		FlushBatchDraw();
	}

}

int checkwin();
void GhostsMove(ghost &g);
void GhostsUpdate(int difficulty);
int ManUpdate();
void BugReset();
int Options()
{
	cleardevice();
	
	BeginBatchDraw();
	IMAGE img, uper_button, lower_button,bu,bl,back,bigback;

	int i = 0,dif_Upbd = 0, dif_Lwbd = 0,vol_Upbd = 0,vol_Lwbd = 0;

	ExMessage msgb;//startbutton
	while (1)
	{
		cleardevice();
		BeginBatchDraw();

		loadimage(&img, "./options.jpg");//背景图片
		putimage(0, 0, &img);

		loadimage(&uper_button, "./uper.png");//背景图片
		putimage(330, 120, &uper_button);
		putimage(330, 220, &uper_button);

		loadimage(&lower_button, "./lower.png");//背景图片
		putimage(400, 120, &lower_button);
		putimage(400, 220, &lower_button);

		loadimage(&back, "./back.png",150,75);
		putimage(170, 500, &back);

		settextcolor(RGB(255, 111, 40));
		settextstyle(40, 0, "黑体");
		outtextxy(70, 130, "难度：");

		settextcolor(RGB(255, 111, 40));
		settextstyle(40, 0, "黑体");
		outtextxy(70, 230, "音量：");

		char vol[10];		
		_itoa_s(volume, vol, 10);


		settextstyle(40, 0, "黑体");


		if (difficulty == 0)
		{
			settextcolor(GREEN);
			outtextxy(180, 130, "Peaceful");
		}
		else if (difficulty == 1)
		{
			settextcolor(RGB(180, 255, 0));
			outtextxy(180, 130, "Easy");
		}
		else if (difficulty == 2)
		{
			settextcolor(YELLOW);
			outtextxy(180, 130, "Normal");
		}
		else if (difficulty == 3)
		{
			settextcolor(RGB(255, 160, 0));
			outtextxy(180, 130, "Hard");
		}
		else
		{
			settextcolor(RED);
			outtextxy(180, 130, "Hell");
		}

		settextcolor(GREEN);
		outtextxy(180, 230, (LPCSTR)vol);
		//line(330, 120, 380, 170);

		loadimage(&bu, "./uper.png", 60, 60);
		loadimage(&bl, "./lower.png", 60, 60);
		loadimage(&bigback, "./back.png", 170, 85);

		peekmessage(&msgb, EM_MOUSE);
		if (msgb.x >= 330 && msgb.x <= 380 && msgb.y >= 120 && msgb.y <= 170)
		{
			putimage(325, 115, &bu);

			if (msgb.lbutton)
			{
				dif_Upbd = 1;
			}
			if (msgb.message == WM_LBUTTONUP && dif_Upbd == 1)
			{
				dif_Upbd = 2;
			}
			if (difficulty != HELL && dif_Upbd == 2)
			{
				difficulty++;
				dif_Upbd = 0;
			}

		}
		else
			dif_Upbd = 0;
		if (msgb.x >= 400 && msgb.x <= 450 && msgb.y >= 120 && msgb.y <= 170)
		{
			putimage(395, 115, &bl);

			if (msgb.lbutton)
			{
				dif_Lwbd = 1;
			}
			if (msgb.message == WM_LBUTTONUP && dif_Lwbd == 1)
			{
				dif_Lwbd = 2;
			}
			if (difficulty != PEACEFUL && dif_Lwbd == 2)
			{
				difficulty--;
				dif_Lwbd = 0;
			}
		}
		else
			dif_Lwbd = 0;
		if (msgb.x >= 330 && msgb.x <= 380 && msgb.y >= 220 && msgb.y <= 270)
		{
			putimage(325, 215, &bu);
			if (msgb.lbutton)
			{
				vol_Upbd = 1;
			}
			if (msgb.message == WM_LBUTTONUP && vol_Upbd == 1)
			{
				vol_Upbd = 2;
			}
			if (volume <=290 && vol_Upbd == 2)
			{
				volume += 10;
				vol_Upbd = 0;
				SetVolume(1);
			}
		}
		else
			vol_Upbd = 0;
		if (msgb.x >= 400 && msgb.x <= 450 && msgb.y >= 220 && msgb.y <= 270)
		{
			putimage(395, 215, &bl);
			if (msgb.lbutton)
			{
				vol_Lwbd = 1;
			}
			if (msgb.message == WM_LBUTTONUP && vol_Lwbd == 1)
			{
				vol_Lwbd = 2;
			}
			if (volume >= 10 && vol_Lwbd == 2)
			{
				volume -= 10;
				vol_Lwbd = 0;
				SetVolume(1);
			}

		}
		else
			vol_Lwbd = 0;
		
		if (msgb.x >= 170 && msgb.x <= 320 && msgb.y >= 500 && msgb.y <= 575)
		{
			

			putimage(160, 495, &bigback);
			if (msgb.lbutton)
			{
				return 0;
			}
		
		}
		

		FlushBatchDraw();

		
	}
	FlushBatchDraw();
}


int Gaming()
{
	PlaygameBGM();
	int i;//循环计数
	int cp;//checkpath
	int junctiontype, junctionnumber;
	int checkhealth = -1;
	initghosts();
	initjunctionpostion();
	initfood();
	initplayer();



	for (i = 0; 1; i++)
	{

		BeginBatchDraw();
		cleardevice();
		int cw = checkwin();

		if (cw == 1 )
		{
			win = 1;
			return 0;
		}

		IMAGE imgmap;
		loadimage(&imgmap, "./map.jpg", 0, 0);
		putimage(35, 80, &imgmap);//地图及UI绘制
		settextcolor(RED);
		settextstyle(40, 20, "黑体");
		outtextxy(60, 35, "Score:");
		outtextxy(310, 35, "x");
		setfillcolor(RED);
		solidpie(270, 40, 300, 70, 0.3, -0.3);
		setfillcolor(RGB(254, 242, 0));
		switch (player.health)
		{
		case 1:solidcircle(350, 57, 10); break;
		case 2:solidcircle(350, 57, 10); solidcircle(380, 57, 10); break;
		case 3:solidcircle(350, 57, 10); solidcircle(380, 57, 10); solidcircle(410, 57, 10); break;
		}
		settextcolor(GREEN);
		settextstyle(30, 15, "黑体");
		char str[100];
		sprintf_s(str, "%d", player.score);
		outtextxy(180, 42, str);


		
		//鬼鬼更新
		GhostsUpdate(difficulty);
		GhostsEatMan();
		ManEatGhosts();

		//吃豆人更新
		checkhealth = ManUpdate();
		if (checkhealth == 0)
		{
			win = 0;
			return 0;	
		}

	

		for (int i = 0; i < 300; i++)//展示食物
		{
			if (Food[i].sort != empty)
			{
				if (Food[i].sort == bean)
				{
					setfillcolor(RGB(254, 242, 0));
					solidrectangle(Food[i].x - 1, Food[i].y - 1, Food[i].x + 1, Food[i].y + 1);
				}
				else if (Food[i].sort == cherry)
				{
					IMAGE imgcherry;
					loadimage(&imgcherry, "./cherry.jpg", 18, 18, true);
					putimage(Food[i].x - 5, Food[i].y - 7, &imgcherry);

				}
				else if (Food[i].sort == big_bean)
				{
					IMAGE imgbigbean;
					loadimage(&imgbigbean, "./big_bean.jpg", 13, 13, true);
					putimage(Food[i].x - 5, Food[i].y - 5, &imgbigbean);
				}
			}
		}


		//显示鬼鬼

		IMAGE imgghostr, imgghosty, imgghostp, imgghostb;
		if (ghostr.state != prey)
		{
			if (ghostr.face == u)//red
				loadimage(&imgghostr, "./redghost_up.jpg", 20, 20, true);
			else if (ghostr.face == d)
				loadimage(&imgghostr, "./redghost_down.jpg", 20, 20, true);
			else if (ghostr.face == r)
				loadimage(&imgghostr, "./redghost_right.jpg", 20, 20, true);
			else if (ghostr.face == l)
				loadimage(&imgghostr, "./redghost_left.jpg", 20, 20, true);
		}
		else
			loadimage(&imgghostr, "./preyghost.jpg", 20, 20, true);


		if (ghostb.state != prey)
		{
			if (ghostb.face == u)//blue
				loadimage(&imgghostb, "./blueghost_up.jpg", 20, 20, true);
			else if (ghostb.face == d)
				loadimage(&imgghostb, "./blueghost_down.jpg", 20, 20, true);
			else if (ghostb.face == r)
				loadimage(&imgghostb, "./blueghost_right.jpg", 20, 20, true);
			else if (ghostb.face == l)
				loadimage(&imgghostb, "./blueghost_left.jpg", 20, 20, true);
		}
		else
			loadimage(&imgghostb, "./preyghost.jpg", 20, 20, true);

		if (ghosty.state != prey)
		{
			if (ghosty.face == u)//yellow
				loadimage(&imgghosty, "./yellowghost_up.jpg", 20, 20, true);
			else if (ghosty.face == d)
				loadimage(&imgghosty, "./yellowghost_down.jpg", 20, 20, true);
			else if (ghosty.face == r)
				loadimage(&imgghosty, "./yellowghost_right.jpg", 20, 20, true);
			else if (ghosty.face == l)
				loadimage(&imgghosty, "./yellowghost_left.jpg", 20, 20, true);

		}
		else
			loadimage(&imgghosty, "./preyghost.jpg", 20, 20, true);

		if (ghostp.state != prey)
		{
			if (ghostp.face == u)//pink
				loadimage(&imgghostp, "./pinkghost_up.jpg", 20, 20, true);
			else if (ghostp.face == d)
				loadimage(&imgghostp, "./pinkghost_down.jpg", 20, 20, true);
			else if (ghostp.face == r)
				loadimage(&imgghostp, "./pinkghost_right.jpg", 20, 20, true);
			else if (ghostp.face == l)
				loadimage(&imgghostp, "./pinkghost_left.jpg", 20, 20, true);
		}
		else
			loadimage(&imgghostp, "./preyghost.jpg", 20, 20, true);




		if (ghostb.x == ghostb.orix && ghostb.y == ghostb.oriy)
			putimage(ghostb.x, ghostb.y ,&imgghostb);
		else
		    putimage(ghostb.x - 10, ghostb.y - 10, &imgghostb);


		if (ghostr.x == ghostr.orix && ghostr.y == ghostr.oriy)
		    putimage(ghostr.x, ghostr.y, &imgghostr);
		else
			putimage(ghostr.x - 10, ghostr.y - 10, &imgghostr);


		if (ghosty.x == ghosty.orix && ghosty.y == ghosty.oriy)
		    putimage(ghosty.x, ghosty.y, &imgghosty);
		else
			putimage(ghosty.x - 10, ghosty.y - 10, &imgghosty);


		if (ghostp.x == ghostp.orix && ghostp.y == ghostp.oriy)
		    putimage(ghostp.x, ghostp.y, &imgghostp);
		else
			putimage(ghostp.x - 10, ghostp.y - 10, &imgghostp);

		//绘制吃豆人
		IMAGE pac1l, pac1r, pac1d, pac1u, pac2;
		loadimage(&pac1l, "./pacman1l.jpg", 19, 19);
		loadimage(&pac1r, "./pacman1r.jpg", 19, 19);
		loadimage(&pac1u, "./pacman1u.jpg", 19, 19);
		loadimage(&pac1d, "./pacman1d.jpg", 19, 19);
		loadimage(&pac2, "./pacman2.jpg", 19, 19);
		if (i % 150 <= 75)
		{
			switch (player.face)
			{
			case u:putimage(player.pos[0] - 5, player.pos[1] - 10, &pac1u); break;
			case d:putimage(player.pos[0] - 5, player.pos[1] - 10, &pac1d); break;
			case l:putimage(player.pos[0] - 5, player.pos[1] - 10, &pac1l); break;
			case r:putimage(player.pos[0] - 5, player.pos[1] - 10, &pac1r); break;
			}
		}
		else
		{
			putimage(player.pos[0] - 5, player.pos[1] - 10, &pac2);
		}




		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		junctiontype = checkjunctionarea(player.pos[0],player.pos[1]).type;
		junctionnumber = checkjunctionarea(player.pos[0], player.pos[1]).junction;

 
		BugReset();

		if (GetAsyncKeyState(VK_UP))
		{
			if (lockcnt >= 1)
				lockcnt++;//cnt==1代表已拉回，说明该键按了一次，而该键再次被触发，所以cnt为++


			cp = checkpath(player.pos[0], player.pos[1]);
			if (junctiontype != all)
			{
				switch (cp)
				{
				case ud: player.pos[1] -= player.speed; unlock(); break;
				case lr:break;
				case udl:player.pos[1] -= player.speed; unlock(); break;
				case udr:player.pos[1] -= player.speed;  unlock(); break;
				case lru:player.pos[1] -= player.speed;  unlock(); break;
				case lrd:break;
				case dl:break;
				case dr:break;
				case ul:player.pos[1] -= player.speed;  unlock(); break;
				case ur:player.pos[1] -= player.speed;  unlock(); break;
				case all:player.pos[1] -= player.speed;  unlock(); break;
				case bug:player.pos[0] = 235;
					player.pos[1] = 375; break;
				}
				if (junctiontype == lru && selflock == 0)
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y;
					selflock = 1;
					lockcnt = 1;
				}
			}
			else if (junctiontype == all)//四向点专属移动系统
			{
				player.pos[1] -= player.speed;
				if (player.pos[0] >= junctionpos[junctionnumber].x - bb && player.pos[0] < junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y - bb)//检测是否达到上边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y - bb - 1;//传送到该方向的线路上
				}
				if (player.pos[0] > junctionpos[junctionnumber].x - bb && player.pos[0] <= junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y + bb)//检测是否达到下边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y + bb + 1;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x - bb && player.pos[1] > junctionpos[junctionnumber].y - bb && player.pos[1] <= junctionpos[junctionnumber].y - bb)//检测是否达到左边界
				{
					player.pos[0] = junctionpos[junctionnumber].x - bb - 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x + bb && player.pos[1] >= junctionpos[junctionnumber].y - bb && player.pos[1] < junctionpos[junctionnumber].y - bb)//检测是否达到右边界
				{
					player.pos[0] = junctionpos[junctionnumber].x + bb + 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
			}
			player.face = u;

		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			if (lockcnt >= 1)
				lockcnt++;//cnt==1代表已拉回，说明该键按了一次，而该键再次被触发，所以cnt为++

			cp = checkpath(player.pos[0], player.pos[1]);
			if (junctiontype != all)
			{
				switch (cp)
				{
				case ud:player.pos[1] += player.speed;  unlock(); break;
				case lr:break;
				case udl:player.pos[1] += player.speed;  unlock(); break;
				case udr:player.pos[1] += player.speed; unlock(); break;
				case lru:break;
				case lrd:player.pos[1] += player.speed;  unlock(); break;
				case dl:player.pos[1] += player.speed;  unlock(); break;
				case dr:player.pos[1] += player.speed; unlock(); break;
				case ul:break;
				case ur:break;
				case all:player.pos[1] += player.speed;  unlock(); break;
				case bug:player.pos[0] = 235;
					player.pos[1] = 375; break;
				}
				if (junctiontype == lrd && selflock == 0)
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y;
					selflock = 1;
					lockcnt = 1;
				}
			}
			else if (junctiontype == all)//四向点专属移动系统
			{
				player.pos[1] += player.speed;
				if (player.pos[0] >= junctionpos[junctionnumber].x - bb && player.pos[0] < junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y - bb)//检测是否达到上边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y - bb - 1;//传送到该方向的线路上
				}
				if (player.pos[0] > junctionpos[junctionnumber].x - bb && player.pos[0] <= junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y + bb)//检测是否达到下边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y + bb + 1;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x - bb && player.pos[1] > junctionpos[junctionnumber].y - bb && player.pos[1] <= junctionpos[junctionnumber].y - bb)//检测是否达到左边界
				{
					player.pos[0] = junctionpos[junctionnumber].x - bb - 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x + bb && player.pos[1] >= junctionpos[junctionnumber].y - bb && player.pos[1] < junctionpos[junctionnumber].y - bb)//检测是否达到右边界
				{
					player.pos[0] = junctionpos[junctionnumber].x + bb + 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
			}

			player.face = d;

		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			if (lockcnt >= 1)
				lockcnt++;//cnt==1代表已拉回，说明该键按了一次，而该键再次被触发，所以cnt为++

			cp = checkpath(player.pos[0], player.pos[1]);
			if (junctiontype != all)
			{
				switch (cp)
				{
				case ud:break;
				case lr:player.pos[0] -= player.speed; unlock(); break;
				case udl:player.pos[0] -= player.speed;  unlock(); break;
				case udr:break;
				case lru:player.pos[0] -= player.speed;  unlock();; break;
				case lrd:player.pos[0] -= player.speed; unlock(); break;
				case dl:player.pos[0] -= player.speed;  unlock(); break;
				case dr:break;
				case ul:player.pos[0] -= player.speed;  unlock(); break;
				case ur:break;
				case all:player.pos[0] -= player.speed;  unlock(); break;
				case bug:player.pos[0] = 235;
					player.pos[1] = 375; break;
				}
				if (junctiontype == udl && selflock == 0)
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y;
					selflock = 1;
					lockcnt = 1;
				}
			}
			else if (junctiontype == all)//四向点专属移动系统
			{
				player.pos[0] -= player.speed;
				if (player.pos[0] >= junctionpos[junctionnumber].x - bb && player.pos[0] < junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y - bb)//检测是否达到上边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y - bb - 1;//传送到该方向的线路上
				}
				if (player.pos[0] > junctionpos[junctionnumber].x - bb && player.pos[0] <= junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y + bb)//检测是否达到下边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y + bb + 1;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x - bb && player.pos[1] > junctionpos[junctionnumber].y - bb && player.pos[1] <= junctionpos[junctionnumber].y - bb)//检测是否达到左边界
				{
					player.pos[0] = junctionpos[junctionnumber].x - bb - 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x + bb && player.pos[1] >= junctionpos[junctionnumber].y - bb && player.pos[1] < junctionpos[junctionnumber].y - bb)//检测是否达到右边界
				{
					player.pos[0] = junctionpos[junctionnumber].x + bb + 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
			}

			player.face = l;

		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (lockcnt >= 1)
				lockcnt++;//cnt==1代表已拉回，说明该键按了一次，而该键再次被触发，所以cnt为++

			cp = checkpath(player.pos[0], player.pos[1]);
			if (junctiontype != all)
			{
				switch (cp)
				{
				case ud:break;
				case lr:player.pos[0] += player.speed;  unlock(); break;
				case udl:break;
				case udr:player.pos[0] += player.speed;  unlock(); break;
				case lru:player.pos[0] += player.speed;  unlock(); break;
				case lrd:player.pos[0] += player.speed; unlock(); break;
				case dl:break;
				case dr:player.pos[0] += player.speed;  unlock(); break;
				case ul:break;
				case ur:player.pos[0] += player.speed;  unlock(); break;
				case all:player.pos[0] += player.speed;  unlock(); break;
				case bug:player.pos[0] = 235;
					player.pos[1] = 375; break;
				}
				if (junctiontype == udr && selflock == 0)
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y;
					selflock = 1;
					lockcnt = 1;
				}
			}
			else if (junctiontype == all)//四向点专属移动系统
			{
				player.pos[0] += player.speed;
			
				if (player.pos[0] >= junctionpos[junctionnumber].x - bb && player.pos[0] < junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y - bb)//检测是否达到上边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y - bb - 1;//传送到该方向的线路上
				}
				if (player.pos[0] > junctionpos[junctionnumber].x - bb && player.pos[0] <= junctionpos[junctionnumber].x + bb && player.pos[1] == junctionpos[junctionnumber].y + bb)//检测是否达到下边界
				{
					player.pos[0] = junctionpos[junctionnumber].x;
					player.pos[1] = junctionpos[junctionnumber].y + bb + 1;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x - bb && player.pos[1] > junctionpos[junctionnumber].y - bb && player.pos[1] <= junctionpos[junctionnumber].y - bb)//检测是否达到左边界
				{
					player.pos[0] = junctionpos[junctionnumber].x - bb - 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
				if (player.pos[0] == junctionpos[junctionnumber].x + bb && player.pos[1] >= junctionpos[junctionnumber].y - bb && player.pos[1] < junctionpos[junctionnumber].y - bb)//检测是否达到右边界
				{
					player.pos[0] = junctionpos[junctionnumber].x + bb + 1;
					player.pos[1] = junctionpos[junctionnumber].y;//传送到该方向的线路上
				}
			}

			player.face = r;


		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////

		eatfood();

		if (player.pos[0] == 60 && player.pos[1] == 340)
			player.pos[0] = 419;
		else if (player.pos[0] == 420 && player.pos[1] == 340)
			player.pos[0] = 61;


		FlushBatchDraw();




	}
}

void GhostsUpdate(int difficulty)
{
	if (ghostb.state == prey)
	{
		if (ghostb.preytime <= PREYTIME)
		    ghostb.preytime += 1;
		else
		{
			ghostb.state = hunter;
			ghostr.state = hunter;
			ghosty.state = hunter;
			ghostp.state = hunter;
			ghostb.preytime = 0;
			ghostb.preytime = 0;
			ghostb.preytime = 0;
			ghostb.preytime = 0;
		}
	}
	if (difficulty == 1)//蓝
	{
		GhostsMove(ghostb);//move
		                   //checkdead
		                   
	}
	else if (difficulty == 2)//蓝红
	{
		GhostsMove(ghostb);
		GhostsMove(ghostr);

	}
	else if (difficulty == 3)//蓝红橙
	{
		GhostsMove(ghostb);
		GhostsMove(ghostr);
		GhostsMove(ghosty);

	}
	else if (difficulty == 4)//蓝红橙粉
	{
		GhostsMove(ghostb);
		GhostsMove(ghostr);
		GhostsMove(ghosty);
		GhostsMove(ghostp);

	}
}
void GhostsMove(ghost &g)
{
	if (g.atHome == 1 && g.life == alive)//活着却在家
	{
		g.x = 235;
		g.y = 285;
		srand((unsigned)time(NULL) + g.randseed);
		int face = rand() % 2;
		if (face == 0)
			g.face = r;
		else
			g.face = l;
		g.atHome = 0;
	}
	if (g.atHome == 0 && g.life == dead)//死了却在外面
	{
		g.x = g.orix;
		g.y = g.oriy;
		g.atHome = 1;

	}
	if (g.atHome == 1 && g.life == dead)//死了老实在家
	{
		if (g.deadtime >= DEADTIME)//刑满释放
		{
			g.life = alive;
			g.x = 235;
			g.y = 285;
			g.atHome = 0;
			g.deadtime = 0;
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 2;
			if (face == 0)
				g.face = r;
			else
				g.face = l;
		}
		else//服刑
		{
			g.deadtime += 1;
		}
	
	}
	if (g.atHome == 0 && g.life == alive)//在外面活蹦乱跳
	{

		int path;
		int newposx = g.x, newposy = g.y;
		path = checkpath(g.x, g.y);

		if (path == ud && (g.face == u || g.face == d))//在道路上固定行走
		{

			if (g.face == u)
			{
				newposy -= g.speed;
			}
			else if (g.face == d)
			{
				newposy += g.speed;
			}
			if (checkpath(newposx,newposy) != NO )
			{
				g.x = newposx;
				g.y = newposy;
			}
		}
		else if (path == lr && (g.face == l || g.face == r))
		{

			if (g.face == l)
			{
				newposx -= g.speed;
			}
			else if (g.face == r)
			{
				newposx += g.speed;
			}
			if (checkpath(newposx, newposy) != NO)
			{
				g.x = newposx;
				g.y = newposy;
			}

			if (newposx == 60 && newposy == 340)
				g.x = 419;
			else if (newposx == 420 && newposy == 340)
				g.x = 61;

		}
		else if (path == udl)/////////////////////////开始进入节点
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 3;
			if (face == 0)
			{
				g.face = u;
				g.y -= g.speed;
			}
			else if (face == 1)
			{
				g.face = d;
				g.y += g.speed;
			}
			else
			{
				g.face = l;
				g.x -= g.speed;
			}
		}
		else if (path == udr)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 3;
			if (face == 0)
			{
				g.face = u;
				g.y -= g.speed;
			}
			else if (face == 1)
			{
				g.face = d;
				g.y += g.speed;
			}
			else
			{
				g.face = r;
				g.x += g.speed;
			}
		}
		else if (path == lru)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 3;
			if (face == 0)
			{
				g.face = l;
				g.x -= g.speed;
			}
			else if (face == 1)
			{
				g.face = r;
				g.x += g.speed;
			}
			else
			{
				g.face = u;
				g.y -= g.speed;
			}
		}
		else if (path == lrd)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 3;
			if (face == 0)
			{
				g.face = l;
				g.x -= g.speed;
			}
			else if (face == 1)
			{
				g.face = r;
				g.x += g.speed;
			}
			else
			{
				g.face = d;
				g.y += g.speed;
			}
		}
		else if (path == dr)
		{
		    srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 2;
			if (face == 0)
			{
				g.face = d;
				g.y += g.speed;
			}
			else if (face == 1)
			{
				g.face = r;
				g.x += g.speed;
			}
        }
		else if (path == dl)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 2;
			if (face == 0)
			{
				g.face = d;
				g.y += g.speed;
			}
			else if (face == 1)
			{
				g.face = l;
				g.x -= g.speed;
			}
        }
		else if (path == ur)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 2;
			if (face == 0)
			{
				g.face = u;
				g.y -= g.speed;
			}
			else if (face == 1)
			{
				g.face = r;
				g.x += g.speed;
			}
        }
		else if (path == ul)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 2;
			if (face == 0)
			{
				g.face = u;
				g.y -= g.speed;
			}
			else if (face == 1)
			{
				g.face = l;
				g.x -= g.speed;
			}
        }
		else if (path == all)
		{
			srand((unsigned)time(NULL) + g.randseed);
			int face = rand() % 4;
			if (face == 0)
			{
				g.face = l;
				g.x -= g.speed;
			}
			else if (face == 1)
			{
				g.face = r;
				g.x += g.speed;
			}
			else if (face == 2)
			{
				g.face = u;
				g.y -= g.speed;
			}
			else
			{
				g.face = d;
				g.y += g.speed;
			}
        }

	}


}
int ManUpdate()
{
	if (player.health == 0)
	{
		return 0;
	}
	return 1;
}
void GhostsEatMan() 
{
	if (ghostb.state == hunter)
	{



		if (ghostb.x >= player.pos[0] - 10 && ghostb.x <= player.pos[0] + 10 && ghostb.y >= player.pos[1] - 10 && ghostb.y <= player.pos[1] + 10)
		{
			player.health--;
			player.pos[0] = 235;
			player.pos[1] = 375;
			
		}
		if (ghostr.x >= player.pos[0] - 10 && ghostr.x <= player.pos[0] + 10 && ghostr.y >= player.pos[1] - 10 && ghostr.y <= player.pos[1] + 10)
		{
			player.health--;
			player.pos[0] = 235;
			player.pos[1] = 375;

		}
		if (ghosty.x >= player.pos[0] - 10 && ghosty.x <= player.pos[0] + 10 && ghosty.y >= player.pos[1] - 10 && ghosty.y <= player.pos[1] + 10)
		{
			player.health--;
			player.pos[0] = 235;
			player.pos[1] = 375;

		}
		if (ghostp.x >= player.pos[0] - 10 && ghostp.x <= player.pos[0] + 10 && ghostp.y >= player.pos[1] - 10 && ghostp.y <= player.pos[1] + 10)
		{
			player.health--;
			player.pos[0] = 235;
			player.pos[1] = 375;

		}
	}
}
void ManEatGhosts()
{
	if (ghostb.state == prey)
	{
		if (ghostb.x >= player.pos[0] - 10 && ghostb.x <= player.pos[0] + 10 && ghostb.y >= player.pos[1] - 10 && ghostb.y <= player.pos[1] + 10)
		{
			ghostb.x = ghostb.orix;
			ghostb.y = ghostb.oriy;
			ghostb.life = dead;
			ghostb.atHome = 1;
		}
		if (ghostr.x >= player.pos[0] - 10 && ghostr.x <= player.pos[0] + 10 && ghostr.y >= player.pos[1] - 10 && ghostr.y <= player.pos[1] + 10)
		{
			ghostr.x = ghostr.orix;
			ghostr.y = ghostr.oriy;
			ghostr.life = dead;
			ghostr.atHome = 1;
		}
		if (ghosty.x >= player.pos[0] - 10 && ghosty.x <= player.pos[0] + 10 && ghosty.y >= player.pos[1] - 10 && ghosty.y <= player.pos[1] + 10)
		{
			ghosty.x = ghosty.orix;
			ghosty.y = ghosty.oriy;
			ghosty.life = dead;
			ghosty.atHome = 1;
		}
		if (ghostp.x >= player.pos[0] - 10 && ghostp.x <= player.pos[0] + 10 && ghostp.y >= player.pos[1] - 10 && ghostp.y <= player.pos[1] + 10)
		{
			ghostp.x = ghostp.orix;
			ghostp.y = ghostp.oriy;
			ghostp.life = dead;
			ghostp.atHome = 1;
		}
	}
}
void BugReset()
{
	settextcolor(GREEN);
	settextstyle(30, 15, "黑体");
	outtextxy(107, 590, "按下R键进行BUG重置");
	int key = GetKeyState(0x52);
	if (key)
	{

		if (checkpath(player.pos[0], player.pos[1]) == NO)
		{
			player.pos[0] = 235;
			player.pos[1] = 375;
		}
	}
}



int Guide()
{
	cleardevice();
	BeginBatchDraw();
	IMAGE img;


	ExMessage msgb;//startbutton
	while (1)
	{
		BeginBatchDraw();
		loadimage(&img, "./guide.png",480,680);
		putimage(5, 0, &img);
		EndBatchDraw();
		if (GetAsyncKeyState(0x52))
			return 0;
	}

}
int End()
{
	StopgameBGM();
	playerinfo._score = player.score;
	FOutInfo();
	if (win == 1)/////////////////win
	{
		IMAGE bk,yes,no,by,bn;
		int x, y;
		char score[100] = "";
		cleardevice();
		BeginBatchDraw();
		loadimage(&bk, "./win.jpg");
		loadimage(&yes, "./yes.png", 150, 75);
		loadimage(&no, "./no.png", 150, 75);
		loadimage(&by, "./yes.png", 170, 85);
		loadimage(&bn, "./no.png", 170, 85);
		while (1)
		{
			cleardevice();
			BeginBatchDraw();
			
			putimage(0, 0, &bk);
			putimage(70, 500, &yes);
			putimage(270, 500, &no);


			settextcolor(GREEN);
			settextstyle(90, 0, PS_COSMETIC);
			_itoa_s(player.score, score, 10);
			y = 220;
			if (player.score - 1000 >= 0)
			{
				x = 145;
			}
			else if (player.score - 100 >= 0 && player.score - 1000 < 0)
			{
				x = 170;
			}
			else if (player.score - 10 >= 0 && player.score - 100 < 0)
			{
				x = 190;
			}
			else
			{
				x = 220;
			}
			outtextxy(x, y, score);
			
			settextstyle(70, 0, PS_COSMETIC);
			settextcolor(RGB(33,230,115));
			outtextxy(50, 110, playerinfo._name);

			ExMessage msgb;
			peekmessage(&msgb, EM_MOUSE);
			if (msgb.x >= 70 && msgb.x <= 220 && msgb.y >= 500 && msgb.y <= 575)
			{
				putimage(65, 497, &by);
				if (msgb.lbutton)
				{
					return 1;
					msgb.lbutton = false;
				}
			}
			else if (msgb.x >= 270 && msgb.x <= 420 && msgb.y >= 500 && msgb.y <= 575)
			{
				putimage(265, 497, &bn);
				if (msgb.lbutton)
				{
					return 0;
					msgb.lbutton = false;
				}
			}
		
			FlushBatchDraw();
		}
		FlushBatchDraw();
		return 1;
	}
	else if (win == 0)////////////lost
	{
		IMAGE bk,yes,no,by,bn;
		int x, y;
		char score[100] = "";
		loadimage(&bk, "./lost.jpg",497,705);
		cleardevice();
		loadimage(&yes, "./yes.png",150,75);
		loadimage(&no, "./no.png", 150, 75);
		loadimage(&by, "./yes.png", 170, 85);
		loadimage(&bn , "./no.png", 170, 85);
		while (1)
		{
			cleardevice();
			BeginBatchDraw();

			putimage(0, 0, &bk);			
			putimage(70, 500, &yes);
			putimage(270, 500, &no);
			
			settextcolor(RED);
			settextstyle(90,0,PS_COSMETIC);
			_itoa_s( player.score,score, 10);
			y = 220;
			if (player.score - 1000 >= 0)
			{
				x = 145;
			}
			else if (player.score - 100 >= 0 && player.score - 1000 < 0)
			{
				x = 170;
			}
			else if (player.score - 10 >= 0 && player.score - 100 < 0)
			{
				x = 190;
			}
			else
			{
				x = 220;
			}

			outtextxy(x, y, score);
			settextstyle(70, 0, PS_COSMETIC);
			settextcolor(RGB(240,84,27));
			outtextxy(50, 120, playerinfo._name);


			ExMessage msgb;
			peekmessage(&msgb, EM_MOUSE);
			if (msgb.x >= 70 && msgb.x <= 220 && msgb.y >= 500 && msgb.y <= 575)
			{
				putimage(65, 497,& by);
				if (msgb.lbutton)
				{
					return 1;
					msgb.lbutton = false;
				}
			}
			else if (msgb.x >= 270 && msgb.x <= 420 && msgb.y >= 500 && msgb.y <= 575)
			{
				putimage(265, 497, &bn);
				if (msgb.lbutton)
				{
					return 0;
					msgb.lbutton = false;
				}
			}

			FlushBatchDraw();
		}
	
		return 1;
	}
}

int checkwin()
{
	int cnt = 0, flag = 0;
	HWND hwndwin = GetHWnd();
	for (int i = 0; i <= 271; i++)
	{
		if (Food[i].sort == empty)
			cnt++;
	}
	if (cnt >= 270)
	{
		flag = 1;
	}
	if (flag == 1)
	{
		return 1;
	}
	else
		return 0;
	//if (GetAsyncKeyState(VK_CAPITAL))
	//    return 1;
}
int main()
{
	initgraph(480, 640);
	title();
	NewFile();
    MainMenu();
	cleardevice();

	int flag = 1;
	while (flag == 1)
	{
		
		Gaming();
		flag = End();
	}
	cleardevice();

	closegraph();
	return 0;
}
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <EasyXPng.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#define Pi 3.1415926 //定义Pi的值
#define WIDTH 1280 // 画面宽度
#define HEIGHT 720 // 画面高度
IMAGE im_box[15];//定义盒子图片
int id = 0;

struct Player //定义坐标结构体记录坐标
{
	double x;//横坐标
	double y;//纵坐标
};

struct Box //定义结构体记录盒子相关信息
{
	double c_x; //记录盒子表面中心y坐标
	double c_y; //记录盒子表面中心y坐标
	int dir; //记录下一个盒子出现的方向,定义方向（0为左，1为右）
	IMAGE im_box; //记录盒子图片
};

struct Player player = { WIDTH / 3.0, HEIGHT * 2.0 / 3 }; //结构体记录玩家位置
struct Box box[20] = { {WIDTH / 3.0,HEIGHT * 2.0 / 3,1} }; //结构体数组记录盒子相关信息

void Start()
{
	//初始化窗口
	initgraph(WIDTH, HEIGHT);//开窗
	setbkcolor(LIGHTGRAY);//设置背景颜色
	setfillcolor(BLACK);//设置玩家颜色
	setlinecolor(BLACK);//设置玩家小球边框颜色
	cleardevice();//清屏

	loadimage(&im_box[0], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[1], _T("./res/box/box1.png"));//载入盒子1图片
	loadimage(&im_box[2], _T("./res/box/box2.png"));//载入盒子0图片
	loadimage(&im_box[3], _T("./res/box/box3.png"));//载入盒子0图片
	loadimage(&im_box[4], _T("./res/box/box4.png"));//载入盒子0图片
	loadimage(&im_box[5], _T("./res/box/box5.png"));//载入盒子0图片
	loadimage(&im_box[6], _T("./res/box/box6.png"));//载入盒子0图片
	loadimage(&im_box[7], _T("./res/box/box7.png"));//载入盒子0图片
	loadimage(&im_box[8], _T("./res/box/box8.png"));//载入盒子0图片
	loadimage(&im_box[9], _T("./res/box/box9.png"));//载入盒子0图片
	loadimage(&im_box[10], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[11], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[12], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[13], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[14], _T("./res/box/box0.png"));//载入盒子0图片

	//初始化玩家与盒子
	int i, count = 0;//计数器
	double distance;//记录盒子y方向移动距离
	for (i = 1;i < 20;i++)
	{
		count++;//计数器+1
		if (count > 2)//当未转向的盒子数大于2时
		{
			box[i].dir = rand() % 2;//生成下一个盒子方向
			if (box[i].dir != box[i - 1].dir)//如果盒子转向
			{
				count = 0;//重新开始计数
			}
		}
		else//当未转向的盒子数小于等于2时
		{
			box[i].dir = box[i - 1].dir;//盒子方向一致
		}
		distance = 151.5 / sqrt(3) + rand() % 150 + 20; //随机生成相邻盒子之间的距离（盒子之间最小距离）
		if (box[i - 1].dir == 1) //如果本盒子在前一个盒子右侧
		{
			box[i].c_x = box[i - 1].c_x + sqrt(3) * distance;
			box[i].c_y = box[i - 1].c_y - distance;
		}
		else if (box[i - 1].dir == 0)//如果本盒子在前一个盒子左侧
		{
			box[i].c_x = box[i - 1].c_x - sqrt(3) * distance;
			box[i].c_y = box[i - 1].c_y - distance;
		}
	}
	for (i = 19;i >= 0;i--)//倒序生成盒子（营造覆盖效果）
	{
		box[i].im_box = im_box[rand() % 7];//随机获取盒子图片
		putimagePng(box[i].c_x - 151.5, box[i].c_y - 90.3, &box[i].im_box);//加载盒子图片
	}
	fillcircle(player.x, player.y, 10);//绘制玩家
}

void PlayerMove()
{
	//获取鼠标按下时长
	ExMessage m;//记录鼠标动作
	clock_t start_t, stop_t;//定义结构体获取鼠标按下与松开时间
	double t;
	while (1)
	{
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN) //如果鼠标左键按下
		{
			start_t = clock(); // 获取鼠标按下时间 
		}
		if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
		{
			stop_t = clock(); //获取鼠标释放时间
			break; //跳出循环
		}
	}
	t = (double)(stop_t - start_t);//计算鼠标按下时长

	int i;
	BeginBatchDraw();
	for (i = 0;i < 50;i++)
	{
		cleardevice();
		if (box[id].dir == 1)
		{
			player.x += sqrt(3) * t / 250;
			player.y -= t / 250;
		}
		else if (box[id].dir == 0)
		{
			player.x -= sqrt(3) * t / 250;
			player.y -= t / 250;
		}
		//绘制静止盒子
		int j;
		for (j = 19;j >= 0;j--)
		{
			putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &box[j].im_box);
		}
		fillcircle(player.x, player.y, 10);
		FlushBatchDraw(); //批量绘制
		Sleep(10);
	}
	EndBatchDraw(); //结束批量绘图
}
int main()
{
	srand((unsigned)time(0)); //产生随机数种子
	Start();
	PlayerMove();
	_getch();
	closegraph();
	return 0;
}

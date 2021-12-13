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
int score = 0; //得分
int id = 0;

void TotalMove();

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

void PrintScore()
{
	TCHAR s[20]; // 定义字符串数组
	_stprintf_s(s, _T("%d"), score); // 将score转换为字符串
	setbkmode(TRANSPARENT);//设置字体背景为透明
	settextcolor(BLACK);//设置文字颜色为黑色
	settextstyle(50, 0, _T("04b_03b")); // 设置文字大小、字体
	outtextxy(50, 50, s); // 输出得分文字
}

void PrintFailure()
{
	
}

void Start()
{
	//初始化窗口
	initgraph(WIDTH, HEIGHT);//开窗
	setbkcolor(LIGHTGRAY);//设置背景颜色
	setfillcolor(BLACK);//设置玩家颜色
	setlinecolor(BLACK);//设置玩家小球边框颜色
	cleardevice();//清屏

	IMAGE im_play;//定义“我也要玩”按钮图片
	IMAGE im_bk;//定义背景图片
	IMAGE im_title;//定义标题图片
	loadimage(&im_play, _T("./res/play.png"));//载入开始游戏按钮图片
	loadimage(&im_bk, _T("./res/background.png"));//载入背景图片
	loadimage(&im_title, _T("./res/title.png"));//载入标题图片
	loadimage(&im_box[0], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[1], _T("./res/box/box1.png"));//载入盒子1图片
	loadimage(&im_box[2], _T("./res/box/box2.png"));//载入盒子2图片
	loadimage(&im_box[3], _T("./res/box/box3.png"));//载入盒子3图片
	loadimage(&im_box[4], _T("./res/box/box4.png"));//载入盒子4图片
	loadimage(&im_box[5], _T("./res/box/box5.png"));//载入盒子5图片
	loadimage(&im_box[6], _T("./res/box/box6.png"));//载入盒子6图片
	loadimage(&im_box[7], _T("./res/box/box7.png"));//载入盒子7图片
	loadimage(&im_box[8], _T("./res/box/box8.png"));//载入盒子8图片
	loadimage(&im_box[9], _T("./res/box/box9.png"));//载入盒子9图片
	loadimage(&im_box[10], _T("./res/box/box0.png"));//载入盒子10图片
	loadimage(&im_box[11], _T("./res/box/box0.png"));//载入盒子11图片
	loadimage(&im_box[12], _T("./res/box/box0.png"));//载入盒子12图片
	loadimage(&im_box[13], _T("./res/box/box0.png"));//载入盒子13图片
	loadimage(&im_box[14], _T("./res/box/box0.png"));//载入盒子14图片
	//putimage(0, 0, &im_bk);//加载背景图片
	putimagePng(WIDTH / 2 - 312, HEIGHT * 2 / 3, &im_play);//加载开始游戏图片
	putimagePng(WIDTH / 2 - 309, HEIGHT / 5, &im_title);//加载“我也要玩'背景图片

	//鼠标点击“开始游戏”
	ExMessage m;//记录鼠标动作
	while (1)//循环等待用户动作
	{
		if (peekmessage(&m, EM_MOUSE))//获取鼠标动作
		{
			if ((m.message == WM_LBUTTONDOWN) && (m.x >= WIDTH / 2 - 312) && (m.x <= WIDTH / 2 + 312) && (m.y >= HEIGHT * 2 / 3) && (m.y <= HEIGHT * 2 / 3 + 246))//鼠标移动到开始游戏按钮处并且点击
			{
				while (1)//等待用户抬起鼠标
				{
					m = getmessage(EM_MOUSE);//获取鼠标动作
					if (m.message == WM_LBUTTONUP)
						break;//如果鼠标抬起则跳出循环
				}
				break;//结束用户动作等待
			}
		}
	}
	cleardevice();
	
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
		if (box[i-1].dir == 1) //如果本盒子在前一个盒子右侧
		{
			box[i].c_x = box[i - 1].c_x + sqrt(3) * distance;
			box[i].c_y = box[i - 1].c_y - distance;
		}
		else if (box[i-1].dir == 0)//如果本盒子在前一个盒子左侧
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
	PrintScore();//输出分数
}

void PlayerMove()
{
	while (((player.x - box[id].c_x) * (player.x - box[id].c_x) + (player.y - box[id].c_y) * (player.y - box[id].c_y)) <= 5780 || ((player.x - box[id - 1].c_x) * (player.x - box[id - 1].c_x) + (player.y - box[id - 1].c_y) * (player.y - box[id - 1].c_y)) <= 5780)
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

		//玩家开始跳跃
		int i,j;//生成计数器
		int NeedCrossLine = 0;
		double g = 0.3;//定义重力值
		double vx = 5.5, vy;//定义x，y方向速度
		double distance = 2 * t / sqrt(27);//距离
		vy = distance / vx / 2 * g;//计算y方向出发速度
		BeginBatchDraw();
		id++;
		for (i = 0;i < distance / vx;i++)
		{
			if (i == 0 && player.y > ((player.x - box[id - 1].c_x) * (box[id].c_y - box[id - 1].c_y) / (box[id].c_x - box[id - 1].c_x) + box[id - 1].c_y))
			{
				NeedCrossLine = 1;
			}
			if (NeedCrossLine == 1 && player.y < ((player.x - box[id - 1].c_x) * (box[id].c_y - box[id - 1].c_y) / (box[id].c_x - box[id - 1].c_x) + box[id - 1].c_y))
			{
				NeedCrossLine = 0;
			}
			if (NeedCrossLine == 0 && player.y > ((player.x - box[id - 1].c_x) * (box[id].c_y - box[id - 1].c_y) / (box[id].c_x - box[id - 1].c_x) + box[id - 1].c_y))
			{
				player.y = ((player.x - box[id - 1].c_x) * (box[id].c_y - box[id - 1].c_y) / (box[id].c_x - box[id - 1].c_x) + box[id - 1].c_y);
				break;
			}
			else
			{
				if (box[id - 1].dir == 1)
				{
					player.x += vx;
					player.y -= vy;
					vy -= g;
				}
				else if (box[id - 1].dir == 0)
				{
					player.x -= vx;
					player.y -= vy;
					vy -= g;
				}
				cleardevice();

				//绘制静止盒子和玩家
				for (j = 19;j >= 0;j--)
				{
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &box[j].im_box);
				}
				fillcircle(player.x, player.y, 10);
				PrintScore();
				FlushBatchDraw(); //批量绘制
				Sleep(10);
			}
		}
		EndBatchDraw(); //结束批量绘图

		if (((player.x - box[id].c_x) * (player.x - box[id].c_x) + (player.y - box[id].c_y) * (player.y - box[id].c_y)) <= 5780)
		{
			score++;
			//场景直线移动
			BeginBatchDraw(); //开始批量绘图
			double x_distance = 0, y_distance = 0;
			if (box[id].dir == 1)
			{
				x_distance = player.x - (WIDTH / 3.0);
				y_distance = player.y - (HEIGHT * 2.0 / 3);
			}
			else if (box[id].dir == 0)
			{
				x_distance = player.x - (WIDTH * 2.0 / 3);
				y_distance = player.y - (HEIGHT * 2.0 / 3);
			}
			int n = 50; //定义帧数
			for (i = 0;i < n;i++)
			{
				cleardevice(); //清屏
				player.x -= x_distance / n; //计算每一帧玩家X移动距离
				player.y -= y_distance / n; //计算每一帧玩家Y移动距离
				for (j = 19;j >= 0;j--)
				{
					box[j].c_x -= x_distance / n; //计算每一帧玩家X移动距离
					box[j].c_y -= y_distance / n; //计算每一帧玩家Y移动距离
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &box[j].im_box);//
				}
				fillcircle(player.x, player.y, 10); //绘制玩家新位置
				PrintScore();
				FlushBatchDraw(); //批量绘制
				Sleep(20);
			}
			EndBatchDraw(); //结束批量绘图
		}
		else if (((player.x - box[id - 1].c_x) * (player.x - box[id - 1].c_x) + (player.y - box[id - 1].c_y) * (player.y - box[id - 1].c_y)) <= 5780)
		{
			continue;
		}
		else break;
	}

	//输出失败文字
	TCHAR s[] = _T("YOU FAIL");//记录失败文字
	setbkmode(TRANSPARENT);//设置字体背景为透明
	settextcolor(RED);//设置文字颜色为红色
	settextstyle(80, 80, _T("04b_03b")); // 设置文字大小、字体
	outtextxy(WIDTH / 2 - 320, HEIGHT / 2 - 40, s); // 输出得分文字
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

#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <EasyXPng.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <io.h>

#define Pi 3.1415926 //定义Pi的值
#define WIDTH 1280 // 画面宽度
#define HEIGHT 720 // 画面高度
IMAGE im_box[15];//定义盒子图片
IMAGE im_help;//定义帮助按钮图片
IMAGE im_pause;//定义帮助按钮图片
int score = 0; //记录得分
int id = 0; //记录玩家所在盒子的ID
int count = 0; //记录未转向的盒子个数
int IsContinue = 0; //记录游戏是否失败

void PrintScore(float x, float y, int fheight);
void StartGame();
void PlayerMove();
void Pause();

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
	int boxid; //记录盒子的图片ID
	//IMAGE im_box; //记录盒子图片
};

struct Player player = { WIDTH / 3.0, HEIGHT * 2.0 / 3 }; //结构体记录玩家位置
struct Box box[15] = { {WIDTH / 3.0,HEIGHT * 2.0 / 3,1,1} }; //结构体数组记录盒子相关信息

void LoadImage()//载入盒子图片
{
	loadimage(&im_box[0], _T("./res/box/box0.png"));//载入盒子0图片
	loadimage(&im_box[1], _T("./res/box/box1.png"));//载入盒子1图片
	loadimage(&im_box[2], _T("./res/box/box2.png"));//载入盒子2图片
	loadimage(&im_box[3], _T("./res/box/box3.png"));//载入盒子3图片
	loadimage(&im_box[4], _T("./res/box/box4.png"));//载入盒子4图片
	loadimage(&im_box[5], _T("./res/box/box5.png"));//载入盒子5图片
	loadimage(&im_box[6], _T("./res/box/box6.png"));//载入盒子6图片
	loadimage(&im_pause, _T("./res/pause.png"));//载入暂停图片
}

void DrawStill(int includeplayer)
{
	int i;
	for (i = 14;i >= 0;i--) //倒序生成盒子（营造覆盖效果）
	{
		putimagePng(box[i].c_x - 151.5, box[i].c_y - 90.3, &im_box[box[i].boxid]);//加载盒子图片
	}
	if (includeplayer == 1)
		fillcircle(player.x, player.y, 10);//绘制玩家
	PrintScore(50, 50, 50);//输出分数
}

int Searchid() //寻找玩家距离最近的盒子
{
	int j; //定义计数器
	LoadImage();
	int min = 10000, minid = -1; //初始化最小值
	for (j = 0;j < 15;j++) //遍历数组查找
	{
		double pbdistance = (player.x - box[j].c_x) * (player.x - box[j].c_x) + (player.y - box[j].c_y) * (player.y - box[j].c_y); //定义玩家与盒子的距离
		if (pbdistance < min)
		{
			min = pbdistance; //记录目前为止最小距离
			minid = j; //记录目前为止最近盒子ID
		}
	}
	return minid; //返回最小ID
}

void readRecordFile()  //读取游戏数据文件存档
{
	int i; //定义计数器
	FILE* fp; //定义文件
	fopen_s(&fp, ".\\gameRecord.dat", "r");
	if (fp)
	{
		for (i = 0;i < 15;i++)
		{
			fscanf_s(fp, "%f %f %d %d", &box[i].c_x, &box[i].c_y, &box[i].dir, &box[i].boxid);
		}
		fscanf_s(fp, "%d %d %d %f %f", &id, &score, &count, &player.x, &player.y);
		fclose(fp);
	}
}

void writeRecordFile()  //存储游戏数据文件存档
{
	int i; //定义计数器
	FILE* fp;
	fopen_s(&fp, ".\\gameRecord.dat", "w");
	if (fp) //如果文件存在
	{
		for (i = 0;i < 15;i++)
		{
			fprintf(fp, "%f %f %d %d", box[i].c_x, box[i].c_y, box[i].dir, box[i].boxid);
		}
		fprintf(fp, "%d %d %d %f %f", id, score, count, player.x, player.y);
		fclose(fp);
	}
}

void PrintScore(float x, float y, int fheight)
{
	TCHAR s[20]; // 定义字符串数组
	_stprintf_s(s, _T("%d"), score); // 将score转换为字符串
	setbkmode(TRANSPARENT);//设置字体背景为透明
	settextcolor(BLACK);//设置文字颜色为黑色
	settextstyle(fheight, 0, _T("04b_03b")); // 设置文字大小、字体
	outtextxy(x, y, s); // 输出得分文字
}

void ProduceBox()//继承旧的盒子并且生成新的盒子
{
	int i; //计数器
	double distance; //记录盒子y方向移动距离
	for (i = 0;i < 5;i++) //继承上一组最后10个盒子的信息
	{
		box[i] = box[i + 10];
	}
	for (i = 5;i < 15;i++)
	{
		count++; //计数器+1
		if (count > 2)//当未转向的盒子数大于2时
		{
			box[i].dir = rand() % 2; //生成下一个盒子方向
			if (box[i].dir != box[i - 1].dir) //如果盒子转向
			{
				count = 0;//重新开始计数
			}
		}
		else //当未转向的盒子数小于等于2时
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
		box[i].boxid = rand() % 7;
		//box[i].im_box = im_box[box[i].boxid]; //随机获取盒子图片
	}
}

void Failure()//输出失败文字
{
	int i; //定义计数器
	TCHAR s[] = _T("YOU FAIL");//记录失败文字
	setbkmode(TRANSPARENT);//设置字体背景为透明
	BeginBatchDraw();
	for (i = 0;i < 1000;i++)
	{
		cleardevice(); //清屏
		DrawStill(0);
		fillcircle(player.x, player.y, 10); //绘制玩家
		settextcolor(RED); //设置文字颜色为红色
		settextstyle(0.08 * i, 0.08 * i, _T("04b_03b")); // 设置文字大小、字体
		outtextxy(WIDTH / 2 - 0.32 * i, HEIGHT / 2 - 0.04 * i, s); // 输出得分文字
		FlushBatchDraw();
		Sleep(0.1);
	}
	EndBatchDraw();
	Sleep(1000);

	cleardevice();
	int c = 0, t_score; //计算score位数
	IMAGE im_score; //定义分数栏图片
	IMAGE im_flower; //定义花朵图片
	IMAGE im_replay; //定义重新开始图片
	IMAGE im_exit; //定义重新开始图片
	loadimage(&im_score, _T("./res/changlle_share1.png"));//载入开始游戏按钮图片
	loadimage(&im_replay, _T("./res/replay2.png")); //载入重新按钮图片
	loadimage(&im_exit, _T("./res/exit.png")); //载入继续图片
	loadimage(&im_flower, _T("./res/flower.png"));
	t_score = score; //临时存储分数
	while (t_score > 0) //计算分数位数
	{
		c++;
		t_score = t_score / 10;
	}
	//游戏结束动画
	BeginBatchDraw();
	for (i = 0;i < 100;i++)
	{
		cleardevice();
		putimagePng(WIDTH / 2 - im_score.getwidth() / 2, 0, &im_score);//加载“分数'图片
		putimagePng(WIDTH / 2 - im_replay.getwidth() / 2 - 80, 0.5 * i, &im_flower);
		putimagePng(WIDTH / 2 - im_exit.getwidth() / 2, HEIGHT * 4 / 7 + im_replay.getheight() + 15, &im_exit);//加载退出按钮图片
		putimagePng(WIDTH / 2 - im_replay.getwidth() / 2, HEIGHT * 4 / 7, &im_replay);//加载重新开始按钮图片
		PrintScore(WIDTH / 2 - 25 * c, 200, 100); //打印份数
		FlushBatchDraw();
		Sleep(5);
	}
	EndBatchDraw();

	while (1)//循环等待用户动作
	{
		ExMessage m; //记录鼠标动作
		m = getmessage(EM_MOUSE); //获取鼠标动作
		if (m.message == WM_LBUTTONDOWN)//鼠标移动到重新开始游戏按钮处并且点击
		{
			if ((m.x >= WIDTH / 2 - im_replay.getwidth() / 2) && (m.x <= WIDTH / 2 + im_replay.getwidth() / 2) && (m.y >= HEIGHT * 4 / 7) && (m.y <= HEIGHT * 4 / 7 + im_replay.getheight()))
			{
				m = getmessage(EM_MOUSE); //再次获取鼠标信息
				if (m.message == WM_LBUTTONUP)//如果释放鼠标左键，重新开始游戏
				{
					cleardevice();
					StartGame();
					PlayerMove();
				}
			}
			else if ((m.x >= WIDTH / 2 - im_exit.getwidth() / 2) && (m.x <= WIDTH / 2 + im_exit.getwidth() / 2) && (m.y >= HEIGHT * 4 / 7 + im_replay.getheight() + 15) && (m.y <= HEIGHT * 4 / 7 + im_replay.getheight() + 15 + im_exit.getheight()))
			{
				exit(0); //退出游戏
			}
			else
				continue;
		}
	}
}

void StartMenu()
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
	IMAGE im_continue;//定义图片
	loadimage(&im_play, _T("./res/play1.png"));//载入开始游戏按钮图片
	loadimage(&im_bk, _T("./res/background.png"));//载入背景图片
	loadimage(&im_title, _T("./res/title.png"));//载入标题图片
	loadimage(&im_continue, _T("./res/continue.png"));//载入继续游戏图片
	putimagePng(WIDTH / 2 - im_play.getwidth() / 2, HEIGHT * 4 / 7, &im_play);//加载开始游戏图片
	putimagePng(WIDTH / 2 - im_continue.getwidth() / 2, HEIGHT * 4 / 7 + im_play.getheight() + 15, &im_continue);//加载开始游戏图片
	putimagePng(WIDTH / 2 - im_title.getwidth() / 2, HEIGHT / 5, &im_title);//加载“跳一跳'图片

	//鼠标点击“开始游戏”
	while (1)//循环等待用户动作
	{
		ExMessage m; //记录鼠标动作
		m = getmessage(EM_MOUSE);
		if (m.message == WM_LBUTTONDOWN)//鼠标移动到重新开始游戏按钮处并且点击
		{
			if ((m.x >= WIDTH / 2 - im_play.getwidth() / 2)
				&& (m.x <= WIDTH / 2 + im_play.getwidth() / 2)
				&& (m.y >= HEIGHT * 4 / 7)
				&& (m.y <= HEIGHT * 4 / 7 + im_play.getheight()))
			{
				m = getmessage(EM_MOUSE); //再次获取鼠标信息
				if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
					break; //等待鼠标抬起后再退出循环，解决start_t未初始化问题
			}
			else if ((m.x >= WIDTH / 2 - im_continue.getwidth() / 2) && (m.x <= WIDTH / 2 + im_continue.getwidth() / 2) && (m.y >= HEIGHT * 4 / 7 + im_play.getheight() + 15) && (m.y <= HEIGHT * 4 / 7 + im_play.getheight() + 15 + im_continue.getheight()))
			{
				m = getmessage(EM_MOUSE); //再次获取鼠标信息
				if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
				{
					if (_access(".\\gameRecord.dat", 0) == 0)
					{
						IsContinue = 1;
						break;
					}
					else
					{
						break;
					}
				}
			}
			else
				continue;
		}
	}
	cleardevice();
}

void StartGame()
{
	//重置属性
	if (IsContinue != 1)
	{
		LoadImage();
		count = 0;
		score = 0;
		player.x = WIDTH / 3.0;
		player.y = HEIGHT * 2.0 / 3;

		//初始化玩家与盒子
		int i; //计数器
		double distance;//记录盒子y方向移动距离
		box[0] = { WIDTH / 3.0,HEIGHT * 2.0 / 3,1,1};
		for (i = 1;i < 15;i++)
		{
			count++; //计数器+1
			if (count > 2)//当未转向的盒子数大于2时
			{
				box[i].dir = rand() % 2;//生成下一个盒子方向
				if (box[i].dir != box[i - 1].dir)//如果盒子转向
				{
					count = 0;//重新开始计数
				}
			}
			else //当未转向的盒子数小于等于2时
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
		for (i = 14;i >= 0;i--)//倒序生成盒子（营造覆盖效果）
		{
			box[i].boxid = rand() % 7;
			//box[i].im_box = im_box[box[i].boxid]; //随机获取盒子图片
			putimagePng(box[i].c_x - 151.5, box[i].c_y - 90.3, &im_box[box[i].boxid]);//加载盒子图片
		}
		putimagePng(WIDTH - im_pause.getwidth() / 2 - 70, 50, &im_pause);
		fillcircle(player.x, player.y, 10);//绘制玩家
		PrintScore(50, 50, 50);//输出分数
	}
	else
	{
		readRecordFile();
		BeginBatchDraw();
		cleardevice();
		DrawStill(1);
		EndBatchDraw();
		PlayerMove();
	}	
}

void PlayerMove()
{
	//准备起跳
	id = Searchid(); //确定目前玩家所在方块的ID
	while (((player.x - box[id].c_x) * (player.x - box[id].c_x) + (player.y - box[id].c_y) * (player.y - box[id].c_y)) <= 5785 || ((player.x - box[id - 1].c_x) * (player.x - box[id - 1].c_x) + (player.y - box[id - 1].c_y) * (player.y - box[id - 1].c_y)) <= 5785)
	{
		putimagePng(WIDTH - im_pause.getwidth() / 2 - 70, 50, &im_pause);
		//获取鼠标按下时长
		//定义结构体获取鼠标按下与松开时间
		double t;
		while (1)
		{
			ExMessage m; //记录鼠标动作
			clock_t start_t = clock(), stop_t;
			m = getmessage(EM_MOUSE);
			if (m.message == WM_LBUTTONDOWN)
			{
				start_t = clock();
				if ((m.x >= WIDTH - im_pause.getwidth() / 2 - 70) && (m.x <= WIDTH + im_pause.getwidth() / 2 - 70) && (m.y >= 50) && (m.y <= 50 + im_pause.getheight()))
				{
					m = getmessage(EM_MOUSE);
					if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
					Pause();
				}
				else
				{
					m = getmessage(EM_MOUSE); //再次获取鼠标信息
					if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
					{
						stop_t = clock(); //获取鼠标释放时间
						t = (double)(stop_t - start_t);//计算鼠标按下时长
						break; //跳出循环
					}
				}
			}		
		}

		//玩家开始跳跃
		int i, j;//生成计数器
		int NeedCrossLine = 0;
		double g = 0.3;//定义重力值
		double vx = 5.5, vy;//定义x，y方向速度
		double distance = 2 * t / sqrt(27); //根据时长计算距离
		vy = distance / vx / 2 * g; //计算y方向出发速度
		BeginBatchDraw();
		id++; //变化ID用于确定直线
		//小球移动动画
		for (i = 0;i < distance / vx;i++)
		{
			//Pause();
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
				for (j = 14;j >= 0;j--)
				{
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &im_box[box[j].boxid]);
				}
				fillcircle(player.x, player.y, 10);
				PrintScore(50, 50, 50);
				FlushBatchDraw(); //批量绘制
				Sleep(10);
			}
		}
		EndBatchDraw(); //结束批量绘图

		//跳跃结束
		int formerid = id - 1;
		id = Searchid(); //判断移动后玩家所在的位置
		if (Searchid() == 12)
		{
			ProduceBox();
			DrawStill(1);
			id = Searchid(); //确定目前玩家所在方块的ID
			formerid = id - 1;

		}
		if (((player.x - box[id].c_x) * (player.x - box[id].c_x) + (player.y - box[id].c_y) * (player.y - box[id].c_y)) <= 5785)
		{
			if (formerid != id)
				score += 1;
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
				for (j = 14;j >= 0;j--)
				{
					//Pause();
					box[j].c_x -= x_distance / n; //计算每一帧玩家X移动距离
					box[j].c_y -= y_distance / n;//计算每一帧玩家Y移动距离
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &im_box[box[j].boxid]);//
				}
				fillcircle(player.x, player.y, 10); //绘制玩家新位置
				PrintScore(50, 50, 50);
				FlushBatchDraw(); //批量绘制
				Sleep(20);
			}
			EndBatchDraw(); //结束批量绘图
		}
		else if (((player.x - box[id - 1].c_x) * (player.x - box[id - 1].c_x) + (player.y - box[id - 1].c_y) * (player.y - box[id - 1].c_y)) <= 5780)
		{
			continue;
		}
		else
		{
			int j, t;
			BeginBatchDraw();
			for (i = 0;i < 100;i++)
			{
				cleardevice();
				player.y += 90.3 / 100.0;
				id = Searchid();
				if (player.x <= box[id].c_x + 153 && player.x >= box[id].c_x)
					t = 1;
				else
					t = 0;
				for (j = 14;j >= id + t;j--) //倒序生成盒子（营造覆盖效果）
				{
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &im_box[box[j].boxid]);//加载盒子图片
				}
				fillcircle(player.x, player.y, 10); //绘制玩家新位置
				for (j = id + t - 1;j >= 0;j--)//倒序生成盒子（营造覆盖效果）
				{
					putimagePng(box[j].c_x - 151.5, box[j].c_y - 90.3, &im_box[box[j].boxid]);//加载盒子图片
				}
				PrintScore(50, 50, 50);//输出分数
				FlushBatchDraw();
				Sleep(5);
			}
			EndBatchDraw();
			break;
		}
	}
	Failure();
}

void Pause()
{
	ExMessage m; //记录鼠标动作
	m = getmessage(EM_MOUSE);
	if (m.message == WM_LBUTTONDOWN)//鼠标移动到重新开始游戏按钮处并且点击
	{
		if ((m.x >= WIDTH - im_pause.getwidth() / 2 - 50) 
			&& (m.x <= WIDTH + im_pause.getwidth() / 2 - 50) 
			&& (m.y >= 50) 
			&& (m.y <= 50 + im_pause.getheight()))
		{
			m = getmessage(EM_MOUSE); //再次获取鼠标信息
			if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
			{
				IMAGE im_exit; //定义退出游戏图片
				IMAGE im_replay; //定义重新开始图片
				IMAGE im_continue; //定义退出游戏图片
				loadimage(&im_replay, _T("./res/replay1.png")); //载入重新按钮图片
				loadimage(&im_continue, _T("./res/continue.png")); //载入继续图片
				loadimage(&im_exit, _T("./res/exit.png")); //载入重新按钮图片
				DrawStill(1);
				putimagePng(WIDTH / 2 - im_continue.getwidth() / 2, HEIGHT * 1 / 5, &im_continue);//加载继续开始按钮图片
				putimagePng(WIDTH / 2 - im_replay.getwidth() / 2, HEIGHT * 2 / 5, &im_replay);//加载重新开始按钮图片
				putimagePng(WIDTH / 2 - im_exit.getwidth() / 2, HEIGHT * 3 / 5, &im_exit);//加载退出按钮图片

				while (1)//循环等待用户动作
				{
					m = getmessage(EM_MOUSE);
					if (m.message == WM_LBUTTONDOWN)//鼠标点击
					{
						if ((m.x >= WIDTH / 2 - im_replay.getwidth() / 2) 
							&& (m.x <= WIDTH / 2 + im_replay.getwidth() / 2) 
							&& (m.y >= HEIGHT * 2 / 5) 
							&& (m.y <= HEIGHT * 2 / 5 + im_replay.getheight()))//如果鼠标点击区域在重新开始
						{
							m = getmessage(EM_MOUSE); //再次获取鼠标信息
							if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
							{
								cleardevice();
								StartGame();
								PlayerMove();
							}
						}
						else if ((m.x >= WIDTH / 2 - im_continue.getwidth() / 2) 
							&& (m.x <= WIDTH / 2 + im_continue.getwidth() / 2) 
							&& (m.y >= HEIGHT * 1 / 5) 
							&& (m.y <= HEIGHT * 1 / 5 + im_continue.getheight()))//点击继续游戏
						{
							m = getmessage(EM_MOUSE); //再次获取鼠标信息
							if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
							{
								BeginBatchDraw();
								cleardevice();
								DrawStill(1);
								EndBatchDraw();
								PlayerMove();
							}	
						}
						else if ((m.x >= WIDTH / 2 - im_exit.getwidth() / 2) 
							&& (m.x <= WIDTH / 2 + im_exit.getwidth() / 2) 
							&& (m.y >= HEIGHT * 3 / 5) 
							&& (m.y <= HEIGHT * 3 / 5 + im_exit.getheight()))//点击退出游戏
						{
							m = getmessage(EM_MOUSE); //再次获取鼠标信息
							if (m.message == WM_LBUTTONUP)//如果释放鼠标左键
							{
								writeRecordFile();
								exit(0);
							}	
						}
					}
				}
			}
		}
	}
}

int main()
{
	srand((unsigned)time(0)); //产生随机数种子
	StartMenu();
	StartGame();
	PlayerMove();
	_getch();
	closegraph();
	return 0;
}

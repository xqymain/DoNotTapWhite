#include <Windows.h>
#include "resource.h"
#include <time.h>

#define BOLCK 100

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //消息句柄 消息编号 附加参数 
//窗口程序的入口函数
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//设计窗口类
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//将背景刷成白色
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//加载光标
	wndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;//加载应用程序实例
	wndClass.lpfnWndProc = WindowProc;//窗口回调函数
	wndClass.lpszClassName = L"DoNotStepOnWhitePisces";//窗口类型名
	wndClass.lpszMenuName = NULL;//菜单名称
	wndClass.style = CS_VREDRAW | CS_HREDRAW;//风格

	//注册窗口//
	RegisterClass(&wndClass);

	//创建窗口
	HWND hWnd=CreateWindow(L"DoNotStepOnWhitePisces",
		L"别踩白块儿-经典模式",
		WS_SYSMENU|WS_CAPTION,
		50,
		50,
		BOLCK*4+16,
		BOLCK*4+34,
		NULL,
		NULL,
		hInstance,
		0
		);
	//显示窗口
	ShowWindow(hWnd,SW_SHOW);
	//更新
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int bw[4];
	static int t=0;

	HDC hDC;        //绘图句柄
	PAINTSTRUCT ps; //结构体 
	HPEN hPen;      //画笔句柄
	HBRUSH hBrush;  //画刷句柄
	POINT point = {0,0};    //鼠标点击坐标
	RECT rect;      //方块大小
	static int Flag;
	static int n = 0;          //点击方块的格数
	wchar_t szTemp[100];//字符数组
	
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		srand((unsigned)time(NULL));
		for (int i = 0; i
			< 4; i++)
		{
			bw[i] = rand() % 4;
		}
		//开启计时器
		SetTimer(hWnd,1,10,NULL);
		break;
	case WM_TIMER:
		t++;
		break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		for (int i = 0; i < 4; i++)
		{
			//指定矩形区域
			SetRect(&rect, bw[i]*BOLCK,i*BOLCK,bw[i]*BOLCK+BOLCK,i*BOLCK+BOLCK);
			//创建一支笔
			hPen=CreatePen(PS_SOLID,1,RGB(0,255,255));
			SelectObject(hDC, hPen);
			//创建一个画刷
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC,hBrush);
			//画一个方块
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			//释放资源
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd,&ps);//结束绘图
		break;
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		SendMessage(hWnd,WM_CHAR,point.x/BOLCK,0);
		break;
	case WM_CHAR:
		hDC=GetDC(hWnd);
		if (wParam!=bw[3])
		{
			Flag = -1;
			KillTimer(hWnd,0);//关闭定时器
			wsprintf(szTemp,L"游戏结束，你输了！用时：%d.%d秒 格数：%d   作者：邢庆宇",t/100,t-(t/100)*100,n);
			MessageBox(NULL,szTemp,L"提示",MB_ICONWARNING);
			exit(0);
		}
		for (int i = 3; i >= 1; i--)
		{
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;//点击黑块的次数
		ScrollWindow(hWnd,0,BOLCK,NULL,NULL);
		//指定矩形区域
		SetRect(&rect, bw[0] * BOLCK,0,bw[0]*BOLCK+BOLCK,BOLCK);
		//创建一支笔
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hDC, hPen);
		//创建一个画刷
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		//画一个方块
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		//释放资源
		DeleteObject(hPen);
		DeleteObject(hBrush);
		ReleaseDC(hWnd,hDC);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
// * Program Name: Do Not Tap White
// * Build Pass by VS 2013, 2015, 2017
// * Author: xqymain
// * Under GPL 3.0, For education way
// * Thanks for your support

// Reference the necessary header files
#include <Windows.h>
#include "resource.h"
#include <time.h>

// Define the size of the white block
#define BLOCK 100

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Message Handle, Message Number, Additional Parameters
// Define Entry function
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	// Design the window class
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	/*
	  GetStockObject() Function
	    The GetStockObject function retrieves a handle to one of the stock pens, brushes, fonts, or palettes.
	    
	      HGDIOBJ GetStockObject(
                _In_ int fnObject
              );
	      
	    fnObject [in]
         The type of stock object. This parameter can be one of the following values.
         For examples: BLACK_BRUSH -- Black brush.
	               DKGRAY_BRUSH -- Dark gray brush.
         Return value
	 If the function succeeds, the return value is a handle to the requested logical object.
	 If the function fails, the return value is NULL.
	 
         From: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
	*/
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Brush the background white
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance; // Load the application instance
	wndClass.lpfnWndProc = WindowProc; // Window callback Function
	wndClass.lpszClassName = L"DoNotTapWhite";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&wndClass);
	
	/*
	  CreateWindow() function

	    Creates an overlapped, pop-up, or child window. It specifies the window class, window title, window style, and (optionally) the initial position and size of the window. The function also specifies the window's parent or owner, if any, and the window's menu.
	    To use extended window styles in addition to the styles supported by CreateWindow, use the CreateWindowEx function.
	  
	    HWND WINAPI CreateWindow(
              _In_opt_ LPCTSTR   lpClassName, 
              _In_opt_ LPCTSTR   lpWindowName, 
              _In_     DWORD     dwStyle,       
              _In_     int       x,
              _In_     int       y,
              _In_     int       nWidth,
              _In_     int       nHeight,
              _In_opt_ HWND      hWndParent,
              _In_opt_ HMENU     hMenu,
              _In_opt_ HINSTANCE hInstance,
              _In_opt_ LPVOID    lpParam
           );
	   
	  Return: Window Handle
	  
          From: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632679(v=vs.85).aspx
	*/
	HWND hWnd = CreateWindow(L"DoNotTapWhite",
				 L"Do not Tap White - Classic Mode",
				 WS_SYSMENU|WS_CAPTION,
				 50,
				 50,
				 BLOCK*4+16,
				 BLOCK*4+34,
				 NULL,
				 NULL,
				 hInstance,
				 0
		);
	
	ShowWindow(hWnd,SW_SHOW);
	
	UpdateWindow(hWnd);

	// Message loop
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

	HDC hDC;            // Drawing handle
	PAINTSTRUCT ps;     // Struct
	HPEN hPen;          // Pen handle
	HBRUSH hBrush;      // Brush handle
	POINT point = {0,0};    // Mouse click coordinates
	RECT rect;          // Square size
	static int Flag;
	static int n = 0;      // The number of times you clicked the box
	wchar_t szTemp[128];
	
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
		
		SetTimer(hWnd,1,10,NULL);
		break;
	case WM_TIMER:
		t++;
		break;
	case WM_PAINT:
		hDC=BeginPaint(hWnd,&ps);
		for (int i = 0; i < 4; i++)
		{
			// Specify a rectangular area
			SetRect(&rect, bw[i]*BLOCK,i*BLOCK,bw[i]*BLOCK+BLOCK,i*BLOCK+BLOCK);
			
			hPen=CreatePen(PS_SOLID,1,RGB(0,255,255));
			SelectObject(hDC, hPen);
			
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC,hBrush);
			/*
			  Rectangle() function
			  
                            The Rectangle function draws a rectangle. 
			    The rectangle is outlined by using the current pen and filled by using the current brush.
			    
			    BOOL Rectangle(
                              _In_ HDC hdc,
                              _In_ int nLeftRect,
                              _In_ int nTopRect,
                              _In_ int nRightRect,
                              _In_ int nBottomRect
                            );
                          
			  Return:
                            If the function succeeds, the return value is nonzero.
                            If the function fails, the return value is zero.
			    
			  From: https://msdn.microsoft.com/en-us/library/windows/desktop/dd162898(v=vs.85).aspx
			*/
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);  // Draw a square
			
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd,&ps);
		break;
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		SendMessage(hWnd,WM_CHAR,point.x/BLOCK,0);
		break;
	case WM_CHAR:
		hDC=GetDC(hWnd);
		if (wParam!=bw[3])
		{
			Flag = -1;
			KillTimer(hWnd,0);
			wsprintf(szTemp,L"Game over, time cost: %d.%d seconds.\nWay: %d.",t/100,t-(t/100)*100,n);
			MessageBox(HWnd,szTemp,L"Do not Tap White",MB_OK);
			exit(0);
		}
		for (int i = 3; i >= 1; i--)
		{
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;    // Click the number of black blocks
		ScrollWindow(hWnd,0,BLOCK,NULL,NULL);
		
		SetRect(&rect, bw[0] * BLOCK,0,bw[0]*BLOCK+BLOCK,BLOCK);
		
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hDC, hPen);
		
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		
		DeleteObject(hPen);
		DeleteObject(hBrush);
		ReleaseDC(hWnd,hDC);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Yes, it's that simple. :)

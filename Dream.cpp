// Dream.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "time.h"
#include "mmsystem.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int x[10];
int y[10];
int incx[10];
int incy[10];
int xmax;
int ymax;
HPEN hpen[10];
HBRUSH bbrush;
int lcount = 10;
int clear = 1;
unsigned int tid = 1;


int	timer = 0;
//bool rndBounce = true;
bool rndBounce = false;


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// set timer res to 5 ms
	timeBeginPeriod(5);
	srand( (unsigned)time( NULL ) );	// init random number generator


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DREAM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_DREAM);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_DREAM);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_DREAM;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


/*DWORD colors[] = {
	RGB(255, 0, 0),
	RGB(255, 128, 0),
	RGB(0, 255, 0),
	RGB(255, 255,0),
	RGB(0,0,255),
	RGB(255, 0, 255),
	RGB(0, 255, 255),
	RGB(255, 255, 255),
	RGB(0,128,255),
	RGB(128, 0, 255),
	RGB(255, 0, 128),
	RGB(0,255,128),
	RGB(128, 255,0),
};*/
DWORD colors[] = {
	RGB(200, 25, 25),
	RGB(200, 128, 25),
	RGB(25, 200, 25),
	RGB(200, 200, 25),
	RGB(25, 25, 200),
	RGB(200, 25, 200),
	RGB(25, 200, 200),
	RGB(200, 200, 200),
	RGB(25, 128, 200),
	RGB(128, 25, 200),
	RGB(200, 25, 128),
	RGB(25, 200, 128),
	RGB(128, 200, 25),
};

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
	int i;

   hInst = hInstance; // Store instance handle in our global variable

//   hWnd = CreateWindow(szWindowClass, szTitle, WS_MAXIMIZE|WS_BORDER,
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
	bbrush = CreateSolidBrush( RGB(0,0,0));

   for(i=0; i<lcount; i++)
   {
	   // if odd then pen is black
	  // if (i & 1) hpen[i] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	   if (i & 1) hpen[i] = CreatePen(PS_SOLID, 1, RGB(10, 10, 10));
	   // else create a color (2,4,6,8)
	   else hpen[i] = CreatePen(PS_SOLID, 1, colors[(i/2) %12]);
//	   hpen[i] = CreatePen(PS_SOLID, 1, colors[(i) %12]);
   }
   //SetTimer(hWnd, tid, 15, NULL);
   SetTimer(hWnd, tid, 20, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


// generate a random # from 0 to r
//
int rndRange(int r)
{
	return (rand()*r + RAND_MAX/2)/RAND_MAX;
}

void init_pattern(RECT rt)
{
	// find the center
	int i, cx, cy;
	cx = (rt.right-rt.left) / 2;
	cy = (rt.bottom-rt.top) / 2;

	// generate the initial line point positions and directions
	// velocity is random from -5 to 5
	//
   for(i=0; i<lcount; i++)
   {
//	   incx[i] = ((i&1)?(i+1):(-(i+1)))%5-1;	// value is 1 to 5 or -1 to -5
//	   incy[i] = ((i&2)?-(i+1):(i+1))%5+1;
	   incx[i] = rndRange(10) - 5;
	   incy[i] = rndRange(10) - 5;
	   if(incx[i] == 0) incx[i]++;
	   if(incy[i] == 0) incy[i]++;
	   x[i] = cx+rndRange(20) - 10;
	   y[i] = cy+rndRange(20) - 10;


   }
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	int i;


	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			HPEN oldpen;
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			GetClientRect(hWnd, &rt);
			SelectObject(hdc, bbrush);

			if(clear)
			{
				Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
				clear = 0;
			}
			
			oldpen = (HPEN) SelectObject(hdc, hpen[0]);
			MoveToEx(hdc, x[0], y[0], NULL);
			for(i=1; i<lcount; i++)
			{
				LineTo(hdc, x[i], y[i]);
				SelectObject(hdc, hpen[i]);
			}
			LineTo(hdc, x[0], y[0]);


			SelectObject(hdc, oldpen);
			//DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;

		case WM_TIMER:
			timer++;
			GetClientRect(hWnd, &rt);

			xmax = rt.right-rt.left;
			ymax = rt.bottom-rt.top;
			for(i=0; i < lcount; i++)
			{
				x[i] += incx[i];
				if((x[i] > xmax) || (x[i] < 0))
				{
					incx[i] = -incx[i];
					x[i] += incx[i];
				}

				y[i] += incy[i];
				if((y[i] > ymax)  || (y[i] < 0))
				{
					incy[i] = -incy[i];
					y[i] += incy[i];
				}
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CREATE:
		case WM_SIZE:
			GetClientRect(hWnd, &rt);
			init_pattern(rt);
			clear = 1;
			// fall thru to default
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

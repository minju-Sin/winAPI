// 비트맵 사진 윈도우 경계 튕기기 
#include "resource.h"
#include <windows.h>
#include <TCHAR.h>
#define WIDTH 160
#define HEIGHT 160 // 출력 영상의 폭 넓이 
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// 커서

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 윈도우 배경 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //윈도우가 나타날 x위치
		CW_USEDEFAULT, //윈도우가 나타날 y위치 
		CW_USEDEFAULT, // 윈도우의 크기가 600x400이 되게 한다. 
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static RECT rectView;
	static HBITMAP hBitmap;
	static int x, y; // 윈도우 x,y좌표 
	static int xStep, yStep; 
	switch (iMsg)
	{
	case WM_CREATE:
		x = 50, y = 50; // 그림 출력 좌상단 좌표(x,y)
		xStep = 20, yStep = 20; // 한번 이동 거리
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		break;
	case WM_KEYDOWN:
		SetTimer(hwnd, 0, 100, NULL);
		break;
	case WM_TIMER:
		GetClientRect(hwnd, &rectView);
		x = x + xStep;
		y = y + yStep;
		if (x + WIDTH > rectView.right || x < 0) { // 좌우 경계 충돌하면 x방향 변경
			xStep = -1 * xStep;
		}
		if (y + HEIGHT > rectView.bottom || y < 0) { // 상하 경계 충돌하면 y방향 변경
			yStep = -1 * yStep;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		BitBlt(hdc, x, y, WIDTH, HEIGHT, memdc, 60, 60, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 0);
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
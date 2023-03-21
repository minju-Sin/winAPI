//고양이 쥐 따라가기 
#include "resource.h"
#include <windows.h>
#include <TCHAR.h>
#include <math.h>

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
	HDC hdc, memdc1, memdc2;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap1, hBitmap2;
	static RECT catRect = { 100, 100, 150, 150 };
	static RECT mouseRect;
	static bool isClick; // 마우스 클릭
	switch (iMsg)
	{
	case WM_CREATE: //윈도우 프로그램 처음 사용할 때 실행됨 
		isClick = false;
		hBitmap1 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap2 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc1 = CreateCompatibleDC(hdc);
		SelectObject(memdc1, hBitmap1);
		memdc2 = CreateCompatibleDC(hdc);
		SelectObject(memdc2, hBitmap2);
		if (isClick) {
			BitBlt(hdc, mouseRect.left, mouseRect.top, 68, 49, memdc2, 0, 0, SRCCOPY);
		}
		BitBlt(hdc, catRect.left, catRect.top, 67, 47, memdc1, 0, 0, SRCCOPY);
		DeleteDC(memdc1);
		DeleteDC(memdc2);
		EndPaint(hwnd, &ps);
		break;

	// 고양이가 쥐를 잡으러 자동 이동
	case WM_TIMER:
		if (isClick) { // 마우스 왼쪽 버튼 눌렀을 때 
			catRect.right = catRect.left + 50;
			catRect.bottom = catRect.top + 50;

			if (mouseRect.left > catRect.left) {
				catRect.left += 1;
			}
			else catRect.left -= 1;

			if (mouseRect.top > catRect.top) {
				catRect.top += 1;
			}
			else catRect.top -= 1;
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;
		
	case WM_LBUTTONDOWN: // 마우스 왼쪽 버튼  눌렀을 때
		isClick = true; 
		//마우스 좌표 - > 쥐 출력 
		mouseRect = { LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + 20, HIWORD(lParam) + 20 };
		//고양이가 쥐를 잡으러 이동한다
		SetTimer(hwnd, 1, 1, NULL);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

		
	case WM_LBUTTONUP: // 마우스 왼쪽 버튼 뗐을 때
		isClick = false;
		//고양이가 멈춘다
		KillTimer(hwnd, 1);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE: // 마우스 움직일 때
		hdc = GetDC(hwnd);
		//마우스 왼쪽 버튼 눌렀을 때 
		if (isClick) {
			//쥐가 마우스 움직이는 방향대로 따라간다
			mouseRect = { LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + 20, HIWORD(lParam) + 20 }; //쥐 출력 
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		ReleaseDC(hwnd, hdc);
		break;
		
	case WM_DESTROY: //윈도우 프로그램 종료할 때 실행됨 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
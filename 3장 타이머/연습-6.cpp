#include <windows.h>
#include <TCHAR.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// 커서 지정

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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
	HDC hdc;
	PAINTSTRUCT ps;
	static bool Drag; 
	static int cx, cy; //고양이
	static int mx, my; //쥐 
	static int stepX, stepY, count; // count는 타이머 발생시마다 1 증가, 고양이 이동 
	
	switch (iMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		cx = cy = 100;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, cx, cy, _T("고양이"), _tcslen(_T("고양이")));
		if (Drag) {
			TextOut(hdc, mx, my, _T("쥐"), _tcslen(_T("쥐")));
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN: 
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		stepX = (mx - cx) / 10; // 고양이와 쥐의 거리 / 10 = 한번 움직이는 x좌표
		stepY = (my - cy) / 10;
		Drag = TRUE;
		InvalidateRgn(hwnd, NULL, TRUE);
		count = 0;
		SetTimer(hwnd, 1, 200, NULL);
		break;

	case WM_LBUTTONUP:
		Drag = FALSE;
		KillTimer(hwnd, 1);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		if (!Drag) break;
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		stepX = (mx - cx) / 10; // 고양이와 쥐의 거리 / 10 = 한번 움직이는 x좌표
		stepY = (my - cy) / 10;
		InvalidateRgn(hwnd, NULL, TRUE);
		count = 0;
		break;

	case WM_TIMER:
		count++; // 한번 이동거리 계산시 분모 10과 연계
				// 10번 만에 쥐에게 이동, 10이면 고양이 정지 
		cx += stepX;
		cy += stepY;
		InvalidateRgn(hwnd, NULL, TRUE);
		if (count == 10) { stepX = 0, stepY = 0; }
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
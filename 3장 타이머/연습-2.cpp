#include <windows.h>
#include <TCHAR.h>
#define r_2 40 // 지름 

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
	RECT rectView; // 윈도우 경계 
	static int x, y; // 현재 원의 x,y좌표
	static int dx, dy, flag;
	switch (iMsg)
	{
	case WM_CREATE:
		x = 20, y = 20; // 시작 원의 좌표는 (20,20)
		dx = 40, dy = 0; //처음은 오른쪽으로 이동 
		flag = 0; // enter누르면 이동 
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rectView);
		hdc = BeginPaint(hwnd, &ps);
		Rectangle(hdc, 0, 0, r_2 * (int)(rectView.right / r_2), r_2 * (int)(rectView.bottom / r_2)); // 경계선
		Ellipse(hdc, x-20, y-20, x + 20, y + 20); //원
		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		GetClientRect(hwnd, &rectView);
		switch (wParam) {
		case VK_RETURN:
			flag = 1 - flag;
			if (flag == 1) SetTimer(hwnd, 1, 100, NULL);
			else KillTimer(hwnd, 1);
			break;
		case VK_RIGHT:
			dx = 40, dy = 0;
			break;
		case VK_LEFT:
			dx = -40, dy = 0;
			break;
		case VK_UP:
			dx = 0, dy = -40;
			break;
		case VK_DOWN:
			dx = 0, dy = +40;
			break;
		}
		break;
	case WM_TIMER:
		GetClientRect(hwnd, &rectView);
		x += dx; y += dy;
		if (x < rectView.left || x > r_2 * (int)(rectView.right / r_2)) {
			x -= dx;
		}
		if (y < rectView.top || y > r_2 * (int)(rectView.bottom / r_2)) {
			y -= dy;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
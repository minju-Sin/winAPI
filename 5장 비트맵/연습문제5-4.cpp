// ��Ʈ�� ���� ������ ��� ƨ��� 
#include "resource.h"
#include <windows.h>
#include <TCHAR.h>
#define WIDTH 160
#define HEIGHT 160 // ��� ������ �� ���� 
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
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// Ŀ��

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������ ��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");
	RegisterClass(&WndClass);
	hwnd = CreateWindow(_T("Window Class Name"),
		_T("Window Title Name"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, //�����찡 ��Ÿ�� x��ġ
		CW_USEDEFAULT, //�����찡 ��Ÿ�� y��ġ 
		CW_USEDEFAULT, // �������� ũ�Ⱑ 600x400�� �ǰ� �Ѵ�. 
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
	static int x, y; // ������ x,y��ǥ 
	static int xStep, yStep; 
	switch (iMsg)
	{
	case WM_CREATE:
		x = 50, y = 50; // �׸� ��� �»�� ��ǥ(x,y)
		xStep = 20, yStep = 20; // �ѹ� �̵� �Ÿ�
		hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		break;
	case WM_KEYDOWN:
		SetTimer(hwnd, 0, 100, NULL);
		break;
	case WM_TIMER:
		GetClientRect(hwnd, &rectView);
		x = x + xStep;
		y = y + yStep;
		if (x + WIDTH > rectView.right || x < 0) { // �¿� ��� �浹�ϸ� x���� ����
			xStep = -1 * xStep;
		}
		if (y + HEIGHT > rectView.bottom || y < 0) { // ���� ��� �浹�ϸ� y���� ����
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
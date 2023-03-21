//����� �� ���󰡱� 
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
	HDC hdc, memdc1, memdc2;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap1, hBitmap2;
	static RECT catRect = { 100, 100, 150, 150 };
	static RECT mouseRect;
	static bool isClick; // ���콺 Ŭ��
	switch (iMsg)
	{
	case WM_CREATE: //������ ���α׷� ó�� ����� �� ����� 
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

	// ����̰� �㸦 ������ �ڵ� �̵�
	case WM_TIMER:
		if (isClick) { // ���콺 ���� ��ư ������ �� 
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
		
	case WM_LBUTTONDOWN: // ���콺 ���� ��ư  ������ ��
		isClick = true; 
		//���콺 ��ǥ - > �� ��� 
		mouseRect = { LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + 20, HIWORD(lParam) + 20 };
		//����̰� �㸦 ������ �̵��Ѵ�
		SetTimer(hwnd, 1, 1, NULL);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

		
	case WM_LBUTTONUP: // ���콺 ���� ��ư ���� ��
		isClick = false;
		//����̰� �����
		KillTimer(hwnd, 1);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE: // ���콺 ������ ��
		hdc = GetDC(hwnd);
		//���콺 ���� ��ư ������ �� 
		if (isClick) {
			//�㰡 ���콺 �����̴� ������ ���󰣴�
			mouseRect = { LOWORD(lParam), HIWORD(lParam), LOWORD(lParam) + 20, HIWORD(lParam) + 20 }; //�� ��� 
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		ReleaseDC(hwnd, hdc);
		break;
		
	case WM_DESTROY: //������ ���α׷� ������ �� ����� 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
#include <windows.h>
#include <TCHAR.h>
#include <math.h>
#include "resource.h"
#define BSIZE 80
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgPunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;



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
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// 커서 지정

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
	switch (iMsg)
	{
	case WM_CREATE:
		break;

	case WM_COMMAND: // 메뉴 
		switch (LOWORD(wParam)) {
		case ID_1:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DlgPunc);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

// 대화상자 
BOOL CALLBACK DlgPunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	int xi, yi;
	static int rect[3][3];
	static int x, y, count;
    switch (iMsg)
    {
    case WM_INITDIALOG:
		x = 0;
		y = 0;
		count = 0;
        break;
    case WM_LBUTTONDOWN:
		x = LOWORD(lParam) / BSIZE;

		y = HIWORD(lParam) / BSIZE;

		if (x < 3 && y < 3 && rect[y][x] == 0) {
			count = 1 - count;
			rect[y][x] = count ? 1 : 2;
		}
		else if ((x < 3 && y < 3) && (rect[y][x] == 1 || rect[y][x] == 2)) {
			rect[y][x] = 0;
		}
        InvalidateRgn(hDlg, NULL, TRUE);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, 0);
            break;
        }
		break;
   
    case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
			for (xi = 0; xi < 3; xi++) {
				for (yi = 0; yi < 3; yi++) {
					Rectangle(hdc, xi * BSIZE, yi * BSIZE, (xi + 1) * BSIZE, (yi + 1) * BSIZE);
					if (rect[yi][xi]) {
						if (rect[yi][xi] == 1) {
							hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
							oldPen = (HPEN)SelectObject(hdc, hPen);

							Ellipse(hdc, xi * BSIZE, yi * BSIZE, (xi + 1) * BSIZE, (yi + 1) * BSIZE);
							SelectObject(hdc, oldPen);
						}
						else {
							hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
							oldPen = (HPEN)SelectObject(hdc, hPen);

							Rectangle(hdc, xi * BSIZE, yi * BSIZE, (xi + 1) * BSIZE, (yi + 1) * BSIZE);
							SelectObject(hdc, oldPen);
						}
						
					}
				}
			}
			break;
		EndPaint(hDlg, &ps);
		break;

    }

    return 0;
}

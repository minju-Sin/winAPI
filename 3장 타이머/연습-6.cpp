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

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// Ŀ�� ����

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
	static int cx, cy; //�����
	static int mx, my; //�� 
	static int stepX, stepY, count; // count�� Ÿ�̸� �߻��ø��� 1 ����, ����� �̵� 
	
	switch (iMsg)
	{
	case WM_CREATE:
		Drag = FALSE;
		cx = cy = 100;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, cx, cy, _T("�����"), _tcslen(_T("�����")));
		if (Drag) {
			TextOut(hdc, mx, my, _T("��"), _tcslen(_T("��")));
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN: 
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		stepX = (mx - cx) / 10; // ����̿� ���� �Ÿ� / 10 = �ѹ� �����̴� x��ǥ
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
		stepX = (mx - cx) / 10; // ����̿� ���� �Ÿ� / 10 = �ѹ� �����̴� x��ǥ
		stepY = (my - cy) / 10;
		InvalidateRgn(hwnd, NULL, TRUE);
		count = 0;
		break;

	case WM_TIMER:
		count++; // �ѹ� �̵��Ÿ� ���� �и� 10�� ����
				// 10�� ���� �㿡�� �̵�, 10�̸� ����� ���� 
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
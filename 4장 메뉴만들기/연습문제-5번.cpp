// ���� �ִ� ������ �� �׸��� 
#define LINE 1 // �޴����� ���� ������ ���� object_mode�� ����
#define ELLIPSE 2 // �׸��� �κп��� �� �������� ���� �ش� ���� �׸�
#define RECTANGLE 3 // object_mode�� �����ϴ� ��

#include <windows.h>
#include <TCHAR.h>
#include "resource.h"
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU4_5);
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

//�� �޴� Ŭ���� �����ȭ���� ���� 
//����ڰ� ������ �� ��ȯ�޴� �κ��� �Լ��� �����Ͽ� ȣ��
//-> ���ڷ� ���� ��� �� �ʿ� 
//->��ҹ�ư ���� �ϸ� ����ϴ� �� ��ȯ
COLORREF ColorSelection(HWND hwnd, COLORREF oldColor) { // ���� ����ϴ� color
	COLORREF tmp[16]; CHOOSECOLOR COLOR;
	for (int i = 0; i < 16; i++)
		tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
	memset(&COLOR, 0, sizeof(CHOOSECOLOR));
	COLOR.lStructSize = sizeof(CHOOSECOLOR);
	COLOR.hwndOwner = hwnd;
	COLOR.lpCustColors = tmp;
	COLOR.Flags = CC_FULLOPEN;
	if (ChooseColor(&COLOR) != 0)
	{
		return COLOR.rgbResult;
	}
	else
		return oldColor;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int object_mode; // �������� 
	static int startX, startY, oldX, oldY; // ���� ��ǥ�� ���콺 �̵��ϱ� �� ��ǥ ����ϴ� ����  (static���� �����ϸ� 0���� �ڵ� �ʱ�ȭ)
	static int endX, endY; //���� ���콺 ��ǥ 
	static BOOL Drag; //���� ��ư ������ TRUE, ���� FALSE 
	HPEN hPen, oldPen; //�� �׵θ� 
	HBRUSH hBrush, oldBrush; //���� �귯�� 
	static COLORREF brushcolor = RGB(255, 255, 255);
	static COLORREF pencolor = RGB(0,0,0);
	switch (iMsg)
	{
	case WM_CREATE: //�� �� ����, �� �� ���� ��/�� ������ ����, ��Ÿ ���� �ʱ�ȭ
		hdc = GetDC(hwnd);
		hPen = CreatePen(PS_SOLID, 1 , pencolor); // �� ���� ���������� ���� 
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(brushcolor); // �� ���� ������� ����  
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush); 
		startX = oldX = 0, startY = oldY = 0; // ������ (0,0)
		object_mode = 0;  
		Drag = FALSE; 

		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
		break;

	case WM_COMMAND: // �޴� msg ó�� 
		hdc = GetDC(hwnd);
		switch (LOWORD(wParam))
		{
		case ID_LINE: // ���� �׸��� �޴�
			object_mode = LINE; // ���������� LINE ����
			break;
		case ID_ELLIPSE: // �� �׸��� �޴� 
			object_mode = ELLIPSE; // ���������� ELLIPSE ����
			break;
		case ID_RECTANGLE: // �簢�� �׸��� �޴�
			object_mode = RECTANGLE; // ���������� RECTANGLE ����
			break;
		case ID_PENCOLOR: // �� ���� �ٲٱ� �޴�
			pencolor = ColorSelection(hwnd, pencolor);
			break;
		case ID_FACECOLOR: // �� ���� �ٲٱ� �޴� 
			brushcolor = ColorSelection(hwnd, brushcolor);
			break;
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_LBUTTONDOWN: // ��ư�� ������ �巡�� ���� ���� 
		oldX = startX = LOWORD(lParam);
		oldY = startY = HIWORD(lParam);
		Drag = TRUE;
		break;

	case WM_LBUTTONUP: // ��ư�� ������ �巡�� ���� 
		Drag = FALSE;
		break;

	case WM_MOUSEMOVE: // ���������� ���� ������ ���� �׸� 
		if (Drag) {
			hdc = GetDC(hwnd);
			SetROP2(hdc, R2_NOTXORPEN);

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			hPen = CreatePen(PS_SOLID, 1, pencolor);
			oldPen = (HPEN)SelectObject(hdc, hPen);
			
			hBrush = CreateSolidBrush(brushcolor);
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			switch (object_mode) {
			case LINE: // ����
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, oldX, oldY); // ����� 

				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, endX, endY);  // �׸��� 
				break;
			case ELLIPSE: // ��
				Ellipse(hdc, startX, startY, oldX, oldY); // ����� 
				Ellipse(hdc, startX, startY, endX, endY); // �׸��� 
				break;
			case RECTANGLE: // �簢�� 
				Rectangle(hdc, startX, startY, oldX, oldY); // �����
				Rectangle(hdc, startX, startY, endX, endY); // �׸��� 
				break;
			}
			oldX = endX;
			oldY = endY; // �� ��ġ�� ���� �������� ����
			ReleaseDC(hwnd, hdc);
		}
		break;  

	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
// ���� �ִ� ������ �� �׸��� 
// ��ü���� ������� �ڵ� ���� 
#define LINE 1 // �޴����� ���� ������ ���� object_mode�� ����
#define ELLIPSE 2 // �׸��� �κп��� �� �������� ���� �ش� ���� �׸�
#define RECTANGLE 3 // object_mode�� �����ϴ� ��

#include <windows.h>
#include <TCHAR.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

class Point {
private:
	int x, y;
public:
	Point(int xx, int yy);
	int GetX() { return x; }
	int GetY() { return y; }
};
Point::Point(int xx, int yy)
	: x(xx), y(yy) {}

class Object {
	//private �����ϴ� ��� public �Լ� ����ϸ� �ڽĿ��� ���� ���� , but �ڵ� ����� 
protected:
	Point p1, p2;
public:
	Object(int x1, int y1, int x2, int y2);
	virtual  void Draw(HDC hdc);
};
Object::Object(int x1, int y1, int x2, int y2)
	:p1(x1, y1), p2(x2, y2) {}
void Object::Draw(HDC hdc) {}

//�� �׸���
class Circle : public Object {
public:
	Circle(int x1, int y1, int x2, int y2);
	void Draw(HDC hdc);
};
Circle::Circle(int x1, int y1, int x2, int y2)
	:Object(x1,y1,x2,y2) {}
void Circle::Draw(HDC hdc) {
	Ellipse(hdc, p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY());
}
Object* inform[200]; // ��ü ���� ������ �迭 ->��ü�� �ּҸ� �����ϴ� �迭
int count; // ���� ������ ���� 

void DrawAllObjects(HDC hdc) {
	Object* p;
	int i;

	for (i = 0; i < count; i++) {
		p = inform[i];
		p->Draw(hdc);
	}
}


//�� �׸��� 
class Line : public Object {
public:
	Line(int x1, int y1, int x2, int y2);
	void Draw(HDC hdc);
};
Line::Line(int x1, int y1, int x2, int y2)
	:Object(x1, y1, x2, y2) {}
void Line::Draw(HDC hdc) {
	MoveToEx(hdc, p1.GetX(), p1.GetY(), NULL);
	LineTo(hdc, p2.GetX(), p2.GetY()); 
}

//�簢�� �׸���

class Recta : public Object {
public:
	Recta(int x1, int y1, int x2, int y2);
	void Draw(HDC hdc);
};
Recta::Recta(int x1, int y1, int x2, int y2)
	:Object(x1, y1, x2, y2) {}
void Recta::Draw(HDC hdc) {
	Rectangle(hdc, p1.GetX(), p1.GetY(), p2.GetX(), p2.GetY());
}

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rectView;
	static int object_mode; // �������� 
	static int startX, startY, oldX, oldY; // ���� ��ǥ�� ���콺 �̵��ϱ� �� ��ǥ ����ϴ� ����  (static���� �����ϸ� 0���� �ڵ� �ʱ�ȭ)
	static int endX, endY; //���� ���콺 ��ǥ 
	static BOOL Drag; //���� ��ư ������ TRUE, ���� FALSE 
	//HPEN hPen, oldPen; //�� �׵θ� 
	//HBRUSH hBrush, oldBrush; //���� �귯�� 
	switch (iMsg)
	{
	case WM_CREATE: 
		object_mode = LINE;
		Drag = FALSE;
		count = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawAllObjects(hdc);
		//�迭�� ������ ��� ��ü�� �׸� 
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND: // �޴� msg ó�� 
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
		}
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
		hdc = GetDC(hwnd);
		if (Drag) {
			SetROP2(hdc, R2_NOTXORPEN);
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

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
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
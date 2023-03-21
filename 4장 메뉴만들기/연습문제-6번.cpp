// 색이 있는 직선과 원 그리기 
// 객체지향 방식으로 코드 구현 
#define LINE 1 // 메뉴에서 선택 도형을 변수 object_mode에 저장
#define ELLIPSE 2 // 그리는 부분에서 이 변수값을 보고 해당 도형 그림
#define RECTANGLE 3 // object_mode에 저장하는 값

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
	//private 지정하는 경우 public 함수 사용하면 자식에서 접근 가능 , but 코드 길어짐 
protected:
	Point p1, p2;
public:
	Object(int x1, int y1, int x2, int y2);
	virtual  void Draw(HDC hdc);
};
Object::Object(int x1, int y1, int x2, int y2)
	:p1(x1, y1), p2(x2, y2) {}
void Object::Draw(HDC hdc) {}

//원 그리기
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
Object* inform[200]; // 객체 저장 포인터 배열 ->객체의 주소를 저장하는 배열
int count; // 도형 저장한 변수 

void DrawAllObjects(HDC hdc) {
	Object* p;
	int i;

	for (i = 0; i < count; i++) {
		p = inform[i];
		p->Draw(hdc);
	}
}


//선 그리기 
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

//사각형 그리기

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

	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// 커서 지정

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
	static int object_mode; // 도형변수 
	static int startX, startY, oldX, oldY; // 시작 좌표와 마우스 이동하기 전 좌표 기억하는 변수  (static으로 선언하면 0으로 자동 초기화)
	static int endX, endY; //현재 마우스 좌표 
	static BOOL Drag; //왼쪽 버튼 누르면 TRUE, 떼면 FALSE 
	//HPEN hPen, oldPen; //원 테두리 
	//HBRUSH hBrush, oldBrush; //원색 브러쉬 
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
		//배열에 저장한 모든 객체를 그림 
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND: // 메뉴 msg 처리 
		switch (LOWORD(wParam))
		{
		case ID_LINE: // 직선 그리기 메뉴
			object_mode = LINE; // 도형변수에 LINE 저장
			break;
		case ID_ELLIPSE: // 원 그리기 메뉴 
			object_mode = ELLIPSE; // 도형변수에 ELLIPSE 저장
			break;
		case ID_RECTANGLE: // 사각형 그리기 메뉴
			object_mode = RECTANGLE; // 도형변수에 RECTANGLE 저장
			break;
		}
		break;

	case WM_LBUTTONDOWN: // 버튼을 누르면 드래그 동작 시작 
		oldX = startX = LOWORD(lParam);
		oldY = startY = HIWORD(lParam);
		Drag = TRUE;
		break;

	case WM_LBUTTONUP: // 버튼을 놓으면 드래그 종료 
		Drag = FALSE;
		break;

	case WM_MOUSEMOVE: // 시작점에서 현재 점으로 도형 그림 
		hdc = GetDC(hwnd);
		if (Drag) {
			SetROP2(hdc, R2_NOTXORPEN);
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			switch (object_mode) {
			case LINE: // 직선
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, oldX, oldY); // 지우기 

				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, endX, endY);  // 그리기 
				break;
			case ELLIPSE: // 원
				Ellipse(hdc, startX, startY, oldX, oldY); // 지우기 
				Ellipse(hdc, startX, startY, endX, endY); // 그리기 
				break;
			case RECTANGLE: // 사각형 
				Rectangle(hdc, startX, startY, oldX, oldY); // 지우기
				Rectangle(hdc, startX, startY, endX, endY); // 그리기 
				break;
			}
			oldX = endX;
			oldY = endY; // 현 위치를 이전 지점으로 저장
		}
		ReleaseDC(hwnd, hdc);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
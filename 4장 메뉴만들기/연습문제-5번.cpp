// 색이 있는 직선과 원 그리기 
#define LINE 1 // 메뉴에서 선택 도형을 변수 object_mode에 저장
#define ELLIPSE 2 // 그리는 부분에서 이 변수값을 보고 해당 도형 그림
#define RECTANGLE 3 // object_mode에 저장하는 값

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

//색 메뉴 클릭시 색상대화상자 띠우고 
//사용자가 선택한 색 반환받는 부분을 함수로 구성하여 호출
//-> 인자로 이전 사용 색 필요 
//->취소버튼 선택 하면 사용하던 색 반환
COLORREF ColorSelection(HWND hwnd, COLORREF oldColor) { // 이전 사용하던 color
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
	static int object_mode; // 도형변수 
	static int startX, startY, oldX, oldY; // 시작 좌표와 마우스 이동하기 전 좌표 기억하는 변수  (static으로 선언하면 0으로 자동 초기화)
	static int endX, endY; //현재 마우스 좌표 
	static BOOL Drag; //왼쪽 버튼 누르면 TRUE, 떼면 FALSE 
	HPEN hPen, oldPen; //원 테두리 
	HBRUSH hBrush, oldBrush; //원색 브러쉬 
	static COLORREF brushcolor = RGB(255, 255, 255);
	static COLORREF pencolor = RGB(0,0,0);
	switch (iMsg)
	{
	case WM_CREATE: //펜 색 지정, 붓 색 생성 펜/붓 변수에 저장, 기타 변수 초기화
		hdc = GetDC(hwnd);
		hPen = CreatePen(PS_SOLID, 1 , pencolor); // 펜 색을 검은색으로 지정 
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(brushcolor); // 붓 색을 흰색으로 지정  
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush); 
		startX = oldX = 0, startY = oldY = 0; // 시작점 (0,0)
		object_mode = 0;  
		Drag = FALSE; 

		SelectObject(hdc, oldPen);
		SelectObject(hdc, oldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);

		ReleaseDC(hwnd, hdc);
		break;

	case WM_COMMAND: // 메뉴 msg 처리 
		hdc = GetDC(hwnd);
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
		case ID_PENCOLOR: // 선 색상 바꾸기 메뉴
			pencolor = ColorSelection(hwnd, pencolor);
			break;
		case ID_FACECOLOR: // 면 색상 바꾸기 메뉴 
			brushcolor = ColorSelection(hwnd, brushcolor);
			break;
		}
		ReleaseDC(hwnd, hdc);
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
			ReleaseDC(hwnd, hdc);
		}
		break;  

	case WM_DESTROY: 
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
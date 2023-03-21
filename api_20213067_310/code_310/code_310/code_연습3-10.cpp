#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	hwnd;
	MSG 	msg;
	WNDCLASS	WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU310);
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



#define ITEMNUM 10 // 아이템 개수 
// 전역변수
int board[22][22]; // 게임판(사각형) 배열 
int wormX[30], wormY[30]; // 지렁이 x,y좌표 
int xDirect, yDirect, len; // 한 번 이동할 때 필요한 x,y변화량 , 지렁이 길이 


void ItemGenerator() // 아이템 무작위 배열하는 함수  
{
	int i, x, y; // 반복할 때 필요한 변수 i
				 // x, y좌표 변수 
	for (i = 0; i < ITEMNUM; i++) // 10번동안 반복 
	{
		// x, y좌표를 랜덤 값으로 지정하여 아이템을 무작위로 놔둠 
		x = rand() % 20 + 1; // 랜덤한 x값
		y = rand() % 20 + 1; // 랜덤한 y값 
		if (board[y][x] == 0) // board[y][x] == 0일 때 (게임판 안에서 원이 그려질 때)
			board[y][x] = 2; // board[y][x] = 2로 잡고 DrawGameBoard()에서 case 2 조건문을 실행함 
		else // 게임판 안이 아닌 다른 곳에서 원이 그려질 때 
		{
			i = i - 1; // 잘 못 그린것으로 아이템을 반복할 때 필요한 변수인 i를 'i = i - 1'로 i값 1감소 
			continue; // 다시 반복문으로 돌아간다
		}
	}
	return; // 현재 있는 함수에서 빠져나가며, 그 함수를 호출했던 곳으로 되돌아 가라
}


void DrawGameBoard(HDC hdc) // 게임판 그리는 함수 
{
	TCHAR str[20]; // 현재 점수 출력할 때 필요한 변수 
	RECT rt = { 400,200,600,300 };
	int i, x, y; // 반복할 때 필요한 변수 i
				 // x, y좌표 변수 
	
	for (y = 0; y < 22; y++) { // 세로축(y) 크기 만큼 반복 
		for (x = 0; x < 22; x++) // 가로축(x) 크기 만큼 반복
		{
			switch (board[y][x])
			{
			case -1: // board[y][x] == -1 일 때
				Rectangle(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); // 게임판의 사각형 그리기 
				break; // switch문 끝냄
			case 2: // 아이템을 먹었을 때 
				SelectObject(hdc, GetStockObject(BLACK_BRUSH)); // 아이템 먹기 전 원의 색깔은 검은색
				Ellipse(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); // 원 그리기 
				SelectObject(hdc, GetStockObject(WHITE_BRUSH)); // 먹은 후 원이 사라져야함 -> 면의 색상 흰색으로 변경 
				break; // switch문 끝냄 
			}
		}
	}

	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); // 지렁이 머리 - 빨간색 2px인 직선
	Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); // 빨간 원 그림
	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); // 지렁이 꼬리 - 파란색 2px인 직선 

	for (i = 1; i < len; i++) // len만큼 반복 
		Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //  아이템을 먹으면 -> 꼬리 하나씩 추가
	//아이템(검은색 원)을 하나 먹을 때 마다 점수 1증가함 (아이템 총 10개 이므로 아이템 하나당 10점)
	wsprintf(str, TEXT("현재 점수는 %d 점입니다."), (len-2)*10); // 화면에 현재 점수를 출력하는 함수
	TextOut(hdc, 450, 200, str, _tcslen(str)); // (450,200)에 문자열 출력
	if (len == 12) { // 아이템을 다 먹었을 때
		TextOut(hdc, 450, 220, _T("축하합니다 성공!!!"), _tcslen(_T("축하합니다 성공!!!"))); // 축하합니다 성공!!! 문자열 화면에 출력 
	}
}


void GameInit() // 지렁이 게임 기본화면 함수 
{
	int i; // for문에 필요한 변수 
	for (i = 0; i < 22; i++) // 사각형 게임판 만들기 DrawGameBoard(hdc)함수 이용 
	{
		// 모두 board[x][y] == -1 이므로 DrawGameBoard(hdc) 함수의 case -1 조건문을 실행함 
		board[i][0] = -1; // 왼쪽에 사각형을 그림
		board[i][21] = -1; // 오른쪽에 사각형을 그림
		board[0][i] = -1; // 위쪽에 사각형을 그림
		board[21][i] = -1; // 아래쪽에 사각형을 그림 
	}
	wormX[0] = 2; wormY[0] = 1; //지렁이 머리 좌표
	wormX[1] = 1; wormY[1] = 1; // 지렁이 꼬리 좌표 
	board[wormY[1]][wormX[0]] = 3; // board[1][2] = 3
	board[wormY[1]][wormX[1]] = 3; // board[1][1] = 3

	ItemGenerator(); // 아이템 무작위 배열하는 함수 호출 -> 아이템을 보여줌 

	len = 2; // 기본 지렁이 길이 =  2 (왜냐면 머리와 꼬리만 존재하므로) 
	xDirect = 1; yDirect = 0; // 머리와 꼬리의 x,y변화량 1
}


void DirectControl(int DirectKey) // 방향키대로 지렁이 이동하는 함수 
{
	switch (DirectKey) {
	case VK_LEFT: // 왼쪽 방향키를 누른 경우
		if (xDirect == 1) // 왼쪽 사각형 경계 밖으로 지렁이가 이동하려고 할 때

			break; // 지렁이가 이동 안함 -> if문 빠져나옴 
		if (board[wormY[0]][wormX[0] - 1] != -1) // 왼쪽 사각형 경계 안에서 지렁이가 이동할 때
		{
			//왼쪽으로 한칸 이동 
			xDirect = -1; // x축 1 감소
			yDirect = 0; // y축 변화없음 
		}
		break; // case문 빠져나옴 
	case VK_RIGHT: // 오른쪽 방향키를 누른 경우
		if (xDirect == -1) // 오른쪽 사각형 경계 밖으로 지렁이가 이동하려고 할 때
			break; // 지렁이가 이동 안함 -> if문 빠져나옴
		if (board[wormY[0]][wormX[0] + 1] != -1) // 오른쪽 사각형 경계 안에서 지렁이가 이동할 때
		{
			// 오른쪽으로 한칸 이동
			xDirect = 1; // x축 1 증가
			yDirect = 0; // y축 변화없음
		}
		break; // case문 빠져나옴
	case VK_UP: // 위쪽 방향키를 누른 경우
		if (yDirect == 1) // 위쪽 사각형 경계 밖으로 지렁이가 이동하려고 할 때
			break; // 지렁이가 이동 안함 -> if문 빠져나옴
		if (board[wormY[0] - 1][wormX[0]] != -1) // 위쪽 사각형 경계 안에서 지렁이가 이동할 때
		{
			// 위쪽으로 한칸 이동
			xDirect = 0; // x축 변화없음
			yDirect = -1; // y축 1 감소
		}
		break; // case문 빠져나옴
	case VK_DOWN: // 아래쪽 방향키를 누른 경우
		if (yDirect == -1) // 아래쪽 사각형 경계 밖으로 지렁이가 이동하려고 할 때
			break; // 지렁이가 이동 안함 -> if문 빠져나옴
		if (board[wormY[0] + 1][wormX[0]] != -1)  // 아래쪽 사각형 경계 안에서 지렁이가 이동할 때
		{
			// 아래쪽으로 한칸 이동
			xDirect = 0; // x축 변화없음
			yDirect = 1; // y축 1 증가 
		}
		break; // case문 빠져나옴
	}
}


void MovingWorm() // 매 초마다 일정하게 지렁이가 이동하는 함수  
{
	int tmpx, tmpy, i; // x변화량, y변화량, 반복문에 사용하는 변수 i

	tmpx = wormX[0]; // 처음 x변화량은 지렁이 머리의 x좌표와 동일 (초기화)
	tmpy = wormY[0]; // 처음 y변화량은 지렁이 머리의 y좌표와 동일 (초기화)
	wormX[0] += xDirect; wormY[0] += yDirect; // 이동할 때 마다 지렁이의 x,y좌표가 일정하게 이동

	if (board[wormY[0]][wormX[0]] == -1 || board[wormY[0]][wormX[0]] == 3) // 사각형 경계선 부분 이거나 경계선 밖일 때
	{
		//초기 변화량 값 
		wormX[0] = tmpx;
		wormY[0] = tmpy;
	}
	else // 게임판 안에서 움직일 때 
	{
		if (board[wormY[0]][wormX[0]] == 2) // 아이템을 먹었을 경우 
		{
			// DrawGameBoard 함수의 case 2 조건문 실행 -> 지렁이 꼬리 하나 추가 
			len = len + 1; // 지렁이 길이 1 증가 
			board[wormY[0]][wormX[0]] = 0; // 게임판 안에서 원이 그려진다. (꼬리가 1개 증가한 원이 그려짐)
		}
		else // 아이템을 먹지 않았을 경우 
			board[wormY[len - 1]][wormX[len - 1]] = 0; // 게임판 안에서 원이 그려진다. (바로 전의 지렁이 상태 유지)

		for (i = len - 1; i > 1; i--) // 지렁이 꼬리 길이 만큼 반복
		{
			wormX[i] = wormX[i - 1]; // 현재 지렁이의 x 좌표가 이전의 x 좌표가 됨
			wormY[i] = wormY[i - 1]; // 현재 지렁이의 y 좌표가 이전의 y 좌표가 됨 
		}
		wormX[1] = tmpx; // 지렁이 첫번째 꼬리의 x좌표는 지렁이 머리의 x좌표가 됨
		wormY[1] = tmpy; // 지렁이 첫번째 꼬리의 y좌표는 지렁이 머리의 y좌표가 됨 
		board[wormY[0]][wormX[0]] = 3; 
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// 디바이스 콘텍스트 저장할 변수 hdc와 출력 영역에 대한 상세 정보 저장할 ps변수선언
	HDC hdc; 
	PAINTSTRUCT ps;
	int start; // menu 시작 버튼 눌렀을 때 필요한 변수 
	int exit; // menu 종료하기 버튼 눌렀을 때 필요한 변수
	switch (iMsg) {
	case WM_CREATE: // 화면 생성할 때 
		GameInit(); // 지렁이 게임 기본화면 함수
		return 0; // case WM_CREATE문 빠져나옴

	case WM_COMMAND: // 메뉴 항목 선택 시 발생하는 WM_COMMAND 메시지 처리
		switch (LOWORD(wParam)) { // 어떤 메뉴 항목이 선택되었는지 판단하는 조건문 
		case ID_START: // menu의 "시작하기" 눌렀을 경우
			start = MessageBox(hwnd, _T("게임을 시작하겠습니까?"), _T("게임시작"), MB_YESNO);
			if (start == IDYES) { // <예> 버튼을 누르면 start 값이 IDYES와 같음
				SetTimer(hwnd, 1, 100, NULL); // 1번 타이머 0.1초 간격으로 실행 
			}
			break; // case ID_START문 빠져나옴
		case ID_EXIT: // menu의 "종료하기" 눌렀을 경우 
			exit = MessageBox(hwnd, _T("게임을 종료하겠습니까?"), _T("게임종료"), MB_YESNO);
			if (exit == IDYES) { // <예> 버튼을 누르면 exit 값이 IDYES와 같음  
				PostQuitMessage(0); // 프로그램 종료
			}
			break; // case ID_EXIT문 빠져나옴 
		}
		break;
	case WM_PAINT: // 화면에 그리기 
		hdc = BeginPaint(hwnd, &ps); // 출력할 영역인 디바이스 콘텍스트를 BeginPaint()함수를 이용해 얻어온다.
		DrawGameBoard(hdc); // 게임판 그리는 함수 
		EndPaint(hwnd, &ps); // 출력을 마친 후 디바이스 콘텍스트 반환함
		break; // case WM_PAINT문 빠져나옴

	case WM_KEYDOWN: // 방향키를 눌렀을 때 
		DirectControl((int)wParam); // 방향키대로 지렁이 이동하는 함수 
		if (len == 12) { // 아이템을 다 먹었을 때 
			KillTimer(hwnd, 1); // 지렁이 자동이동을 멈춤
		}
		break; // case WM_KEYDOWN문 빠져나옴

	case WM_TIMER: // 자동으로 이동시키는 타이머 메시지 
		MovingWorm(); // 매 초마다 일정하게 지렁이가 이동하는 함수
		InvalidateRgn(hwnd, NULL, TRUE); // 화면을 지우고 WM_PAINT 메시지 발생시킴
		break; // case WM_TIMER문 빠져나옴 

	case WM_DESTROY: // 화면 종료할 때 사용
		KillTimer(hwnd, 1); // 1번 타이머 동작 멈춤
		PostQuitMessage(0); // 프로그램 종료 
		return 0; //case WM_DESTROY문 빠져나옴 
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
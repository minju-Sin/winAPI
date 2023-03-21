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



#define ITEMNUM 10 // ������ ���� 
// ��������
int board[22][22]; // ������(�簢��) �迭 
int wormX[30], wormY[30]; // ������ x,y��ǥ 
int xDirect, yDirect, len; // �� �� �̵��� �� �ʿ��� x,y��ȭ�� , ������ ���� 


void ItemGenerator() // ������ ������ �迭�ϴ� �Լ�  
{
	int i, x, y; // �ݺ��� �� �ʿ��� ���� i
				 // x, y��ǥ ���� 
	for (i = 0; i < ITEMNUM; i++) // 10������ �ݺ� 
	{
		// x, y��ǥ�� ���� ������ �����Ͽ� �������� �������� ���� 
		x = rand() % 20 + 1; // ������ x��
		y = rand() % 20 + 1; // ������ y�� 
		if (board[y][x] == 0) // board[y][x] == 0�� �� (������ �ȿ��� ���� �׷��� ��)
			board[y][x] = 2; // board[y][x] = 2�� ��� DrawGameBoard()���� case 2 ���ǹ��� ������ 
		else // ������ ���� �ƴ� �ٸ� ������ ���� �׷��� �� 
		{
			i = i - 1; // �� �� �׸������� �������� �ݺ��� �� �ʿ��� ������ i�� 'i = i - 1'�� i�� 1���� 
			continue; // �ٽ� �ݺ������� ���ư���
		}
	}
	return; // ���� �ִ� �Լ����� ����������, �� �Լ��� ȣ���ߴ� ������ �ǵ��� ����
}


void DrawGameBoard(HDC hdc) // ������ �׸��� �Լ� 
{
	TCHAR str[20]; // ���� ���� ����� �� �ʿ��� ���� 
	RECT rt = { 400,200,600,300 };
	int i, x, y; // �ݺ��� �� �ʿ��� ���� i
				 // x, y��ǥ ���� 
	
	for (y = 0; y < 22; y++) { // ������(y) ũ�� ��ŭ �ݺ� 
		for (x = 0; x < 22; x++) // ������(x) ũ�� ��ŭ �ݺ�
		{
			switch (board[y][x])
			{
			case -1: // board[y][x] == -1 �� ��
				Rectangle(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); // �������� �簢�� �׸��� 
				break; // switch�� ����
			case 2: // �������� �Ծ��� �� 
				SelectObject(hdc, GetStockObject(BLACK_BRUSH)); // ������ �Ա� �� ���� ������ ������
				Ellipse(hdc, x * 20, y * 20, (x + 1) * 20, (y + 1) * 20); // �� �׸��� 
				SelectObject(hdc, GetStockObject(WHITE_BRUSH)); // ���� �� ���� ��������� -> ���� ���� ������� ���� 
				break; // switch�� ���� 
			}
		}
	}

	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(255, 0, 0))); // ������ �Ӹ� - ������ 2px�� ����
	Ellipse(hdc, wormX[0] * 20, wormY[0] * 20, (wormX[0] + 1) * 20, (wormY[0] + 1) * 20); // ���� �� �׸�
	SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255))); // ������ ���� - �Ķ��� 2px�� ���� 

	for (i = 1; i < len; i++) // len��ŭ �ݺ� 
		Ellipse(hdc, wormX[i] * 20, wormY[i] * 20, (wormX[i] + 1) * 20, (wormY[i] + 1) * 20); //  �������� ������ -> ���� �ϳ��� �߰�
	//������(������ ��)�� �ϳ� ���� �� ���� ���� 1������ (������ �� 10�� �̹Ƿ� ������ �ϳ��� 10��)
	wsprintf(str, TEXT("���� ������ %d ���Դϴ�."), (len-2)*10); // ȭ�鿡 ���� ������ ����ϴ� �Լ�
	TextOut(hdc, 450, 200, str, _tcslen(str)); // (450,200)�� ���ڿ� ���
	if (len == 12) { // �������� �� �Ծ��� ��
		TextOut(hdc, 450, 220, _T("�����մϴ� ����!!!"), _tcslen(_T("�����մϴ� ����!!!"))); // �����մϴ� ����!!! ���ڿ� ȭ�鿡 ��� 
	}
}


void GameInit() // ������ ���� �⺻ȭ�� �Լ� 
{
	int i; // for���� �ʿ��� ���� 
	for (i = 0; i < 22; i++) // �簢�� ������ ����� DrawGameBoard(hdc)�Լ� �̿� 
	{
		// ��� board[x][y] == -1 �̹Ƿ� DrawGameBoard(hdc) �Լ��� case -1 ���ǹ��� ������ 
		board[i][0] = -1; // ���ʿ� �簢���� �׸�
		board[i][21] = -1; // �����ʿ� �簢���� �׸�
		board[0][i] = -1; // ���ʿ� �簢���� �׸�
		board[21][i] = -1; // �Ʒ��ʿ� �簢���� �׸� 
	}
	wormX[0] = 2; wormY[0] = 1; //������ �Ӹ� ��ǥ
	wormX[1] = 1; wormY[1] = 1; // ������ ���� ��ǥ 
	board[wormY[1]][wormX[0]] = 3; // board[1][2] = 3
	board[wormY[1]][wormX[1]] = 3; // board[1][1] = 3

	ItemGenerator(); // ������ ������ �迭�ϴ� �Լ� ȣ�� -> �������� ������ 

	len = 2; // �⺻ ������ ���� =  2 (�ֳĸ� �Ӹ��� ������ �����ϹǷ�) 
	xDirect = 1; yDirect = 0; // �Ӹ��� ������ x,y��ȭ�� 1
}


void DirectControl(int DirectKey) // ����Ű��� ������ �̵��ϴ� �Լ� 
{
	switch (DirectKey) {
	case VK_LEFT: // ���� ����Ű�� ���� ���
		if (xDirect == 1) // ���� �簢�� ��� ������ �����̰� �̵��Ϸ��� �� ��

			break; // �����̰� �̵� ���� -> if�� �������� 
		if (board[wormY[0]][wormX[0] - 1] != -1) // ���� �簢�� ��� �ȿ��� �����̰� �̵��� ��
		{
			//�������� ��ĭ �̵� 
			xDirect = -1; // x�� 1 ����
			yDirect = 0; // y�� ��ȭ���� 
		}
		break; // case�� �������� 
	case VK_RIGHT: // ������ ����Ű�� ���� ���
		if (xDirect == -1) // ������ �簢�� ��� ������ �����̰� �̵��Ϸ��� �� ��
			break; // �����̰� �̵� ���� -> if�� ��������
		if (board[wormY[0]][wormX[0] + 1] != -1) // ������ �簢�� ��� �ȿ��� �����̰� �̵��� ��
		{
			// ���������� ��ĭ �̵�
			xDirect = 1; // x�� 1 ����
			yDirect = 0; // y�� ��ȭ����
		}
		break; // case�� ��������
	case VK_UP: // ���� ����Ű�� ���� ���
		if (yDirect == 1) // ���� �簢�� ��� ������ �����̰� �̵��Ϸ��� �� ��
			break; // �����̰� �̵� ���� -> if�� ��������
		if (board[wormY[0] - 1][wormX[0]] != -1) // ���� �簢�� ��� �ȿ��� �����̰� �̵��� ��
		{
			// �������� ��ĭ �̵�
			xDirect = 0; // x�� ��ȭ����
			yDirect = -1; // y�� 1 ����
		}
		break; // case�� ��������
	case VK_DOWN: // �Ʒ��� ����Ű�� ���� ���
		if (yDirect == -1) // �Ʒ��� �簢�� ��� ������ �����̰� �̵��Ϸ��� �� ��
			break; // �����̰� �̵� ���� -> if�� ��������
		if (board[wormY[0] + 1][wormX[0]] != -1)  // �Ʒ��� �簢�� ��� �ȿ��� �����̰� �̵��� ��
		{
			// �Ʒ������� ��ĭ �̵�
			xDirect = 0; // x�� ��ȭ����
			yDirect = 1; // y�� 1 ���� 
		}
		break; // case�� ��������
	}
}


void MovingWorm() // �� �ʸ��� �����ϰ� �����̰� �̵��ϴ� �Լ�  
{
	int tmpx, tmpy, i; // x��ȭ��, y��ȭ��, �ݺ����� ����ϴ� ���� i

	tmpx = wormX[0]; // ó�� x��ȭ���� ������ �Ӹ��� x��ǥ�� ���� (�ʱ�ȭ)
	tmpy = wormY[0]; // ó�� y��ȭ���� ������ �Ӹ��� y��ǥ�� ���� (�ʱ�ȭ)
	wormX[0] += xDirect; wormY[0] += yDirect; // �̵��� �� ���� �������� x,y��ǥ�� �����ϰ� �̵�

	if (board[wormY[0]][wormX[0]] == -1 || board[wormY[0]][wormX[0]] == 3) // �簢�� ��輱 �κ� �̰ų� ��輱 ���� ��
	{
		//�ʱ� ��ȭ�� �� 
		wormX[0] = tmpx;
		wormY[0] = tmpy;
	}
	else // ������ �ȿ��� ������ �� 
	{
		if (board[wormY[0]][wormX[0]] == 2) // �������� �Ծ��� ��� 
		{
			// DrawGameBoard �Լ��� case 2 ���ǹ� ���� -> ������ ���� �ϳ� �߰� 
			len = len + 1; // ������ ���� 1 ���� 
			board[wormY[0]][wormX[0]] = 0; // ������ �ȿ��� ���� �׷�����. (������ 1�� ������ ���� �׷���)
		}
		else // �������� ���� �ʾ��� ��� 
			board[wormY[len - 1]][wormX[len - 1]] = 0; // ������ �ȿ��� ���� �׷�����. (�ٷ� ���� ������ ���� ����)

		for (i = len - 1; i > 1; i--) // ������ ���� ���� ��ŭ �ݺ�
		{
			wormX[i] = wormX[i - 1]; // ���� �������� x ��ǥ�� ������ x ��ǥ�� ��
			wormY[i] = wormY[i - 1]; // ���� �������� y ��ǥ�� ������ y ��ǥ�� �� 
		}
		wormX[1] = tmpx; // ������ ù��° ������ x��ǥ�� ������ �Ӹ��� x��ǥ�� ��
		wormY[1] = tmpy; // ������ ù��° ������ y��ǥ�� ������ �Ӹ��� y��ǥ�� �� 
		board[wormY[0]][wormX[0]] = 3; 
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// ����̽� ���ؽ�Ʈ ������ ���� hdc�� ��� ������ ���� �� ���� ������ ps��������
	HDC hdc; 
	PAINTSTRUCT ps;
	int start; // menu ���� ��ư ������ �� �ʿ��� ���� 
	int exit; // menu �����ϱ� ��ư ������ �� �ʿ��� ����
	switch (iMsg) {
	case WM_CREATE: // ȭ�� ������ �� 
		GameInit(); // ������ ���� �⺻ȭ�� �Լ�
		return 0; // case WM_CREATE�� ��������

	case WM_COMMAND: // �޴� �׸� ���� �� �߻��ϴ� WM_COMMAND �޽��� ó��
		switch (LOWORD(wParam)) { // � �޴� �׸��� ���õǾ����� �Ǵ��ϴ� ���ǹ� 
		case ID_START: // menu�� "�����ϱ�" ������ ���
			start = MessageBox(hwnd, _T("������ �����ϰڽ��ϱ�?"), _T("���ӽ���"), MB_YESNO);
			if (start == IDYES) { // <��> ��ư�� ������ start ���� IDYES�� ����
				SetTimer(hwnd, 1, 100, NULL); // 1�� Ÿ�̸� 0.1�� �������� ���� 
			}
			break; // case ID_START�� ��������
		case ID_EXIT: // menu�� "�����ϱ�" ������ ��� 
			exit = MessageBox(hwnd, _T("������ �����ϰڽ��ϱ�?"), _T("��������"), MB_YESNO);
			if (exit == IDYES) { // <��> ��ư�� ������ exit ���� IDYES�� ����  
				PostQuitMessage(0); // ���α׷� ����
			}
			break; // case ID_EXIT�� �������� 
		}
		break;
	case WM_PAINT: // ȭ�鿡 �׸��� 
		hdc = BeginPaint(hwnd, &ps); // ����� ������ ����̽� ���ؽ�Ʈ�� BeginPaint()�Լ��� �̿��� ���´�.
		DrawGameBoard(hdc); // ������ �׸��� �Լ� 
		EndPaint(hwnd, &ps); // ����� ��ģ �� ����̽� ���ؽ�Ʈ ��ȯ��
		break; // case WM_PAINT�� ��������

	case WM_KEYDOWN: // ����Ű�� ������ �� 
		DirectControl((int)wParam); // ����Ű��� ������ �̵��ϴ� �Լ� 
		if (len == 12) { // �������� �� �Ծ��� �� 
			KillTimer(hwnd, 1); // ������ �ڵ��̵��� ����
		}
		break; // case WM_KEYDOWN�� ��������

	case WM_TIMER: // �ڵ����� �̵���Ű�� Ÿ�̸� �޽��� 
		MovingWorm(); // �� �ʸ��� �����ϰ� �����̰� �̵��ϴ� �Լ�
		InvalidateRgn(hwnd, NULL, TRUE); // ȭ���� ����� WM_PAINT �޽��� �߻���Ŵ
		break; // case WM_TIMER�� �������� 

	case WM_DESTROY: // ȭ�� ������ �� ���
		KillTimer(hwnd, 1); // 1�� Ÿ�̸� ���� ����
		PostQuitMessage(0); // ���α׷� ���� 
		return 0; //case WM_DESTROY�� �������� 
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
// ����Ʈ�ڽ��� ���ڿ� �����ϱ�
#include <windows.h>
#include <TCHAR.h>
#include "resource.h"
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
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  	// Ŀ�� ����

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

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_64:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG6_4), hwnd, DlgPunc);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK DlgPunc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	TCHAR word[100]; // ���� �Է� 

	switch (iMsg) {
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_COPY: // �����ϱ�
			GetDlgItemText(hDlg, IDC_EDIT_SOURCE, word, 100); // ���ڿ� ���� 
			SetDlgItemText(hDlg, IDC_EDIT_COPY, word); 
			break;
			
		case IDC_BUTTON_EDIT: // �����ϱ�
			SetDlgItemText(hDlg, IDC_EDIT_SOURCE, _T(" ")); // ��ĭ ���ڿ� ����
			SetDlgItemText(hDlg, IDC_EDIT_COPY, _T(""));
			break;

		case IDC_BUTTON_END: // �����ϱ�
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0; //������ �ۼ�
}

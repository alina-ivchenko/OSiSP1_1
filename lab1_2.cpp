// Hello2.cpp
#include <windows.h>
#include <zmouse.h>
#define TIMER 1
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
const int winHeigh = 500;
const int winWidth = 700;
const byte STEP = 5;
const byte COUNT = 8;
int coordX = 5;
int coordY = 5;
//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int
	nCmdShow)
{
	HWND hMainWnd;
	char szClassName[] = "MyClass";
	MSG msg;
	WNDCLASSEX wc;
	
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Cannot register class", "Error", MB_OK);
		return 0;
	}


	
	hMainWnd = CreateWindow(
		szClassName, "Circle", WS_MAXIMIZE | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, 0, winWidth, winHeigh,
		(HWND)NULL, (HMENU)NULL,
		(HINSTANCE)hInstance, NULL
		);
	if (!hMainWnd) {
		MessageBox(NULL, "Cannot create main window", "Error", MB_OK);
		return 0;
	}
	
	ShowWindow(hMainWnd, nCmdShow);
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
//====================================================================
bool IsLRBorder(RECT rect, POINT pt[COUNT])
{
	for (int i = 0; i < COUNT; i++)
	{
		if (pt[i].x<rect.left || pt[i].x>rect.right)
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool IsUDBorder(RECT rect, POINT pt[COUNT])
{
	for (int i = 0; i < COUNT; i++)
	{
		if (pt[i].y< rect.top || pt[i].y>rect.bottom)
		{
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	GetClientRect(hWnd, &rect);
	COLORREF color;
	HPEN hPen;
	HBRUSH hBrush;
	static POINT pt[COUNT];
	
	switch (uMsg)
	{
	case WM_CREATE:
		pt[0].x = 20;
		pt[0].y = 140;
		pt[1].x = 60;
		pt[1].y = 140;
		pt[2].x = 80;
		pt[2].y = 160;
		pt[3].x = 80;
		pt[3].y = 200;
		pt[4].x = 60;
		pt[4].y = 220;
		pt[5].x = 20;
		pt[5].y = 220;
		pt[6].x = 0;
		pt[6].y = 200;
		pt[7].x = 0;
		pt[7].y = 160;
		SetTimer(hWnd, TIMER, 50, NULL);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		color = 0x00616527;
		hPen = CreatePen(PS_INSIDEFRAME, 10, color);
		hBrush = CreateSolidBrush(RGB(255, 0, 67)); //задаём сплошную кисть, закрашенную цветом RGB
		SelectObject(hDC, hPen);
		SelectObject(hDC, hBrush); //делаем кисть активной
		Polygon(hDC, pt, COUNT);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (wParam == TIMER)
		{
			for (int i = 0; i<COUNT; i++)
			{
				pt[i].x += coordX;
				pt[i].y += coordY;
			}
			if (IsLRBorder(rect, pt))
			{
				coordX *= -1;
			}
			if (IsUDBorder(rect, pt))
			{
				coordY *= -1;
			}
			InvalidateRect(NULL, NULL, TRUE);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

// Hello1.cpp
#include <windows.h>
#include <zmouse.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
const int winHeigh = 500;
const int winWidth = 700;
const byte STEP = 5;
const int figureSizeDiv2 = 50;
int figureXLeft = winWidth / 2 - figureSizeDiv2;
int figureXRight = winWidth / 2 + figureSizeDiv2;
int figureYTop = winHeigh / 2 - figureSizeDiv2;
int figureYBottom = winHeigh / 2 + figureSizeDiv2;
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
		szClassName, "Circle", /*WS_OVERLAPPEDWINDOW*/WS_MAXIMIZE | WS_MINIMIZEBOX | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, 0, /*CW_USEDEFAULT, 0,*/ winWidth, winHeigh,
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
void CircleDraw(HWND hWnd, HDC *hDC, PAINTSTRUCT *ps, RECT rect, HPEN hPen, HBRUSH hBrush, int figureXLeft, int figureYTop, int figureXRight, int figureYBottom)
{
	*hDC = BeginPaint(hWnd, ps);
	GetClientRect(hWnd, &rect);
	SelectObject(*hDC, hPen);
	SelectObject(*hDC, hBrush); //делаем кисть активной

	//BOOL Ellipse(HDC hdc, int xLeft, int yTop, int xRight, int yBottom);
	Ellipse(*hDC, figureXLeft, figureYTop, figureXRight, figureYBottom);
	EndPaint(hWnd, ps);
}

void ChangeCoord(int *coordTopOrLeft, int *coordBottomOrRight, int STEP, int flag)
{
	*coordTopOrLeft += STEP*flag;
	*coordBottomOrRight += STEP*flag;
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
	int wheelDelta;

	color = 0x00616527;
	hPen = CreatePen(PS_INSIDEFRAME, 20, color);
	hBrush = CreateSolidBrush(RGB(255, 0, 67)); //задаём сплошную кисть, закрашенную цветом RGB
	
	switch (uMsg)
	{
	case WM_PAINT:
		CircleDraw(hWnd, &hDC, &ps, rect, hPen, hBrush, figureXLeft, figureYTop, figureXRight, figureYBottom);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if ((figureXLeft - STEP) >= rect.left)
			{
				ChangeCoord(&figureXLeft, &figureXRight, STEP, -1);
			}
			break;
		case VK_RIGHT:
			if ((figureXRight + STEP) <= rect.right)
			{
				ChangeCoord(&figureXLeft, &figureXRight, STEP, 1);
			}
			break;
		case VK_UP:
			if ((figureYTop - STEP) >= rect.top)
			{
				ChangeCoord(&figureYTop, &figureYBottom, STEP, -1);
			}
			break;
		case VK_DOWN:
			if ((figureYBottom + STEP) <= rect.bottom)
			{
				ChangeCoord(&figureYTop, &figureYBottom, STEP, 1);
			}
			break;
		default:
			break;
		}
		InvalidateRect(hWnd, 0, 1);
		//CircleDraw(hWnd, &hDC, &ps, rect, hPen, hBrush, figureXLeft, figureYTop, figureXRight, figureYBottom);
		break;
	case WM_MOUSEWHEEL:
		wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
		if (wParam & MK_SHIFT)
		{
			if (wheelDelta > 0)
			{
				while (((figureXLeft - STEP) >= rect.left) && (wheelDelta > 0))
				{
					ChangeCoord(&figureXLeft, &figureXRight, STEP, -1);
					InvalidateRect(hWnd, 0, 1);
					wheelDelta--;
				}
			}
			else
			{
				while (((figureXRight + STEP) <= rect.right) && (wheelDelta < 0))
				{
					ChangeCoord(&figureXLeft, &figureXRight, STEP, 1);
					InvalidateRect(hWnd, 0, 1);
					wheelDelta++;
				}
			}
		}
		else
		{
			if (wheelDelta > 0)
			{
				while (((figureYTop - STEP) >= rect.top) && (wheelDelta > 0))
				{
					ChangeCoord(&figureYTop, &figureYBottom, STEP, -1);
					InvalidateRect(hWnd, 0, 1);
					wheelDelta--;
				}
			}
			else
			{
				while (((figureYBottom + STEP) <= rect.bottom) && ((wheelDelta < 0)))
				{
					ChangeCoord(&figureYTop, &figureYBottom, STEP, 1);
					InvalidateRect(hWnd, 0, 1);
					wheelDelta++;
				}
			}
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

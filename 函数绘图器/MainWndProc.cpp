#include <windows.h>
#include "resource.h"
#include "def.h"
#include "graph.h"
#include "NewDlgProc.h"
#include "ListDlgProc.h"
#include "JumpDlgProc.h"
#include "OptionDlgProc.h"
#include "HelpDlgProc.h"
#include "AboutDlgProc.h"

extern HINSTANCE hInst;
extern int cxClient, cyClient;
extern FUNC function[1024];
extern int cFunc;
extern double division[7];
extern int size_g[5];
extern int unit;
extern int space;
extern int xOrigin, yOrigin;
extern COLORREF cAxis, cGrid, cText, cBackground;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*图标*/
	static HICON hIcon_ZoomIn;
	static HICON hIcon_ZoomOut;
	static HICON hIcon_Return;

	/*内存DC*/
	static HDC hdcMem = NULL;
	HDC hdcWnd;
	HBITMAP hBitmap;
	static RECT rcClient;

	/*鼠标拖动*/
	static int dx, dy;

	/*鼠标点击*/
	double t;

	/*对话框应用更改*/
	int ret;

	switch (message)
	{
	case WM_CREATE:
		/*加载图标*/
		hIcon_ZoomIn = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ZOOMIN));
		hIcon_ZoomOut = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ZOOMOUT));
		hIcon_Return = LoadIcon(hInst, MAKEINTRESOURCE(IDI_RETURN));
		/*初始化相关数据*/
		cFunc = 0;
		unit = 1;
		space = 3;
		cAxis = RGB(0, 0, 0);
		cGrid = RGB(196, 195, 191);
		cText = RGB(0, 0, 0);
		ZeroMemory(function, sizeof(function));
		function[0].cGraph = RGB(0, 162, 0);
		cBackground = RGB(255, 255, 255);
		GetClientRect(hwnd, &rcClient);
		xOrigin = (rcClient.right - rcClient.left) / 2;
		yOrigin = (rcClient.bottom - rcClient.top) / 2;
		return 0;
	case WM_SIZE:
		/*获取客户区尺寸*/
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		/*构造内存DC*/
		if (hdcMem != NULL)
			DeleteDC(hdcMem);
		hdcWnd = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdcWnd);
		hBitmap = CreateCompatibleBitmap(hdcWnd, cxClient, cyClient);
		ReleaseDC(hwnd, hdcWnd);
		SelectObject(hdcMem, hBitmap);
		DeleteObject(hBitmap);
		/*保存客户区矩形*/
		rcClient.left = rcClient.top = 0;
		rcClient.right = cxClient;
		rcClient.bottom = cyClient;
		/*绘图*/
		DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
		return 0;
	case WM_LBUTTONDOWN:
		if (cyClient - HIWORD(lParam) <= 32 && cxClient - LOWORD(lParam) <= 128 && cxClient - LOWORD(lParam) >= 96)
		{
			/*原点回到窗口中央*/
			xOrigin = cxClient / 2;
			yOrigin = cyClient / 2;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
		}
		else if (cyClient - HIWORD(lParam) <= 32 && cxClient - LOWORD(lParam) <= 64 && cxClient - LOWORD(lParam) >= 32)
		{
			/*放大坐标系*/
			if (space == 0)
			{
				MessageBeep(0);
				return 0;
			}
			t = division[space];
			space = max(space - 1, 0);
			if (xOrigin < cxClient / 2)
				xOrigin -= (t / division[space] - 1)*(cxClient / 2 - xOrigin);
			else
				xOrigin += (t / division[space] - 1)*(xOrigin - cxClient / 2);
			if (yOrigin < cyClient / 2)
				yOrigin -= (t / division[space] - 1)*(cyClient / 2 - yOrigin);
			else
				yOrigin += (t / division[space] - 1)*(yOrigin - cyClient / 2);
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
		}
		else if (cyClient - HIWORD(lParam) <= 32 && cxClient - LOWORD(lParam) <= 32)
		{
			/*缩小坐标系*/
			if (space == 6)
			{
				MessageBeep(0);
				return 0;
			}
			t = division[space];
			space = min(space + 1, 6);
			if (xOrigin < cxClient / 2)
				xOrigin += (1 - t / division[space])*(cxClient / 2 - xOrigin);
			else
				xOrigin -= (1 - t / division[space])*(xOrigin - cxClient / 2);
			if (yOrigin < cyClient / 2)
				yOrigin += (1 - t / division[space])*(cyClient / 2 - yOrigin);
			else
				yOrigin -= (1 - t / division[space])*(yOrigin - cyClient / 2);
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
		}

		dx = LOWORD(lParam) - xOrigin;
		dy = HIWORD(lParam) - yOrigin;
		return 0;
	case WM_MOUSEMOVE:
		/*实现鼠标拖动坐标系*/
		if (wParam & MK_LBUTTON)
		{
			xOrigin = LOWORD(lParam) - dx;
			yOrigin = HIWORD(lParam) - dy;
			dx = LOWORD(lParam) - xOrigin;
			dy = HIWORD(lParam) - yOrigin;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
		}
		return 0;
	case WM_KEYDOWN:
		/*实现方向键移动坐标系*/
		switch (wParam)
		{
		case VK_LEFT:
			xOrigin += 25;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case VK_RIGHT:
			xOrigin -= 25;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case VK_UP:
			yOrigin += 25;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case VK_DOWN:
			yOrigin -= 25;
			DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_LBUTTONDOWN, 0, MAKELONG(cxClient - 120, cyClient - 10));
			break;
		default:
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		case ID_TOOLS_NEW:
			ret = DialogBox(hInst, MAKEINTRESOURCE(IDD_NEW), hwnd, NewDlgProc);
			if (ret == 1)
				DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case ID_TOOLS_LIST:
			ret = DialogBox(hInst, MAKEINTRESOURCE(IDD_LIST), hwnd, ListDlgProc);
			if (ret == 1)
				DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case ID_TOOLS_JUMP:
			ret = DialogBox(hInst, MAKEINTRESOURCE(IDD_JUMP), hwnd, JumpDlgProc);
			if (ret == 1)
				DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);			break;
		case ID_TOOLS_OPTION:
			ret = DialogBox(hInst, MAKEINTRESOURCE(IDD_OPTION), hwnd, OptionDlgProc);
			if (ret == 1)
				DrawAll(hwnd, hdcMem, function, hIcon_Return, hIcon_ZoomIn, hIcon_ZoomOut, unit, space, xOrigin, yOrigin, rcClient, cAxis, cGrid, cText, cBackground);
			break;
		case ID_HELP_VIEWHELP:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP), hwnd, HelpDlgProc);
			break;
		case ID_HELP_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
			break;
		default:
			break;
		}
		return 0;
	case WM_ERASEBKGND:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
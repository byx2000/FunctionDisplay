#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include "def.h"
#include "RPN.h"
#include "resource.h"

extern int cFunc;
extern double division[7];
extern int size_g[5];
extern const int space;
extern const int unit;

void DrawCoordinate(HDC hdc, int unit, double space, int xOrigin, int yOrigin, int cxClient, int cyClient, COLORREF cAxis, COLORREF cGrid, COLORREF cText)
{
    HPEN hPen_Grid, hPen_Axis;
    HFONT hFont_Text;
    int i;
    int cnt;
    char buf[50];

	hPen_Grid = CreatePen(PS_SOLID, 1, cGrid);
	hPen_Axis = CreatePen(PS_SOLID, 1, cAxis);
	hFont_Text = CreateFont(9, 4, 0, 0, FW_THIN, FALSE, FALSE, FALSE, CHINESEBIG5_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, TEXT("Consolas"));

    ///画网格
	SelectObject(hdc, hPen_Grid);
    if (yOrigin < cyClient)
    {
        cnt = (cyClient-yOrigin)/unit+1;
        for (i = max(-(yOrigin/unit), 0); i < cnt; ++i)
        {
            MoveToEx(hdc, 0, yOrigin + i * unit, NULL);
            LineTo(hdc, cxClient, yOrigin + i * unit);
        }
    }
    if (yOrigin > 0)
    {
        cnt = yOrigin/unit+1;
        for (i = max(0, (yOrigin-cyClient)/unit); i < cnt; ++i)
        {
            MoveToEx(hdc, 0, yOrigin - i * unit, NULL);
            LineTo(hdc, cxClient, yOrigin - i * unit);
        }
    }
    if (xOrigin < cxClient)
    {
        cnt = (cxClient-xOrigin)/unit+1;
        for (i = max(0, -(xOrigin/unit)); i < cnt; ++i)
        {
            MoveToEx(hdc, xOrigin + i * unit, 0, NULL);
            LineTo(hdc, xOrigin + i * unit, cyClient);
        }
    }
    if (xOrigin > 0)
    {
        cnt = xOrigin/unit+1;
        for (i = max(0, (xOrigin-cxClient)/unit); i < cnt; ++i)
        {
            MoveToEx(hdc, xOrigin - i * unit, 0, NULL);
            LineTo(hdc, xOrigin - i * unit, cyClient);
        }
    }

    ///画坐标轴
    SelectObject(hdc, hPen_Axis);
    if (yOrigin >= 0 && yOrigin <= cyClient)
    {
        MoveToEx(hdc, 0, yOrigin, NULL);
        LineTo(hdc, cxClient, yOrigin);
    }
    if (xOrigin >= 0 && xOrigin <= cxClient)
    {
        MoveToEx(hdc, xOrigin, 0, NULL);
        LineTo(hdc, xOrigin, cyClient);
    }
  
    ///标刻度
	SelectObject(hdc, hFont_Text);
    SetTextColor(hdc, cText);
    SetBkMode(hdc, TRANSPARENT);
    if (xOrigin < cxClient)
    {
        cnt = (cxClient - xOrigin) / unit + 1;
        for (i = max(1, -(xOrigin/unit)); i < cnt; ++i)
        {
            sprintf_s(buf, "%.1lf", i * space);
            TextOutA(hdc, xOrigin + i * unit - 4, min(max(yOrigin + 5, 0), cyClient-10), buf, strlen(buf));
        }
    }
    if (xOrigin > 0)
    {
        cnt = xOrigin / unit + 2;
        for (i = max(1, (xOrigin-cxClient)/unit); i < cnt; ++i)
        {
            sprintf_s(buf, "-%.1lf", i * space);
            TextOutA(hdc, xOrigin - i * unit - 8, min(max(yOrigin + 5, 0), cyClient-10), buf, strlen(buf));
        }
    }
    if (yOrigin > 0)
    {
        cnt = yOrigin / unit + 1;
        for (i = max(1, (yOrigin-cyClient)/unit); i < cnt; ++i)
        {
            sprintf_s(buf, "%.1lf", i * space);
            TextOutA(hdc, min(max(xOrigin+3, 0), cxClient-30), yOrigin - i * unit - 4, buf, strlen(buf));
        }
    }
    if (yOrigin < cyClient)
    {
        cnt = (cyClient - yOrigin) / unit + 1;
        for (i = max(1, -(yOrigin)/unit); i < cnt; ++i)
        {
            sprintf_s(buf, "-%.1lf", i * space);
            TextOutA(hdc, min(max(xOrigin+3, 0), cxClient-30), yOrigin + i * unit - 4, buf, strlen(buf));
        }
    }
    if (xOrigin >= 0 && xOrigin <= cxClient && yOrigin >= 0 && yOrigin <= cyClient)
        TextOutA(hdc, xOrigin+5, yOrigin + 5, "O", strlen("O"));

    ///释放资源
    DeleteObject(hFont_Text);
	DeleteObject(hPen_Axis);
	DeleteObject(hPen_Grid);
}

void DrawGraph(HDC hdc, int unit, FUNC * func, double space, int xOrigin, int yOrigin, int cxClient, int cyClient)
{
	if (cFunc == 0)
		return;

    int status;
    double beg;
    int i, j;
    HPEN hPen;
    double res;
    int t1;
    POINT a[100];
	char t[1024];
    int cnt;

    double u = space / (double)unit;
    POINT * pt = (POINT *)malloc(sizeof(POINT) * cxClient);
	for (i = 0; i < cxClient; ++i)
		pt[i].x = i;
    beg = (-xOrigin) / (unit) * space - (xOrigin%unit) * u;

	for (j = 0; j < cFunc; ++j)
	{
		
		CopyMemory(t, func[j].Expression, 1024);
		//MessageBoxA(NULL, t, "", MB_OK);
		PreProc(t, &status);
		if (status == SYNTAX_ERROR)
		{
			cFunc--;
			free(pt);
			return;
		}

		t1 = 0;
		cnt = 0;
		for (i = 0; i < cxClient; ++i)
		{
			res = Calculator(t, (beg + i*u), 0, &status);
			pt[i].y = yOrigin - (res / u) + ((res<0) ? 1 : 0);
			if (status == SYNTAX_ERROR)
			{
				free(pt);
				return;
			}
			else if (status == SUCCEED && pt[i].y >= 0 && pt[i].y <= cyClient && i < cxClient)
			{
				
				res = Calculator(t, (beg + max(i-1, 0)*u), 0, &status);
				if (status == SUCCEED)
				{
					a[cnt].x = max(i - 1, 0);
					pt[max(i - 1, 0)].y = yOrigin - (res / u) + ((res < 0) ? 1 : 0);
				}
				else
				{
					a[cnt].x = i;
					status = SUCCEED;
				}
					
				while (status == SUCCEED && pt[i].y >= 0 && pt[i].y <= cyClient && i < cxClient)
				{
					i++;
					res = Calculator(t, (beg + i*u), 0, &status);
					pt[i].y = yOrigin - (res / u) + ((res<0) ? 1 : 0);
				}
				
				res = Calculator(t, (beg + min(i, cxClient-1)*u), 0, &status);
				if (status == SUCCEED)
				{
					a[cnt].y = min(i, cxClient - 1);
					pt[min(i, cxClient - 1)].y = yOrigin - (res / u) + ((res < 0) ? 1 : 0);
				}
				else
					a[cnt].y = i - 1;
				cnt++;
				i--;
			}
		}

		hPen = CreatePen(PS_SOLID, 1, func[j].cGraph);
		SelectObject(hdc, hPen);
		if (cnt != 0)
		{
			for (i = 0; i < cnt; ++i)
			{
				Polyline(hdc, pt + a[i].x, a[i].y - a[i].x + 1);
			}
		}

		DeleteObject(hPen);
	}

    free(pt);
}

void DrawAll(HWND hwnd, HDC hdcMem, FUNC * func, HICON hBack, HICON hZoomIn, HICON hZoomOut, int unit, int space, int xOrigin, int yOrigin, RECT rClient, COLORREF cAxis, COLORREF cGrid, COLORREF cText, COLORREF cBackground)
{
    FillRect(hdcMem, &rClient, CreateSolidBrush(cBackground));
    DrawCoordinate(hdcMem, size_g[unit], division[space], xOrigin, yOrigin, rClient.right-rClient.left, rClient.bottom-rClient.top, cAxis, cGrid, cText);
	DrawGraph(hdcMem, size_g[unit], func, division[space], xOrigin, yOrigin, rClient.right - rClient.left, rClient.bottom - rClient.top);
    DrawIcon(hdcMem, rClient.right-rClient.left-128, rClient.bottom-rClient.top-32, hBack);
    DrawIcon(hdcMem, rClient.right-rClient.left-64, rClient.bottom-rClient.top-32, hZoomIn);
	DrawIcon(hdcMem, rClient.right - rClient.left - 32, rClient.bottom - rClient.top - 32, hZoomOut);
    HDC hdc = GetDC(hwnd);
    BitBlt(hdc, 0, 0, rClient.right-rClient.left, rClient.bottom-rClient.top, hdcMem, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
}

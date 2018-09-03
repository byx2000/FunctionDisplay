#pragma once

#include <windows.h>
#include "def.h"

void DrawCoordinate(HDC hdc, int unit, double space, int xOrigin, int yOrigin, int cxClient, int cyClient, COLORREF cAxis, COLORREF cGrid, COLORREF cText);
void DrawGraph(HDC hdc, int unit, FUNC * func, double space, int xOrigin, int yOrigin, int cxClient, int cyClient);
void DrawAll(HWND hwnd, HDC hdcMem, FUNC * func, HICON hBack, HICON hZoomIn, HICON hZoomOut, int unit, int space, int xOrigin, int yOrigin, RECT rClient, COLORREF cAxis, COLORREF cGrid, COLORREF cText, COLORREF cBackground);

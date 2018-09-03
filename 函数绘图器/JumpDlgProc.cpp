#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"

extern int xOrigin, yOrigin;
extern int space;
extern int unit;
extern double division[7];
extern int size_g[5];
extern int cxClient, cyClient;

BOOL CALLBACK JumpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char s1[1024] = { 0 };
	static char s2[1024] = { 0 };
	static double x = 0, y = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowTextA(GetDlgItem(hDlg, IDC_X), s1);
		SetWindowTextA(GetDlgItem(hDlg, IDC_Y), s2);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetWindowTextA(GetDlgItem(hDlg, IDC_X), s1, sizeof(s1));
			GetWindowTextA(GetDlgItem(hDlg, IDC_Y), s2, sizeof(s2));
			if (strlen(s1) == 0 || strlen(s2) == 0)
			{
				MessageBox(hDlg, TEXT(" ‰»ÎŒ™ø’£°"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				return TRUE;
			}
			x = atof(s1);
			y = atof(s2);
			if (x < 0)
				xOrigin = xOrigin + abs((int)(x / division[space] * size_g[unit])) - (xOrigin - cxClient / 2);
			else
				xOrigin = xOrigin - abs((int)(x / division[space] * size_g[unit])) - (xOrigin - cxClient / 2);
			if (y < 0)
				yOrigin = yOrigin - abs((int)(y / division[space] * size_g[unit])) - (yOrigin - cyClient / 2);
			else
				yOrigin = yOrigin + abs((int)(y / division[space] * size_g[unit])) - (yOrigin - cyClient / 2);
			EndDialog(hDlg, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

#include <windows.h>
#include <commdlg.h>
#include <stdio.h>
#include "resource.h"

extern double division[7];
extern int size_g[5];
extern int space;
extern int unit;
extern COLORREF cAxis, cGrid, cText, cBackground;

BOOL CALLBACK OptionDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char buf[10];

	static CHOOSECOLOR ccAxis, ccBackground, ccGrid, ccText;
	static COLORREF crCustColors[16];
	static HWND hCombox_Unit, hCombox_Space;

	switch (message)
	{
	case WM_INITDIALOG:
		/*获取窗口句柄*/
		hCombox_Unit = GetDlgItem(hDlg, IDC_GRIDSIZE);
		hCombox_Space = GetDlgItem(hDlg, IDC_SPACE);

		/*向尺寸组合框添加字符串*/
		for (i = 4; i >= 0; --i)
		{
			sprintf_s(buf, "%d", size_g[i]);
			SendMessageA(hCombox_Unit, CB_INSERTSTRING, 0, (LPARAM)buf);
		}
		/*向分度值组合框添加字符串*/
		for (i = 6; i >= 0; --i)
		{
			sprintf_s(buf, "%.1lf", division[i]);
			SendMessageA(hCombox_Space, CB_INSERTSTRING, 0, (LPARAM)buf);
		}

		/*选中当前网格尺寸*/
		SendMessage(hCombox_Unit, CB_SETCURSEL, unit, 0);
		/*选中当前分度值*/
		SendMessage(hCombox_Space, CB_SETCURSEL, space, 0);

		/*初始化*/
		ccAxis.rgbResult = cAxis;
		ccBackground.rgbResult = cBackground;
		ccGrid.rgbResult = cGrid;
		ccText.rgbResult = cText;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			/*应用更改*/
			unit = SendMessage(GetDlgItem(hDlg, IDC_GRIDSIZE), CB_GETCURSEL, 0, 0);
			space = SendMessage(GetDlgItem(hDlg, IDC_SPACE), CB_GETCURSEL, 0, 0);
			cAxis = ccAxis.rgbResult;
			cBackground = ccBackground.rgbResult;
			cGrid = ccGrid.rgbResult;
			cText = ccText.rgbResult;
			EndDialog(hDlg, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_CHOOSEAXISCOLOR:
			ccAxis.lStructSize = sizeof(CHOOSECOLOR);
			ccAxis.hwndOwner = hDlg;
			ccAxis.hInstance = NULL;
			ccAxis.rgbResult = cAxis;
			ccAxis.lpCustColors = crCustColors;
			ccAxis.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccAxis.lCustData = 0;
			ccAxis.lpfnHook = NULL;
			ccAxis.lpTemplateName = NULL;
			ChooseColor(&ccAxis);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		case IDC_CHOOSEBKCOLOR:
			ccBackground.lStructSize = sizeof(CHOOSECOLOR);
			ccBackground.hwndOwner = hDlg;
			ccBackground.hInstance = NULL;
			ccBackground.rgbResult = cBackground;
			ccBackground.lpCustColors = crCustColors;
			ccBackground.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccBackground.lCustData = 0;
			ccBackground.lpfnHook = NULL;
			ccBackground.lpTemplateName = NULL;
			ChooseColor(&ccBackground);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		case IDC_CHOOSEGRIDCOLOR:
			ccGrid.lStructSize = sizeof(CHOOSECOLOR);
			ccGrid.hwndOwner = hDlg;
			ccGrid.hInstance = NULL;
			ccGrid.rgbResult = cGrid;
			ccGrid.lpCustColors = crCustColors;
			ccGrid.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccGrid.lCustData = 0;
			ccGrid.lpfnHook = NULL;
			ccGrid.lpTemplateName = NULL;
			ChooseColor(&ccGrid);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		case IDC_CHOOSETEXTCOLOR:
			ccText.lStructSize = sizeof(CHOOSECOLOR);
			ccText.hwndOwner = hDlg;
			ccText.hInstance = NULL;
			ccText.rgbResult = cText;
			ccText.lpCustColors = crCustColors;
			ccText.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccText.lCustData = 0;
			ccText.lpfnHook = NULL;
			ccText.lpTemplateName = NULL;
			ChooseColor(&ccText);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		case IDC_COORDDEF:
			/*选中当前网格尺寸*/
			SendMessage(hCombox_Unit, CB_SETCURSEL, 1, 0);
			/*选中当前分度值*/
			SendMessage(hCombox_Space, CB_SETCURSEL, 3, 0);
			return TRUE;
		case IDC_COLORDEF:
			ccAxis.rgbResult = RGB(0, 0, 0);;
			ccBackground.rgbResult = RGB(255, 255, 255);
			ccGrid.rgbResult = RGB(196, 195, 191);
			ccText.rgbResult = RGB(0, 0, 0);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		default:
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		switch (GetDlgCtrlID((HWND)lParam))
		{
		case IDC_AXISCOLOR:
			return (INT_PTR)CreateSolidBrush(ccAxis.rgbResult);
		case IDC_BKCOLOR:
			return (INT_PTR)CreateSolidBrush(ccBackground.rgbResult);
		case IDC_GRIDCOLOR:
			return (INT_PTR)CreateSolidBrush(ccGrid.rgbResult);
		case IDC_TEXTCOLOR:
			return (INT_PTR)CreateSolidBrush(ccText.rgbResult);
		}
		return FALSE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

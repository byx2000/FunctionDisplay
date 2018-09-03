#include <windows.h>
#include "resource.h"
#include "def.h"
#include "RPN.h"

extern FUNC function[1024];
extern int cFunc;

BOOL CALLBACK NewDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CHOOSECOLOR ccGraph;
	static COLORREF crCustColors[16];

	char s[1024];
	char t[1024];
	int len;
	int i, j;
	int status;

	switch (message)
	{
	case WM_INITDIALOG:
		ccGraph.rgbResult = function[max(cFunc - 1, 0)].cGraph;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			/*Ìí¼Óº¯Êý*/
			GetWindowTextA(GetDlgItem(hDlg, IDC_EXPRESSION), s, 1024);
			len = strlen(s);
			for (i = 0; i < len; ++i)
			{
				for (j = i; s[j] != ';' && j < len; ++j);
				ZeroMemory(t, 1024);
				CopyMemory(t, s + i, j - i);
				//MessageBoxA(NULL, t, "", MB_OK);
				PreProc(t, &status);
				Calculator(t, 0, 0, &status);
				if (status != SYNTAX_ERROR && strlen(t) != 0)
				{
					CopyMemory(function[cFunc].Expression, s + i, j - i);
					function[cFunc].Expression[j - i] = '\0';
					function[cFunc].cGraph = ccGraph.rgbResult;
					cFunc++;
				}
					
				i = j;
			}
			EndDialog(hDlg, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDB_GRAPHCOLOR:
			ccGraph.lStructSize = sizeof(CHOOSECOLOR);
			ccGraph.hwndOwner = hDlg;
			ccGraph.hInstance = NULL;
			ccGraph.rgbResult = function[max(cFunc - 1, 0)].cGraph;
			ccGraph.lpCustColors = crCustColors;
			ccGraph.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccGraph.lCustData = 0;
			ccGraph.lpfnHook = NULL;
			ccGraph.lpTemplateName = NULL;
			ChooseColor(&ccGraph);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		default:
			break;
		}
		break;
	case WM_CTLCOLORSTATIC:
		if (GetDlgCtrlID((HWND)lParam) == IDC_GRAPHCOLOR)
			return (INT_PTR)CreateSolidBrush(ccGraph.rgbResult);
		else
			return FALSE;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}
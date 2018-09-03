#include <windows.h>
#include "resource.h"
#include "def.h"
#include "RPN.h"

extern FUNC function[1024];
extern int sel;

BOOL CALLBACK ModifyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CHOOSECOLOR ccGraph;
	static COLORREF crCustColors[16];
	static HWND hEdit;

	char s[1024];
	char t[1024];
	int status;

	switch (message)
	{
	case WM_INITDIALOG:
		hEdit = GetDlgItem(hDlg, IDC_NEWEXPRESSION);
		ccGraph.rgbResult = function[sel].cGraph;
		CopyMemory(s, function[sel].Expression, 1024);
		SetWindowTextA(hEdit, s);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_GRAPHCOLOR:
			ccGraph.lStructSize = sizeof(CHOOSECOLOR);
			ccGraph.hwndOwner = hDlg;
			ccGraph.hInstance = NULL;
			ccGraph.rgbResult = function[sel].cGraph;
			ccGraph.lpCustColors = crCustColors;
			ccGraph.Flags = CC_RGBINIT | CC_FULLOPEN;
			ccGraph.lCustData = 0;
			ccGraph.lpfnHook = NULL;
			ccGraph.lpTemplateName = NULL;
			ChooseColor(&ccGraph);
			InvalidateRect(hDlg, NULL, FALSE);
			return TRUE;
		case IDOK:
			GetWindowTextA(hEdit, t, 1024);
			PreProc(t, &status);
			Calculator(t, 0, 0, &status);
			if (status == SYNTAX_ERROR)
			{
				MessageBox(hDlg, TEXT("您输入的表达式有误，请重新输入！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				return TRUE;
			}
			GetWindowTextA(hEdit, function[sel].Expression, 1024);
			function[sel].cGraph = ccGraph.rgbResult;
			EndDialog(hDlg, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
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
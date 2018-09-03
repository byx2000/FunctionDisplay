#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "def.h"
#include "ModifyDlgProc.h"

extern HINSTANCE hInst;
extern int cFunc;
extern FUNC function[1024];
int sel;

BOOL CALLBACK ListDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	static HWND hButton_Modify, hButton_Delete, hButton_Clear;
	static int IsEnable;
	char s[1024+5];
	int i;
	static int ret1 = 0, ret2 = 0, ret3 = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		/*获取窗口句柄*/
		hList = GetDlgItem(hDlg, IDC_FUNCLIST);
		hButton_Modify = GetDlgItem(hDlg, IDC_MODIFY);
		hButton_Delete = GetDlgItem(hDlg, IDC_DELETE);
		hButton_Clear = GetDlgItem(hDlg, IDC_CLEAR);
		/*向列表框中填充数据*/
		for (i = 0; i < cFunc; ++i)
		{
			sprintf_s(s, "y = %s", function[i].Expression);
			SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)s);
		}
		/*禁用按钮*/
		EnableWindow(hButton_Modify, FALSE);
		EnableWindow(hButton_Delete, FALSE);
		IsEnable = FALSE;
		if (SendMessage(hList, LB_GETCOUNT, 0, 0) == 0)
			EnableWindow(hButton_Clear, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_RETURN:
			EndDialog(hDlg, ret1 | ret2 | ret3);
			return TRUE;
		case IDC_MODIFY:
			sel = SendMessage(hList, LB_GETCURSEL, 0, 0);
			ret1 = DialogBox(hInst, MAKEINTRESOURCE(IDD_MODIFY), hDlg, ModifyDlgProc);
			if (ret1 == 1)
			{
				SendMessage(hList, LB_DELETESTRING, sel, 0);
				sprintf_s(s, "y = %s", function[sel].Expression);
				SendMessageA(hList, LB_INSERTSTRING, sel, (LPARAM)s);
			}	
			SendMessage(hList, LB_SETCURSEL, sel, 0);
			return TRUE;
		case IDC_CLEAR:
			if (MessageBox(hDlg, TEXT("您确定要清空列表吗？"), TEXT("清空列表"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				ret3 = 0;
				return TRUE;
			}
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			cFunc = 0;
			ret3 = 1;
			return TRUE;
		case IDC_DELETE:
			if (MessageBox(hDlg, TEXT("您确定要删除此条目吗？"), TEXT("删除条目"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				ret2 = 0;
				return TRUE;
			}	
			sel = SendMessage(hList, LB_GETCURSEL, 0, 0);
			for (i = sel; i < cFunc - 1; ++i)
				function[i] = function[i + 1];
			SendMessage(hList, LB_DELETESTRING, sel, 0);
			cFunc--;
			ret2 = 1;
			if (SendMessage(hList, LB_GETCOUNT, 0, 0) != 0)
				SendMessage(hList, LB_SETCURSEL, 0, 0);
			else
			{
				/*禁用按钮*/
				EnableWindow(hButton_Modify, FALSE);
				EnableWindow(hButton_Delete, FALSE);
				IsEnable = FALSE;
				EnableWindow(hButton_Clear, FALSE);
			}
			return TRUE;
		default:
			break;
		}

		if (HIWORD(wParam) == LBN_SELCHANGE && IsEnable == 0 && SendMessage(hList, LB_GETCOUNT, 0, 0) != 0)
		{
			/*激活按钮*/
			EnableWindow(hButton_Modify, TRUE);
			EnableWindow(hButton_Delete, TRUE);
			IsEnable = TRUE;
			EnableWindow(hButton_Clear, TRUE);
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
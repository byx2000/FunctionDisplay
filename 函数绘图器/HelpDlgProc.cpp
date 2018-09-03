#include <windows.h>
#include "resource.h"

void AddString(HWND hEdit, const char* buf)
{
	char s[2014];
	GetWindowTextA(hEdit, s, sizeof(s));
	strcat_s(s, buf);
	SetWindowTextA(hEdit, s);
}

BOOL CALLBACK HelpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit;

	switch (message)
	{
	case WM_INITDIALOG:
		hEdit = GetDlgItem(hDlg, IDC_HELPTEXT);
		AddString(hEdit, "                                      函数绘图器      帮助内容\r\n");
		AddString(hEdit, "●操作说明\r\n");
		AddString(hEdit, "      1.绘制函数图像\r\n");
		AddString(hEdit, "            选择菜单项：工具->绘制新函数，弹出“绘制新函数”对话框，您可以输入函数的解析式(支持+、-、*、/以及括号)以及选择图像颜色\r\n");
		AddString(hEdit, "            在编辑框内输入函数时，用“;”分隔每一个解析式，可以一次画出多个函数图像\r\n");
		AddString(hEdit, "      2.管理多个函数\r\n");
		AddString(hEdit, "            选择菜单项：工具->函数列表，在弹出的“函数列表”对话框中，您可以查看当前已绘制的所有函数并进行删除、清空、修改操作\r\n");
		AddString(hEdit, "      3.个性化设置\r\n");
		AddString(hEdit, "            选择菜单项：工具->选项，您可以对坐标系进行相关设置，并可以随时恢复默认值\r\n");
		AddString(hEdit, "      4.快速跳转\r\n");
		AddString(hEdit, "            选择菜单项：工具->快速跳转，输入您要跳转到的坐标。点击“跳转”按钮，该坐标位置将会出现在窗口中央\r\n");
		AddString(hEdit, "      5.右下角快捷按钮\r\n");
		AddString(hEdit, "            在窗口的右下角，有三个快捷按钮，从左到右依次为：使原点回归窗口中央、放大坐标系、缩小坐标系。\r\n");
		AddString(hEdit, "            使用快捷按钮可方便用户操作。\r\n");
		AddString(hEdit, "      6.其它操作\r\n");
		AddString(hEdit, "            ①使用鼠标拖曳可移动坐标系\r\n");
		AddString(hEdit, "            ②使用键盘方向键可移动坐标系\r\n");
		AddString(hEdit, "            ③使用键盘上的'+'、'-'可放大或缩小坐标系\r\n");
		AddString(hEdit, "            ④按下ESC件可以使原点回归窗口中央\r\n");
		AddString(hEdit, "\r\n");
		SetFocus(hEdit);
		SendMessage(hEdit, EM_SETSEL, -1, 0);
		return TRUE;
	case WM_CTLCOLORSTATIC:
		return (BOOL)CreateSolidBrush(RGB(255, 255, 255));
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

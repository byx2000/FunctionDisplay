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
		AddString(hEdit, "                                      ������ͼ��      ��������\r\n");
		AddString(hEdit, "�����˵��\r\n");
		AddString(hEdit, "      1.���ƺ���ͼ��\r\n");
		AddString(hEdit, "            ѡ��˵������->�����º����������������º������Ի������������뺯���Ľ���ʽ(֧��+��-��*��/�Լ�����)�Լ�ѡ��ͼ����ɫ\r\n");
		AddString(hEdit, "            �ڱ༭�������뺯��ʱ���á�;���ָ�ÿһ������ʽ������һ�λ����������ͼ��\r\n");
		AddString(hEdit, "      2.����������\r\n");
		AddString(hEdit, "            ѡ��˵������->�����б��ڵ����ġ������б��Ի����У������Բ鿴��ǰ�ѻ��Ƶ����к���������ɾ������ա��޸Ĳ���\r\n");
		AddString(hEdit, "      3.���Ի�����\r\n");
		AddString(hEdit, "            ѡ��˵������->ѡ������Զ�����ϵ����������ã���������ʱ�ָ�Ĭ��ֵ\r\n");
		AddString(hEdit, "      4.������ת\r\n");
		AddString(hEdit, "            ѡ��˵������->������ת��������Ҫ��ת�������ꡣ�������ת����ť��������λ�ý�������ڴ�������\r\n");
		AddString(hEdit, "      5.���½ǿ�ݰ�ť\r\n");
		AddString(hEdit, "            �ڴ��ڵ����½ǣ���������ݰ�ť������������Ϊ��ʹԭ��ع鴰�����롢�Ŵ�����ϵ����С����ϵ��\r\n");
		AddString(hEdit, "            ʹ�ÿ�ݰ�ť�ɷ����û�������\r\n");
		AddString(hEdit, "      6.��������\r\n");
		AddString(hEdit, "            ��ʹ�������ҷ���ƶ�����ϵ\r\n");
		AddString(hEdit, "            ��ʹ�ü��̷�������ƶ�����ϵ\r\n");
		AddString(hEdit, "            ��ʹ�ü����ϵ�'+'��'-'�ɷŴ����С����ϵ\r\n");
		AddString(hEdit, "            �ܰ���ESC������ʹԭ��ع鴰������\r\n");
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

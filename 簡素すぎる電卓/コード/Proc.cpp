#include "Dentaku.h"

void FFW_CREATE(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp);
void FFW_PAINT(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp, HDC hdc);
void FFW_COMMAND(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp);

HWND CreateButton(int x, int y, int width, int hight, LPCWSTR str, HWND hwnd, int num);
HINSTANCE GetHINSTANCE(void);

static std::string EditText;
static HWND EditHwnd;
static Calculation clclt;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg) {
	case WM_CREATE:
		FFW_CREATE(hwnd, msg, wp, lp);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		FFW_PAINT(hwnd, msg, wp, lp, hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		FFW_COMMAND(hwnd, msg, wp, lp);
		InvalidateRect(hwnd, NULL, TRUE);//�ĕ`��i���������ꂽ��K���s���j
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

HINSTANCE GetHINSTANCE(void) {
	return ((HINSTANCE)GetModuleHandle(0));
}

HWND CreateButton(int x, int y, int width, int hight, LPCWSTR str, HWND hwnd, int num) {
	return CreateWindow(TEXT("BUTTON"), str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, width, hight, hwnd, (HMENU)num, GetHINSTANCE(), NULL);
}

void FFW_CREATE(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp) {
	//������̏�����
	EditText = "";
	/*�e�L�X�g�{�b�N�X�̍쐬*/
	EditHwnd = CreateWindowEx(0,
		TEXT("EDIT"),TEXT(""),
		SS_CENTERIMAGE | WS_CHILD | WS_VISIBLE,
		0,0,125,50,
		hwnd,(HMENU)EDITBOX,
		GetHINSTANCE(),	NULL);
	/*�{�^���̍쐬*/
	CreateButton(0, 150, 25, 25,TEXT("^"), hwnd, BUTTON_ZYOU);//�ݏ�{�^�� ^
	CreateButton(25, 150, 25, 25, TEXT("��"),hwnd, BUTTON_ROOT);//��{�^�� r(
	CreateButton(50, 150, 25, 25, TEXT("sin"),hwnd, BUTTON_SIN);//Sin�{�^�� s(
	CreateButton(75, 150, 25, 25, TEXT("cos"),hwnd, BUTTON_COS);//Cos�{�^�� c(
	CreateButton(100, 150, 25, 25, TEXT("tan"),hwnd, BUTTON_TAN);//Tan�{�^�� t(
	CreateButton(0, 125, 25, 25,TEXT("0"), hwnd, BUTTON_NUM[0]);//0�{�^�� 0
	CreateButton(25, 125, 25, 25,TEXT("("), hwnd, BUTTON_START);//(�{�^�� (
	CreateButton(50, 125, 25, 25, TEXT(")"), hwnd, BUTTON_END);//)�{�^�� )
	CreateButton(75, 125, 25, 25, TEXT("-"), hwnd, BUTTON_HIKI);//-�{�^�� -
	CreateButton(100, 125, 25, 25, TEXT("."), hwnd, BUTTON_TEN);//.�{�^�� .
	CreateButton(0, 100, 25, 25, TEXT("1"), hwnd, BUTTON_NUM[1]);//1�{�^�� 1
	CreateButton(25, 100, 25, 25, TEXT("2"), hwnd, BUTTON_NUM[2]);//2�{�^�� 2
	CreateButton(50, 100, 25, 25, TEXT("3"), hwnd, BUTTON_NUM[3]);//3�{�^�� 3
	CreateButton(75, 100, 25, 25, TEXT("+"), hwnd, BUTTON_TASI);//+�{�^�� +
	CreateButton(100, 100, 25, 25, TEXT("="), hwnd, BUTTON_EQ);//=�{�^�� �v�Z����
	CreateButton(0, 75, 25, 25, TEXT("4"), hwnd, BUTTON_NUM[4]);//4�{�^�� 4
	CreateButton(25, 75, 25, 25, TEXT("5"), hwnd, BUTTON_NUM[5]);//5�{�^�� 5
	CreateButton(50, 75, 25, 25, TEXT("6"), hwnd, BUTTON_NUM[6]);//6�{�^�� 6
	CreateButton(75, 75, 25, 25, TEXT("��"), hwnd, BUTTON_WARI);//���{�^�� /
	CreateButton(100, 75, 25, 25, TEXT("��"), hwnd, BUTTON_BACK);//���{�^�� �ꕶ������
	CreateButton(0, 50, 25, 25, TEXT("7"), hwnd, BUTTON_NUM[7]);//7�{�^�� 7
	CreateButton(25, 50, 25, 25, TEXT("8"), hwnd, BUTTON_NUM[8]);//8�{�^�� 8
	CreateButton(50, 50, 25, 25, TEXT("9"), hwnd, BUTTON_NUM[9]);//9�{�^�� 9
	CreateButton(75, 50, 25, 25, TEXT("�~"), hwnd, BUTTON_KAKE);//�~�{�^�� *
	CreateButton(100,50,25,25,TEXT("C"),hwnd,BUTTON_C);//C�{�^�� �S�ď���
}

void FFW_PAINT(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp, HDC hdc) {
	CString Estr="";
	for (int i = 0; i < EditText.length(); i++) {
		/*�ϊ����K�v�ȕ������A�K�؂ɕϊ��i�u�������j���Ă���*/
		if (EditText[i] == 'r')
			Estr += "��";
		else if (EditText[i] == 's')
			Estr += "sin";
		else if (EditText[i] == 'c')
			Estr += "cos";
		else if (EditText[i] == 't')
			Estr += "tan";
		else if (EditText[i] == '*')
			Estr += "�~";
		else if (EditText[i] == '/')
			Estr += "��";
		else//�ϊ��s�v
			Estr += EditText[i];
	}
	SetWindowText(EditHwnd, Estr);
}

void FFW_COMMAND(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp) {
	CString Estr;
	if ((int)LOWORD(wp) >= 0 && (int)LOWORD(wp) <= 9)
		EditText += std::to_string((int)LOWORD(wp));
	else {//�����ȊO
		switch (LOWORD(wp)) {
		case BUTTON_ZYOU:
			EditText += "^(";
			break;
		case BUTTON_ROOT:
			EditText += "r(";
			break;
		case BUTTON_SIN:
			EditText += "s(";
			break;
		case BUTTON_COS:
			EditText += "c(";
			break;
		case BUTTON_TAN:
			EditText += "t(";
			break;
		case BUTTON_START:
			EditText += "(";
			break;
		case BUTTON_END:
			EditText += ")";
			break;
		case BUTTON_HIKI:
			EditText += "-";
			break;
		case BUTTON_TEN:
			EditText += ".";
			break;
		case BUTTON_TASI:
			EditText += "+";
			break;
		case BUTTON_KAKE:
			EditText += "*";
			break;
		case BUTTON_WARI:
			EditText += "/";
			break;
		case BUTTON_C:
			EditText = "";
			break;
		case BUTTON_BACK:
			if (EditText.length() > 0)
				EditText = EditText.substr(0, EditText.length() - 1);
			break;
		case BUTTON_EQ://�v�Z������
			clclt.Set(EditText);
			EditText = std::to_string(clclt.Do());
			if (clclt.IsError) {//�G���[����
				switch (clclt.ErrorNum) {//����
				case CLCT_ERROR_WARI:
					Estr = "���鐔�i����j�����ʂƂ���0�ƂȂ������ߌv�Z���ł��܂���ł����I�I";
					break;
				case CLCT_ERROR_ROOT:
					Estr = "���[�g�i��j�̒��g�����̐��ł��������߁A�����ɂȂ��Ă��܂��̂Ōv�Z���ł��܂���ł����I�I";
					break;
				case CLCT_ERROR_FORMULA:
					Estr = "�v�Z�����s���������̂Ōv�Z�𒆎~���܂����I�����Ƃ��Đ��藧���ɒ����Ă���u���v���������Ă��������I�I";
					break;
				}
				MessageBox(hwnd, Estr, TEXT("�G���["), MB_OK);//�G���[�̍��m
			}
			break;
		}
	}
}
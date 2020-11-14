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
		InvalidateRect(hwnd, NULL, TRUE);//再描画（何か押されたら必ず行う）
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
	//文字列の初期化
	EditText = "";
	/*テキストボックスの作成*/
	EditHwnd = CreateWindowEx(0,
		TEXT("EDIT"),TEXT(""),
		SS_CENTERIMAGE | WS_CHILD | WS_VISIBLE,
		0,0,125,50,
		hwnd,(HMENU)EDITBOX,
		GetHINSTANCE(),	NULL);
	/*ボタンの作成*/
	CreateButton(0, 150, 25, 25,TEXT("^"), hwnd, BUTTON_ZYOU);//累乗ボタン ^
	CreateButton(25, 150, 25, 25, TEXT("√"),hwnd, BUTTON_ROOT);//√ボタン r(
	CreateButton(50, 150, 25, 25, TEXT("sin"),hwnd, BUTTON_SIN);//Sinボタン s(
	CreateButton(75, 150, 25, 25, TEXT("cos"),hwnd, BUTTON_COS);//Cosボタン c(
	CreateButton(100, 150, 25, 25, TEXT("tan"),hwnd, BUTTON_TAN);//Tanボタン t(
	CreateButton(0, 125, 25, 25,TEXT("0"), hwnd, BUTTON_NUM[0]);//0ボタン 0
	CreateButton(25, 125, 25, 25,TEXT("("), hwnd, BUTTON_START);//(ボタン (
	CreateButton(50, 125, 25, 25, TEXT(")"), hwnd, BUTTON_END);//)ボタン )
	CreateButton(75, 125, 25, 25, TEXT("-"), hwnd, BUTTON_HIKI);//-ボタン -
	CreateButton(100, 125, 25, 25, TEXT("."), hwnd, BUTTON_TEN);//.ボタン .
	CreateButton(0, 100, 25, 25, TEXT("1"), hwnd, BUTTON_NUM[1]);//1ボタン 1
	CreateButton(25, 100, 25, 25, TEXT("2"), hwnd, BUTTON_NUM[2]);//2ボタン 2
	CreateButton(50, 100, 25, 25, TEXT("3"), hwnd, BUTTON_NUM[3]);//3ボタン 3
	CreateButton(75, 100, 25, 25, TEXT("+"), hwnd, BUTTON_TASI);//+ボタン +
	CreateButton(100, 100, 25, 25, TEXT("="), hwnd, BUTTON_EQ);//=ボタン 計算する
	CreateButton(0, 75, 25, 25, TEXT("4"), hwnd, BUTTON_NUM[4]);//4ボタン 4
	CreateButton(25, 75, 25, 25, TEXT("5"), hwnd, BUTTON_NUM[5]);//5ボタン 5
	CreateButton(50, 75, 25, 25, TEXT("6"), hwnd, BUTTON_NUM[6]);//6ボタン 6
	CreateButton(75, 75, 25, 25, TEXT("÷"), hwnd, BUTTON_WARI);//÷ボタン /
	CreateButton(100, 75, 25, 25, TEXT("←"), hwnd, BUTTON_BACK);//←ボタン 一文字消す
	CreateButton(0, 50, 25, 25, TEXT("7"), hwnd, BUTTON_NUM[7]);//7ボタン 7
	CreateButton(25, 50, 25, 25, TEXT("8"), hwnd, BUTTON_NUM[8]);//8ボタン 8
	CreateButton(50, 50, 25, 25, TEXT("9"), hwnd, BUTTON_NUM[9]);//9ボタン 9
	CreateButton(75, 50, 25, 25, TEXT("×"), hwnd, BUTTON_KAKE);//×ボタン *
	CreateButton(100,50,25,25,TEXT("C"),hwnd,BUTTON_C);//Cボタン 全て消す
}

void FFW_PAINT(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp, HDC hdc) {
	CString Estr="";
	for (int i = 0; i < EditText.length(); i++) {
		/*変換が必要な文字を、適切に変換（置き換え）していく*/
		if (EditText[i] == 'r')
			Estr += "√";
		else if (EditText[i] == 's')
			Estr += "sin";
		else if (EditText[i] == 'c')
			Estr += "cos";
		else if (EditText[i] == 't')
			Estr += "tan";
		else if (EditText[i] == '*')
			Estr += "×";
		else if (EditText[i] == '/')
			Estr += "÷";
		else//変換不要
			Estr += EditText[i];
	}
	SetWindowText(EditHwnd, Estr);
}

void FFW_COMMAND(HWND hwnd, UINT meg, WPARAM wp, LPARAM lp) {
	CString Estr;
	if ((int)LOWORD(wp) >= 0 && (int)LOWORD(wp) <= 9)
		EditText += std::to_string((int)LOWORD(wp));
	else {//数字以外
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
		case BUTTON_EQ://計算をする
			clclt.Set(EditText);
			EditText = std::to_string(clclt.Do());
			if (clclt.IsError) {//エラー発生
				switch (clclt.ErrorNum) {//分岐
				case CLCT_ERROR_WARI:
					Estr = "割る数（分母）が結果として0となったため計算ができませんでした！！";
					break;
				case CLCT_ERROR_ROOT:
					Estr = "ルート（√）の中身が負の数であったため、虚数になってしまうので計算ができませんでした！！";
					break;
				case CLCT_ERROR_FORMULA:
					Estr = "計算式が不正だったので計算を中止しました！数式として成り立つ式に直してから「＝」を押ししてください！！";
					break;
				}
				MessageBox(hwnd, Estr, TEXT("エラー"), MB_OK);//エラーの告知
			}
			break;
		}
	}
}
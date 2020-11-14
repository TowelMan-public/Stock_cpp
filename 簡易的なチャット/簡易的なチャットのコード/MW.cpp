#include "chat.h"

namespace MW {
	//ここで使い、定義する定数
	const int BUTTON = 10001;
	const int EDIT = 10002;
	//privateでグローバルな変数群
	static HWND MainHwnd = NULL;
	static HWND ChatListHwnd = NULL;
	static HWND UserListHwnd = NULL;
	static HWND GroupListHwnd = NULL;
	static HWND EditHwnd = NULL;
	static HWND ButtonHwnd = NULL;
	static HINSTANCE Hinstance;
	//publicな関数群
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	HINSTANCE GetHinstance(void);
	void UpdateUser(void);
	void UpdateGroup(void);
	void UpdateChat(int talkID);
	//privateな関数群
	void Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc);
	void Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void CreateUserList(bool IsInit = true);
	void CreateGroupList(bool IsInit = true);
	void CreateChatList(bool IsInit = true);
}

HINSTANCE MW::GetHinstance(void) {
	return Hinstance;
}

void MW::UpdateUser(void){
	CreateUserList(false);
}

void MW::UpdateGroup(void){
	CreateGroupList(false);
}

void MW::UpdateChat(int talkID){
	//チャットにトークIDを反映させる
	CW::SetTalkID(talkID);
	CreateChatList(false);
}

LRESULT CALLBACK MW::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg) {
	case WM_CREATE:
		Create(hwnd, msg, wp, lp);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		Paint(hwnd, msg, wp, lp, hdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		Comand(hwnd, msg, wp, lp);
		//InvalidateRect(hwnd, NULL, TRUE);//再描画（何か押されたら必ず行う）
		break;
	case WM_DESTROY:
		DB::End();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void MW::Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	//Hinstanceのセット
	Hinstance = ((HINSTANCE)GetModuleHandle(0));
	//MainHwnd
	MainHwnd = hwnd;
	//ユーザーリストの作成
	CreateUserList();
	//グループリストの作成
	CreateGroupList();
	//チャットリストの作成
	CreateChatList();
	//送信ボタンの作成
	ButtonHwnd = CreateWindow(
		_TEXT("BUTTON"), _TEXT("送信"),//クラス名、アプリ名
		WS_CHILD | WS_VISIBLE | WS_BORDER,//ウィンドウのスタイル
		226, 400, 100, 50,//場所x、場所y、幅、高さ
		hwnd, (HMENU)BUTTON,//親ウィンドウハンドル、メニューのハンドル
		Hinstance, NULL//インスタンスハンドル、void*的な奴
	);
	//送信ボックスの作成
	EditHwnd = CreateWindow(
		_TEXT("EDIT"), NULL,//クラス名、アプリ名
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL,//ウィンドウのスタイル
		226, 450, 557, 150,//場所x、場所y、幅、高さ
		hwnd, (HMENU)EDIT,//親ウィンドウハンドル、メニューのハンドル
		Hinstance, NULL//インスタンスハンドル、void*的な奴
	);
}

void MW::Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc){
}

void MW::Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	LPWSTR strText;
	Accnt act;
	std::wstring infoStr;
	switch (LOWORD(wp)) {
	case MWM_LOGIN://IDD_DIALOG_LOGIN
		//ログインが住んでないときのみ
		if (!DB::GetIsLogin()) {
			if (DB::HaveMyID())//登録されてる
				DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_LOGIN), hwnd, Dialog::LoginProc);
			else//登録がされていない
				DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_New), hwnd, Dialog::NewProc);
		}
		break;
	case  WMM_CON_ADD://IDD_DIALOG_ADD_UESER
		if (DB::GetIsLogin())
			DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_ADD_UESER), hwnd, Dialog::AddUeserProc);
		break;
	case WMM_CON_MAKEG://IDD_DIALOG_MAKE
		if (DB::GetIsLogin())
			DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_MAKE), hwnd, Dialog::MakeGroupProc);
		break;
	case WMM_CON_ADDG://IDD_DIALOG_ADD_GROUP
		if (DB::GetIsLogin())
			DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_ADD_GROUP), hwnd, Dialog::AddGroupProc);
		break;
	case WMM_CON_INFO://メッセージボックスで情報を表示
		if (DB::GetIsLogin() && CW::GetTalkID() != -1) {
			if (DB::GetIsGroupByID(CW::GetTalkID())) {
				act = DB::GetGroupByID(CW::GetTalkID());
				infoStr = _TEXT("ID:") + std::to_wstring(act.number);
				infoStr += _TEXT("グループ名:") + act.name;
			}
			else {
				act = DB::GetUserByID(CW::GetTalkID());
				infoStr = _TEXT("ID:") + std::to_wstring(act.number) + _TEXT("\n");
				infoStr += _TEXT("ユーザー名:") + act.name;
			}
			MessageBox(hwnd, infoStr.c_str(), _TEXT("情報"), MB_OK);
		}
		break;
	case WMM_CON_UESER://IDD_DIALOG_UESER
		if (DB::GetIsLogin())
			DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_UESER), hwnd, Dialog::DUeserProc);
		break;
	case WMM_CON_GROUPADD://IDD_DIALOG_GROUP
		if (DB::GetIsLogin())
			DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_GROUP), hwnd, Dialog::DGroupProc);
		break;
	case BUTTON://送信
		//文字列の取得
		if (DB::GetIsLogin() && GetWindowTextLength(EditHwnd) != 0 && CW::GetTalkID() != -1) {//文字があり、宛先が選択されてる
			strText = new WCHAR[GetWindowTextLength(EditHwnd) + 2];//+2は余分にみているから
			GetWindowText(EditHwnd, strText, GetWindowTextLength(EditHwnd) + 2);
			//トークIDに送信する
			DB::SendChat(CW::GetTalkID(), strText);
			UpdateChat(CW::GetTalkID());
			delete[] strText;
		}
		break;
	case EN_UPDATE:
		break;
	case EN_CHANGE:
		break;
	case EN_ERRSPACE:
		break;
	}
}

void MW::CreateUserList(bool IsInit){
	if (IsInit) {//クラス登録
		WNDCLASS winc2;
		winc2.style = CS_HREDRAW | CS_VREDRAW;//基本スタイル
		winc2.lpfnWndProc = UW::UserListProc;//プロキシ関数へのポインタ
		winc2.cbClsExtra = NULL;//クラス構造体の追加領域（必要なし）
		winc2.cbWndExtra = NULL;//ウィンドウ構造体の追加領域（必要なし）
		winc2.hInstance = Hinstance;//インスタンスハンドル
		winc2.hIcon = LoadIcon(NULL, IDI_APPLICATION);//アイコン(モジュール,名前)
		winc2.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソル（モジュール,名前）
		winc2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//クライアントエリアの背景色(ブラシのハンドルを渡す)
		winc2.lpszMenuName = NULL;//クラスメニュー
		winc2.lpszClassName = _TEXT("Ueser");//クラス名
		if (!RegisterClass(&winc2)) return;
	}
	//ウィンドウ生成
	UserListHwnd = CreateWindow(
		_TEXT("Ueser"), _TEXT("ユーザー"),//クラス名、アプリ名
		WS_CHILD | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//ウィンドウのスタイル
		0, 0, 225, 300,//場所x、場所y、幅、高さ
		MainHwnd, NULL,//親ウィンドウハンドル、メニューのハンドル
		Hinstance, NULL//インスタンスハンドル、void*的な奴
	);
	ShowWindow(UserListHwnd, SW_SHOW);
}

void MW::CreateGroupList(bool IsInit){
	if (IsInit) {//クラス登録
		WNDCLASS winc3;
		winc3.style = CS_HREDRAW | CS_VREDRAW;//基本スタイル
		winc3.lpfnWndProc = GW::GroupListProc;//プロキシ関数へのポインタ
		winc3.cbClsExtra = NULL;//クラス構造体の追加領域（必要なし）
		winc3.cbWndExtra = NULL;//ウィンドウ構造体の追加領域（必要なし）
		winc3.hInstance = Hinstance;//インスタンスハンドル
		winc3.hIcon = LoadIcon(NULL, IDI_APPLICATION);//アイコン(モジュール,名前)
		winc3.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソル（モジュール,名前）
		winc3.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//クライアントエリアの背景色(ブラシのハンドルを渡す)
		winc3.lpszMenuName = NULL;//クラスメニュー
		winc3.lpszClassName = _TEXT("Group");//クラス名
		if (!RegisterClass(&winc3)) return;
	}
	//ウィンドウ生成
	GroupListHwnd = CreateWindow(
		_TEXT("Group"), _TEXT("グループ"),//クラス名、アプリ名
		WS_CHILD | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//ウィンドウのスタイル
		0, 300, 225, 300,//場所x、場所y、幅、高さ
		MainHwnd, NULL,//親ウィンドウハンドル、メニューのハンドル
		Hinstance, NULL//インスタンスハンドル、void*的な奴
	);
	ShowWindow(GroupListHwnd, SW_SHOW);
}

void MW::CreateChatList(bool IsInit){
	if (IsInit) {//クラス登録
		WNDCLASS winc;
		winc.style = CS_HREDRAW | CS_VREDRAW;//基本スタイル
		winc.lpfnWndProc = CW::ChatListProc;//プロキシ関数へのポインタ
		winc.cbClsExtra = NULL;//クラス構造体の追加領域（必要なし）
		winc.cbWndExtra = NULL;//ウィンドウ構造体の追加領域（必要なし）
		winc.hInstance = Hinstance;//インスタンスハンドル
		winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//アイコン(モジュール,名前)
		winc.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソル（モジュール,名前）
		winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//クライアントエリアの背景色(ブラシのハンドルを渡す)
		winc.lpszMenuName = NULL;//クラスメニュー
		winc.lpszClassName = _TEXT("Chat");//クラス名
		if (!RegisterClass(&winc)) return;
	}
	//ウィンドウ生成
	ChatListHwnd = CreateWindow(
		_TEXT("Chat"), _TEXT("チャット"),//クラス名、アプリ名
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//ウィンドウのスタイル
		226, 0, 557, 400,//場所x、場所y、幅、高さ
		MainHwnd, NULL,//親ウィンドウハンドル、メニューのハンドル
		Hinstance, NULL//インスタンスハンドル、void*的な奴
	);
}

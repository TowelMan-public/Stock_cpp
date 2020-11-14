#include "chat.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;//基本スタイル
	winc.lpfnWndProc = MW::WndProc;//プロキシ関数へのポインタ
	winc.cbClsExtra = NULL;//クラス構造体の追加領域（必要なし）
	winc.cbWndExtra = NULL;//ウィンドウ構造体の追加領域（必要なし）
	winc.hInstance = hInstance;//インスタンスハンドル
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//アイコン(モジュール,名前)
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);//マウスカーソル（モジュール,名前）
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//クライアントエリアの背景色(ブラシのハンドルを渡す)
	winc.lpszMenuName = _TEXT("MAIN_MENU");;//クラスメニュー
	winc.lpszClassName = WIN_CLASS_NAME;//クラス名

	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindow(
		WIN_CLASS_NAME, WIN_AP_NAME,//クラス名、アプリ名
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,//ウィンドウのスタイル
		0, 0, 800, 660,//場所x、場所y、幅、高さ
		NULL, NULL,//親ウィンドウハンドル、メニューのハンドル
		hInstance, NULL//インスタンスハンドル、void*的な奴
	);

	if (hwnd == NULL) return 0;
	ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}

	return msg.wParam;
}
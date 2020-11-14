#include "chat.h"

namespace GW {
	//ここで使い、定義する定数
	const int MAX_Y_POINT = 300;
	const int WIDE = 215;
	const int B_HIGHT = 50;
	//privateでグローバルな変数群
	static int nowTopY = 0;
	static int hight = 0;
	static SCROLLINFO scr;
	static AccountS acts;
	static HWND button[5];
	//publicな関数群
	LRESULT CALLBACK GroupListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//privateな関数群
	void Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc);
	void Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Vscroll(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
}

LRESULT CALLBACK GW::GroupListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hh;
	//UpdateWindow(ListHwnd);
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
	case WM_VSCROLL:
		Vscroll(hwnd, msg, wp, lp);
		break;
	case WM_DESTROY:
		//acts.Break();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void GW::Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	if (DB::GetIsLogin()) {
		//内容の取得(DB部から)
		acts = DB::GetGroup();
		for (int i = nowTopY * -1, j = 1; i < acts.Count && j <= 5; i++, j++) {
			button[j-1] = CreateWindow(
				_TEXT("button"), acts[i].name.c_str(),//クラス名、アプリ名
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,//ウィンドウのスタイル
				0, i * B_HIGHT, WIDE, B_HIGHT,//場所x、場所y、幅、高さ
				hwnd, (HMENU)(long long)((long long)j),//親ウィンドウハンドル、メニューのハンドル
				MW::GetHinstance(), NULL//インスタンスハンドル、void*的な奴
			);
		}
	}
	HDC hdc;
	PAINTSTRUCT ps;
	//高さの取得
	hight = acts.Count;
	//スクロールバー
	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_ALL;
	scr.nMin = 0;
	scr.nMax = hight;
	scr.nPage = 5;

	SetScrollInfo(hwnd, SB_VERT, &scr, TRUE);

	if (DB::GetIsLogin()) {

	}
}

void GW::Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc) {
	if (DB::GetIsLogin()) {
		for (int i = 0; i < 5; i++) {
			SendMessage(button[i], WM_DESTROY, 0, 0);
			//DestroyWindow(button[i]);
			button[i] = NULL;
		}
		for (int i = nowTopY * -1, j = 1; i < acts.Count && j <= 5; i++, j++) {
			button[j-1] = CreateWindow(
				_TEXT("button"), acts[i].name.c_str(),//クラス名、アプリ名
				WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,//ウィンドウのスタイル
				0, i * B_HIGHT, WIDE, B_HIGHT,//場所x、場所y、幅、高さ
				hwnd, (HMENU)(j),//親ウィンドウハンドル、メニューのハンドル
				MW::GetHinstance(), NULL//インスタンスハンドル、void*的な奴
			);
		}
	}
}

void GW::Vscroll(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (LOWORD(wp)) {
	case SB_TOP:
		scr.nPos = scr.nMin;
		break;
	case SB_BOTTOM:
		scr.nPos = scr.nMax;
		break;
	case SB_LINEUP:
		if (scr.nPos) scr.nPos--;
		break;
	case SB_LINEDOWN:
		if (scr.nPos < scr.nMax - 1) scr.nPos++;
		break;
	case SB_PAGEUP:
		scr.nPos -= scr.nPage;
		break;
	case SB_PAGEDOWN:
		scr.nPos += scr.nPage;
		break;
	case SB_THUMBPOSITION:
		scr.nPos = HIWORD(wp);
		break;
	}
	nowTopY = 0 - scr.nPos * 100;
	SetScrollInfo(hwnd, SB_VERT, &scr, TRUE);
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void GW::Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	int idx;
	int id;
	if (LOWORD(wp) >= 1 && LOWORD(wp) <= 5) {
		idx = (nowTopY * -1) + (LOWORD(wp) - 1);
		id = DB::GetTalkByGroup(acts[idx].number);
		MW::UpdateChat(id);
	}
}
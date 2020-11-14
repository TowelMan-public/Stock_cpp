#include "chat.h"

namespace CW {
	//ここで使い、定義する定数
	const int MAX_Y_POINT = 400;
	const int WIDE = 400;
	const int MY_SELF_X = 157;
	const int OTHER_X = 0;
	//privateでグローバルな変数群
	static int talkID = -1;
	static int nowTopY = 0;//どんどん負の数になっていく・0より大きくならない
	static int hight=0;
	static SCROLLINFO scr;
	//publicな関数群
	LRESULT CALLBACK ChatListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void SetTalkID(int id);
	int GetTalkID(void);
	//privateな関数群
	void Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc);
	void Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Vscroll(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
}

LRESULT CALLBACK CW::ChatListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
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
	case WM_VSCROLL:
		Vscroll(hwnd, msg, wp, lp);
		return 0;
		break;
	case WM_COMMAND:
		Comand(hwnd, msg, wp, lp);
		InvalidateRect(hwnd, NULL, TRUE);//再描画（何か押されたら必ず行う）
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void CW::SetTalkID(int id){
	talkID = id;
}

int CW::GetTalkID(void) {
	return talkID;
}

void CW::Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	//高さの取得
	hdc = BeginPaint(hwnd, &ps);
	Paint(hwnd, msg, wp, lp, hdc);
	EndPaint(hwnd, &ps);
	//スクロールバー
	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_ALL;
	scr.nMin = 0;
	scr.nMax = hight/4+((hight%4)!=0);
	scr.nPage = 4;

	SetScrollInfo(hwnd, SB_VERT, &scr, TRUE);
}

void CW::Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc) {//ここで更新等も行う　主な処理を行う
	if (talkID == -1) return;
	RECT* rest = new RECT;
	std::wstring text;
	//該当するチャットデータをロードしてもらう
	DB::Load(talkID);
	for (int y = nowTopY, i = 0; i < DB::GetChatCount(); i++) {
		//wideの決定
		if (DB::GetChatIsMe()) {//自分
			rest->left = MY_SELF_X;
			rest->right = WIDE + MY_SELF_X;			
			text = _TEXT("");
		}
		else {//他者
			rest->left = OTHER_X;
			rest->right = WIDE;
			text = DB::GerChatUser() + _TEXT("\n");
		}		
		//hightについて
		rest->bottom = 0;		
		rest->top = y;
		//TODO textに内容を追加する
		text += DB::GetChat();
		DrawText(hdc, text.c_str(), text.length(), rest, DT_CALCRECT | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
		DrawText(hdc, text.c_str(), text.length(), rest, DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
		DB::NextChat();
		//全体の高さの更新
		y=hight = rest->bottom + 20;
	}
	DB::ChatEnd();
	delete rest;
	return;
}

void CW::Vscroll(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
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
	nowTopY = 0 - scr.nPos*100;
	SetScrollInfo(hwnd, SB_VERT, &scr, TRUE);
	InvalidateRect(hwnd, NULL, TRUE);
	UpdateWindow(hwnd);
}

void CW::Comand(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

}
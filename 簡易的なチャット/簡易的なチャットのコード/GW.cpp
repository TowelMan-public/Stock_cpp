#include "chat.h"

namespace GW {
	//�����Ŏg���A��`����萔
	const int MAX_Y_POINT = 300;
	const int WIDE = 215;
	const int B_HIGHT = 50;
	//private�ŃO���[�o���ȕϐ��Q
	static int nowTopY = 0;
	static int hight = 0;
	static SCROLLINFO scr;
	static AccountS acts;
	static HWND button[5];
	//public�Ȋ֐��Q
	LRESULT CALLBACK GroupListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//private�Ȋ֐��Q
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
		//InvalidateRect(hwnd, NULL, TRUE);//�ĕ`��i���������ꂽ��K���s���j
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
		//���e�̎擾(DB������)
		acts = DB::GetGroup();
		for (int i = nowTopY * -1, j = 1; i < acts.Count && j <= 5; i++, j++) {
			button[j-1] = CreateWindow(
				_TEXT("button"), acts[i].name.c_str(),//�N���X���A�A�v����
				WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,//�E�B���h�E�̃X�^�C��
				0, i * B_HIGHT, WIDE, B_HIGHT,//�ꏊx�A�ꏊy�A���A����
				hwnd, (HMENU)(long long)((long long)j),//�e�E�B���h�E�n���h���A���j���[�̃n���h��
				MW::GetHinstance(), NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
			);
		}
	}
	HDC hdc;
	PAINTSTRUCT ps;
	//�����̎擾
	hight = acts.Count;
	//�X�N���[���o�[
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
				_TEXT("button"), acts[i].name.c_str(),//�N���X���A�A�v����
				WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,//�E�B���h�E�̃X�^�C��
				0, i * B_HIGHT, WIDE, B_HIGHT,//�ꏊx�A�ꏊy�A���A����
				hwnd, (HMENU)(j),//�e�E�B���h�E�n���h���A���j���[�̃n���h��
				MW::GetHinstance(), NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
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
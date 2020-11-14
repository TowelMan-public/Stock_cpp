#include "chat.h"

namespace CW {
	//�����Ŏg���A��`����萔
	const int MAX_Y_POINT = 400;
	const int WIDE = 400;
	const int MY_SELF_X = 157;
	const int OTHER_X = 0;
	//private�ŃO���[�o���ȕϐ��Q
	static int talkID = -1;
	static int nowTopY = 0;//�ǂ�ǂ񕉂̐��ɂȂ��Ă����E0���傫���Ȃ�Ȃ�
	static int hight=0;
	static SCROLLINFO scr;
	//public�Ȋ֐��Q
	LRESULT CALLBACK ChatListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void SetTalkID(int id);
	int GetTalkID(void);
	//private�Ȋ֐��Q
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
		InvalidateRect(hwnd, NULL, TRUE);//�ĕ`��i���������ꂽ��K���s���j
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
	//�����̎擾
	hdc = BeginPaint(hwnd, &ps);
	Paint(hwnd, msg, wp, lp, hdc);
	EndPaint(hwnd, &ps);
	//�X�N���[���o�[
	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_ALL;
	scr.nMin = 0;
	scr.nMax = hight/4+((hight%4)!=0);
	scr.nPage = 4;

	SetScrollInfo(hwnd, SB_VERT, &scr, TRUE);
}

void CW::Paint(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, HDC hdc) {//�����ōX�V�����s���@��ȏ������s��
	if (talkID == -1) return;
	RECT* rest = new RECT;
	std::wstring text;
	//�Y������`���b�g�f�[�^�����[�h���Ă��炤
	DB::Load(talkID);
	for (int y = nowTopY, i = 0; i < DB::GetChatCount(); i++) {
		//wide�̌���
		if (DB::GetChatIsMe()) {//����
			rest->left = MY_SELF_X;
			rest->right = WIDE + MY_SELF_X;			
			text = _TEXT("");
		}
		else {//����
			rest->left = OTHER_X;
			rest->right = WIDE;
			text = DB::GerChatUser() + _TEXT("\n");
		}		
		//hight�ɂ���
		rest->bottom = 0;		
		rest->top = y;
		//TODO text�ɓ��e��ǉ�����
		text += DB::GetChat();
		DrawText(hdc, text.c_str(), text.length(), rest, DT_CALCRECT | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
		DrawText(hdc, text.c_str(), text.length(), rest, DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
		DB::NextChat();
		//�S�̂̍����̍X�V
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
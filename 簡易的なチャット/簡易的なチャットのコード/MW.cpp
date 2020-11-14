#include "chat.h"

namespace MW {
	//�����Ŏg���A��`����萔
	const int BUTTON = 10001;
	const int EDIT = 10002;
	//private�ŃO���[�o���ȕϐ��Q
	static HWND MainHwnd = NULL;
	static HWND ChatListHwnd = NULL;
	static HWND UserListHwnd = NULL;
	static HWND GroupListHwnd = NULL;
	static HWND EditHwnd = NULL;
	static HWND ButtonHwnd = NULL;
	static HINSTANCE Hinstance;
	//public�Ȋ֐��Q
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	HINSTANCE GetHinstance(void);
	void UpdateUser(void);
	void UpdateGroup(void);
	void UpdateChat(int talkID);
	//private�Ȋ֐��Q
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
	//�`���b�g�Ƀg�[�NID�𔽉f������
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
		//InvalidateRect(hwnd, NULL, TRUE);//�ĕ`��i���������ꂽ��K���s���j
		break;
	case WM_DESTROY:
		DB::End();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

void MW::Create(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	//Hinstance�̃Z�b�g
	Hinstance = ((HINSTANCE)GetModuleHandle(0));
	//MainHwnd
	MainHwnd = hwnd;
	//���[�U�[���X�g�̍쐬
	CreateUserList();
	//�O���[�v���X�g�̍쐬
	CreateGroupList();
	//�`���b�g���X�g�̍쐬
	CreateChatList();
	//���M�{�^���̍쐬
	ButtonHwnd = CreateWindow(
		_TEXT("BUTTON"), _TEXT("���M"),//�N���X���A�A�v����
		WS_CHILD | WS_VISIBLE | WS_BORDER,//�E�B���h�E�̃X�^�C��
		226, 400, 100, 50,//�ꏊx�A�ꏊy�A���A����
		hwnd, (HMENU)BUTTON,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		Hinstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);
	//���M�{�b�N�X�̍쐬
	EditHwnd = CreateWindow(
		_TEXT("EDIT"), NULL,//�N���X���A�A�v����
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL,//�E�B���h�E�̃X�^�C��
		226, 450, 557, 150,//�ꏊx�A�ꏊy�A���A����
		hwnd, (HMENU)EDIT,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		Hinstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
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
		//���O�C�����Z��łȂ��Ƃ��̂�
		if (!DB::GetIsLogin()) {
			if (DB::HaveMyID())//�o�^����Ă�
				DialogBox(Hinstance, MAKEINTRESOURCE(IDD_DIALOG_LOGIN), hwnd, Dialog::LoginProc);
			else//�o�^������Ă��Ȃ�
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
	case WMM_CON_INFO://���b�Z�[�W�{�b�N�X�ŏ���\��
		if (DB::GetIsLogin() && CW::GetTalkID() != -1) {
			if (DB::GetIsGroupByID(CW::GetTalkID())) {
				act = DB::GetGroupByID(CW::GetTalkID());
				infoStr = _TEXT("ID:") + std::to_wstring(act.number);
				infoStr += _TEXT("�O���[�v��:") + act.name;
			}
			else {
				act = DB::GetUserByID(CW::GetTalkID());
				infoStr = _TEXT("ID:") + std::to_wstring(act.number) + _TEXT("\n");
				infoStr += _TEXT("���[�U�[��:") + act.name;
			}
			MessageBox(hwnd, infoStr.c_str(), _TEXT("���"), MB_OK);
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
	case BUTTON://���M
		//������̎擾
		if (DB::GetIsLogin() && GetWindowTextLength(EditHwnd) != 0 && CW::GetTalkID() != -1) {//����������A���悪�I������Ă�
			strText = new WCHAR[GetWindowTextLength(EditHwnd) + 2];//+2�͗]���ɂ݂Ă��邩��
			GetWindowText(EditHwnd, strText, GetWindowTextLength(EditHwnd) + 2);
			//�g�[�NID�ɑ��M����
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
	if (IsInit) {//�N���X�o�^
		WNDCLASS winc2;
		winc2.style = CS_HREDRAW | CS_VREDRAW;//��{�X�^�C��
		winc2.lpfnWndProc = UW::UserListProc;//�v���L�V�֐��ւ̃|�C���^
		winc2.cbClsExtra = NULL;//�N���X�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc2.cbWndExtra = NULL;//�E�B���h�E�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc2.hInstance = Hinstance;//�C���X�^���X�n���h��
		winc2.hIcon = LoadIcon(NULL, IDI_APPLICATION);//�A�C�R��(���W���[��,���O)
		winc2.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\���i���W���[��,���O�j
		winc2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�N���C�A���g�G���A�̔w�i�F(�u���V�̃n���h����n��)
		winc2.lpszMenuName = NULL;//�N���X���j���[
		winc2.lpszClassName = _TEXT("Ueser");//�N���X��
		if (!RegisterClass(&winc2)) return;
	}
	//�E�B���h�E����
	UserListHwnd = CreateWindow(
		_TEXT("Ueser"), _TEXT("���[�U�["),//�N���X���A�A�v����
		WS_CHILD | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//�E�B���h�E�̃X�^�C��
		0, 0, 225, 300,//�ꏊx�A�ꏊy�A���A����
		MainHwnd, NULL,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		Hinstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);
	ShowWindow(UserListHwnd, SW_SHOW);
}

void MW::CreateGroupList(bool IsInit){
	if (IsInit) {//�N���X�o�^
		WNDCLASS winc3;
		winc3.style = CS_HREDRAW | CS_VREDRAW;//��{�X�^�C��
		winc3.lpfnWndProc = GW::GroupListProc;//�v���L�V�֐��ւ̃|�C���^
		winc3.cbClsExtra = NULL;//�N���X�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc3.cbWndExtra = NULL;//�E�B���h�E�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc3.hInstance = Hinstance;//�C���X�^���X�n���h��
		winc3.hIcon = LoadIcon(NULL, IDI_APPLICATION);//�A�C�R��(���W���[��,���O)
		winc3.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\���i���W���[��,���O�j
		winc3.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�N���C�A���g�G���A�̔w�i�F(�u���V�̃n���h����n��)
		winc3.lpszMenuName = NULL;//�N���X���j���[
		winc3.lpszClassName = _TEXT("Group");//�N���X��
		if (!RegisterClass(&winc3)) return;
	}
	//�E�B���h�E����
	GroupListHwnd = CreateWindow(
		_TEXT("Group"), _TEXT("�O���[�v"),//�N���X���A�A�v����
		WS_CHILD | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//�E�B���h�E�̃X�^�C��
		0, 300, 225, 300,//�ꏊx�A�ꏊy�A���A����
		MainHwnd, NULL,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		Hinstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);
	ShowWindow(GroupListHwnd, SW_SHOW);
}

void MW::CreateChatList(bool IsInit){
	if (IsInit) {//�N���X�o�^
		WNDCLASS winc;
		winc.style = CS_HREDRAW | CS_VREDRAW;//��{�X�^�C��
		winc.lpfnWndProc = CW::ChatListProc;//�v���L�V�֐��ւ̃|�C���^
		winc.cbClsExtra = NULL;//�N���X�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc.cbWndExtra = NULL;//�E�B���h�E�\���̂̒ǉ��̈�i�K�v�Ȃ��j
		winc.hInstance = Hinstance;//�C���X�^���X�n���h��
		winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//�A�C�R��(���W���[��,���O)
		winc.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\���i���W���[��,���O�j
		winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�N���C�A���g�G���A�̔w�i�F(�u���V�̃n���h����n��)
		winc.lpszMenuName = NULL;//�N���X���j���[
		winc.lpszClassName = _TEXT("Chat");//�N���X��
		if (!RegisterClass(&winc)) return;
	}
	//�E�B���h�E����
	ChatListHwnd = CreateWindow(
		_TEXT("Chat"), _TEXT("�`���b�g"),//�N���X���A�A�v����
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL & ~WS_MAXIMIZEBOX,//�E�B���h�E�̃X�^�C��
		226, 0, 557, 400,//�ꏊx�A�ꏊy�A���A����
		MainHwnd, NULL,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		Hinstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);
}

#include "chat.h"

namespace Dialog {
	//�����Ŏg���A��`����萔
	//private�ŃO���[�o���ȕϐ��Q
	//public�Ȋ֐��Q
	INT_PTR CALLBACK LoginProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK NewProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK MakeGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//INT_PTR CALLBACK DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//private�Ȋ֐��Q
}

INT_PTR CALLBACK Dialog::LoginProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	LPWSTR pass;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) == 0) {//�`�F�b�N
				MessageBox(hwnd, _TEXT("��������͂��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			pass = new WCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_PASS), pass, sizeof(WCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + 2));
			//���O�C��������
			DB::Login(pass);
			MW::UpdateUser();
			MW::UpdateGroup();
			delete[] pass;
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Dialog::NewProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	LPWSTR name, pass;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) == 0 || GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) == 0) {//�`�F�b�N
				MessageBox(hwnd, _TEXT("��������͂��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			name = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_UESER), name, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) + 2));
			pass = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_PASS), pass, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + 2));
			//���[�U�[�o�^�ƃ��O�C�������Ă��炤
			DB::MakeAccount(name,pass);
			delete[] name, pass;
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Dialog::AddUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	PTCHAR num;
	int id;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) == 0) {//�`�F�b�N
				MessageBox(hwnd, _TEXT("��������͂��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			num = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_UESER_NO), num, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) + 2));
			id = _wtoi(num);//int�ɕϊ�
			//���[�U�[�̒ǉ�
			DB::AddUser(id);
			delete[] num;
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Dialog::MakeGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	PTCHAR name;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			//IDC_EDIT_GROUP_NAME
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) == 0) {//�`�F�b�N
				MessageBox(hwnd, _TEXT("��������͂��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			name = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME), name, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) + 2));
			//�O���[�v�̍쐬
			DB::MakeGroup(name);
			delete[] name;
			MW::UpdateGroup();
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Dialog::AddGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	PTCHAR num;
	int id;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			//IDC_EDIT_GROUP_NAME
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) == 0) {//�`�F�b�N
				MessageBox(hwnd, _TEXT("��������͂��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			num = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO), num, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) + 2));
			id = _wtoi(num);//int�ɕϊ�
			//�O���[�v�̒ǉ�
			DB::AddGroup(id);
			delete[] num;
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Dialog::DUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	PTCHAR num;
	static bool IsInit = true;
	static int idx = -1;
	static AccountS acts;
	switch (msg) {
	case WM_PAINT:
		//�������iWM_CREATE�͌Ă΂�Ȃ��j
		if (IsInit) {
			IsInit = false;
			idx = -1;
			acts = DB::GetDuser();
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, FALSE, 0);//�ĕ`�揈����~
			for (int i = 0; i < acts.Count; i++) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST),
					LB_INSERTSTRING, i, (LPARAM)acts[i].name.c_str());
			}
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, TRUE, 0);//�ĕ`�揈���ĊJ
		}
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_LIST:
			idx = SendMessage(GetDlgItem(hwnd, IDC_LIST), LB_GETCURSEL, 0, 0);
			break;
		case IDOK:
			if (idx > acts.Count || idx == -1) {
				MessageBox(hwnd, _TEXT("���X�g�{�b�N�X�����I�����Ă���ɂ��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}			
			DB::AcceptUser(acts[idx].number);
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			IsInit = true;
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		IsInit = true;
		return TRUE;
	}
	return FALSE;
}


INT_PTR CALLBACK Dialog::DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
	HDC hdc;
	PAINTSTRUCT ps;
	PTCHAR num;
	static bool IsInit = true;
	static int idx = -1;
	static AccountS acts;
	static int groupID;
	Accnt act;
	switch (msg) {
	case WM_PAINT:
		//�������iWM_CREATE�͌Ă΂�Ȃ��j
		if (IsInit) {
			IsInit = false;
			idx = -1;
			if (!DB::GetIsGroupByID(CW::GetTalkID())) {
				EndDialog(hwnd, IDOK);
				IsInit = true;
				return TRUE;
			}
			groupID = DB::GetGroupByID(CW::GetTalkID()).number;
			acts = DB::GetDgroup(groupID);
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, FALSE, 0);//�ĕ`�揈����~
			for (int i = 0; i < acts.Count; i++) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST),
					LB_INSERTSTRING, i, (LPARAM)acts[i].name.c_str());
			}
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, TRUE, 0);//�ĕ`�揈���ĊJ
		}
		hdc = BeginPaint(hwnd, &ps);
		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_LIST:
			idx = SendMessage(GetDlgItem(hwnd, IDC_LIST), LB_GETCURSEL, 0, 0);
			break;
		case IDOK:
			if (idx > acts.Count || idx == -1) {
				MessageBox(hwnd, _TEXT("���X�g�{�b�N�X�����I�����Ă���ɂ��Ă�������"), _TEXT("����"), MB_OK);
				break;
			}
			DB::AcceptGroup(groupID, acts[idx].number);
			//���̂܂ܑ����ĕ���
		case IDCANCEL:
			EndDialog(hwnd, IDOK);
			IsInit = true;
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, IDOK);
		IsInit = true;
		return TRUE;
	}
	return FALSE;
}

#include "chat.h"

namespace Dialog {
	//ここで使い、定義する定数
	//privateでグローバルな変数群
	//publicな関数群
	INT_PTR CALLBACK LoginProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK NewProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK MakeGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//INT_PTR CALLBACK DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	//privateな関数群
}

INT_PTR CALLBACK Dialog::LoginProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;
	LPWSTR pass;
	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) == 0) {//チェック
				MessageBox(hwnd, _TEXT("文字を入力してください"), _TEXT("注意"), MB_OK);
				break;
			}
			pass = new WCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_PASS), pass, sizeof(WCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + 2));
			//ログインをする
			DB::Login(pass);
			MW::UpdateUser();
			MW::UpdateGroup();
			delete[] pass;
			//そのまま続けて閉じる
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
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) == 0 || GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) == 0) {//チェック
				MessageBox(hwnd, _TEXT("文字を入力してください"), _TEXT("注意"), MB_OK);
				break;
			}
			name = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_UESER), name, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER)) + 2));
			pass = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_PASS), pass, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PASS)) + 2));
			//ユーザー登録とログインをしてもらう
			DB::MakeAccount(name,pass);
			delete[] name, pass;
			//そのまま続けて閉じる
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
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) == 0) {//チェック
				MessageBox(hwnd, _TEXT("文字を入力してください"), _TEXT("注意"), MB_OK);
				break;
			}
			num = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_UESER_NO), num, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_UESER_NO)) + 2));
			id = _wtoi(num);//intに変換
			//ユーザーの追加
			DB::AddUser(id);
			delete[] num;
			//そのまま続けて閉じる
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
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			//IDC_EDIT_GROUP_NAME
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) == 0) {//チェック
				MessageBox(hwnd, _TEXT("文字を入力してください"), _TEXT("注意"), MB_OK);
				break;
			}
			name = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME), name, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NAME)) + 2));
			//グループの作成
			DB::MakeGroup(name);
			delete[] name;
			MW::UpdateGroup();
			//そのまま続けて閉じる
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
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDOK:
			//IDC_EDIT_GROUP_NAME
			if (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) == 0) {//チェック
				MessageBox(hwnd, _TEXT("文字を入力してください"), _TEXT("注意"), MB_OK);
				break;
			}
			num = new TCHAR[(long long)GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) + (long long)2];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO), num, sizeof(TCHAR) * (GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_GROUP_NO)) + 2));
			id = _wtoi(num);//intに変換
			//グループの追加
			DB::AddGroup(id);
			delete[] num;
			//そのまま続けて閉じる
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
		//初期化（WM_CREATEは呼ばれない）
		if (IsInit) {
			IsInit = false;
			idx = -1;
			acts = DB::GetDuser();
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, FALSE, 0);//再描画処理停止
			for (int i = 0; i < acts.Count; i++) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST),
					LB_INSERTSTRING, i, (LPARAM)acts[i].name.c_str());
			}
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, TRUE, 0);//再描画処理再開
		}
		hdc = BeginPaint(hwnd, &ps);
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_LIST:
			idx = SendMessage(GetDlgItem(hwnd, IDC_LIST), LB_GETCURSEL, 0, 0);
			break;
		case IDOK:
			if (idx > acts.Count || idx == -1) {
				MessageBox(hwnd, _TEXT("リストボックスから一つ選択してからにしてください"), _TEXT("注意"), MB_OK);
				break;
			}			
			DB::AcceptUser(acts[idx].number);
			//そのまま続けて閉じる
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
		//初期化（WM_CREATEは呼ばれない）
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
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, FALSE, 0);//再描画処理停止
			for (int i = 0; i < acts.Count; i++) {
				SendMessage(GetDlgItem(hwnd, IDC_LIST),
					LB_INSERTSTRING, i, (LPARAM)acts[i].name.c_str());
			}
			SendMessage(GetDlgItem(hwnd, IDC_LIST), WM_SETREDRAW, TRUE, 0);//再描画処理再開
		}
		hdc = BeginPaint(hwnd, &ps);
		//ここ
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wp)) {
		case IDC_LIST:
			idx = SendMessage(GetDlgItem(hwnd, IDC_LIST), LB_GETCURSEL, 0, 0);
			break;
		case IDOK:
			if (idx > acts.Count || idx == -1) {
				MessageBox(hwnd, _TEXT("リストボックスから一つ選択してからにしてください"), _TEXT("注意"), MB_OK);
				break;
			}
			DB::AcceptGroup(groupID, acts[idx].number);
			//そのまま続けて閉じる
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

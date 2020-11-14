#pragma once
/*�C���N���[�h������̂���*/
#include <windows.h>
#include <math.h>
#include <string>
#include <atlstr.h>
#include <xstring>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn/Statement.h>
#include <cppconn/ResultSet.h>

/*�V���{������*/
#include "resource.h"
#define WIN_CLASS_NAME _TEXT("for_chat")
#define WIN_AP_NAME _TEXT("�`���b�g")

/*�\���́E�N���X*/

//���[�U�[�E�O���[�v�Ǘ��p
struct Accnt
{
	int number;
	std::wstring name;
};

class AccountS {
public:
	int Count;//�� 1�`  0�Ȃ疳
public:
	AccountS(const AccountS& m);
	AccountS(int idx);
	AccountS(void);
	~AccountS();
	void Make(int idx);
	void Break(void);
	Accnt& operator[](int idx);
	void operator=(AccountS former);
private:
	Accnt* m_acnt;//�����Ȃ��Ƃ���nullptr������
};

/*���̏ꏊ����Q�Ƃ���K�v�̂���Apublic�Ȋ֐��Q�i���O��Ԗ��͗��́E�\�[�X�t�@�C���͖��O��Ԃ̖��O���g���j*/
namespace MW {
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	HINSTANCE GetHinstance(void);
	void UpdateUser(void);
	void UpdateGroup(void);
	void UpdateChat(int talkID);
}

namespace UW {
	LRESULT CALLBACK UserListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
}

namespace GW {
	LRESULT CALLBACK GroupListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
}

namespace CW{
	LRESULT CALLBACK ChatListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void SetTalkID(int id);
	int GetTalkID(void);
}

namespace Dialog {
	INT_PTR CALLBACK LoginProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK NewProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK MakeGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK AddGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DUeserProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	INT_PTR CALLBACK DGroupProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
}

namespace DB {
	void End(void);
	bool GetIsLogin(void);
	bool GetIsGroupByID(int talkID);
	void SendChat(int talkID, std::wstring text);
	Accnt GetGroupByID(int talkID);
	Accnt GetUserByID(int talkID);
	AccountS GetUser(void);
	int GetTalkByUser(int userD);
	AccountS GetGroup(void);
	int GetTalkByGroup(int groupID);
	void Load(int talkID);
	bool GetChatIsMe(void);
	std::wstring GerChatUser(void);
	std::wstring GetChat(void);
	void NextChat(void);
	void Login(std::wstring pass);
	void MakeAccount(std::wstring name, std::wstring pass);
	void AddUser(int id);
	void MakeGroup(std::wstring name);
	void AddGroup(int id);
	AccountS GetDuser(void);
	void AcceptUser(int userID);
	void ChatEnd(void);
	bool HaveMyID(void);
	int GetChatCount();
	AccountS GetDgroup(int groupID);
	void AcceptGroup(int groupID, int userID);
}
#pragma once
#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <string>
#include <imagehlp.h>
#include <fstream>
#include "lpfnWndProc.h"

#pragma	comment(lib, "imagehlp.lib")

struct MyError {
	int error;
	HWND hWnd;
};

struct BASIC_KIBAN {//�P����
	CString word;//�P�ꖼ
	CString junl;//���ޖ�
	CString info;//���e
	CString file;//�摜��
};
class KIBAN {
public:
	TCHAR* word;//�P�ꖼ
	TCHAR* junl;//���ޖ�
	TCHAR* info;//���e������
	TCHAR* file;//�摜�p�X��
	size_t size_w;//�P�ꖼ�̃T�C�Y
	size_t size_j;//���ޖ��̃T�C�Y
	size_t size_i;//���e�̃T�C�Y
	size_t size_f;//�摜�p�X���̃T�C�Y
	bool TF;//�O�����ǂ݂���ł悢���ۂ�(true:�ǂݍ���ł悢 fales:�ǂݍ���ł͂Ȃ�Ȃ�)
public:
	KIBAN(void);//�R���X�g���N�^
	~KIBAN(void);//�f�X�g���N�^
	void set(void);//���Z�b�g
	void set(TCHAR* str, size_t sz);//�P����̃Z�b�g�i�ЂƂÁj
	int virtual run(TCHAR* a = NULL, size_t sz = 0);//�o�^���̎��s(�����w�肵�Ȃ�������o�^�E�w�肵����ύX�̎��s�j
	void operator=(BASIC_KIBAN date);//�P����̃Z�b�g�i�\���̂��g���Ĉꊇ�ɃZ�b�g�j
};

class KIBAN_KENSAKU :
	public KIBAN {
private:
	CString* Cjunl;//���ޖ��i�������p�j
	int Count;
public:
	KIBAN_KENSAKU(void);//�R���X�g���N�^
	~KIBAN_KENSAKU(void);//�f�X�g���N�^
	int virtual run(void);//�����̎��s�i�I�[�o�[���C�h�j
	int GetStats(void);//��������␔�̎擾��
	BASIC_KIBAN operator[](int a);//�w�肳�ꂽ����Ԃ��i1�`Count�̊ԂŎw��)
};
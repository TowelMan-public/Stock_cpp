#pragma once
/*�C���N���[�h������̂���*/
#include <windows.h>
#include <math.h>
#include <string>
#include <atlstr.h>

/*�V���{������*/
#define WIN_CLASS_NAME TEXT("for_dentaku")
#define Win_AP_NAME TEXT("dentaku")

/*�G�f�B�b�g�{�b�N�X*/
#define EDITBOX 70
/*�{�^��*/
const int BUTTON_NUM[] = {0,1,2,3,4,5,6,7,8,9};
#define BUTTON_C 10
#define BUTTON_KAKE 11
#define BUTTON_WARI 12
#define BUTTON_TASI 13
#define BUTTON_HIKI 14
#define BUTTON_BACK 15
#define BUTTON_EQ 16
#define BUTTON_TEN 17
#define BUTTON_ZYOU 18
#define BUTTON_ROOT 19
#define BUTTON_SIN 20
#define BUTTON_COS 21
#define BUTTON_TAN 22
#define BUTTON_START 23
#define BUTTON_END 24
/*�v�Z��̃G���[*/
#define CLCT_ERROR_WARI 1
#define CLCT_ERROR_ROOT 2
#define CLCT_ERROR_FORMULA -1

/*�����N*/
//libm.a

/*�����Ŏg���֐�����*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

/*�K�v�ȃN���X*/
class Calculation {//�Z�b�g���ꂽ�����̌v�Z������
public:
	bool IsError;
	int ErrorNum;//0(NULL)�Ȃ�G���[�Ȃ�
public:
	Calculation(void);//������
	~Calculation(void);//�㏈��
	void Set(std::string str);//���̃Z�b�g
	double Do(void);//�Z�b�g���ꂽ���̌v�Z
private:
	std::string m_formula;
	int m_idx;//�v�Z������Ƃ��ɁA���ɓǂݍ��ޏ��ɂ��ẴC���f�b�N�X
private:
	void Judgment(void);//���̔���
	double GetAns(void);//���̌v�Z�̎��s�i���̒l�����߂�j
	double GetArgumentValue(void);//���̒l�����߂�
	double GetValue(void);
};
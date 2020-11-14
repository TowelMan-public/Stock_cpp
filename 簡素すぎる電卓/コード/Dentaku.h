#pragma once
/*インクルードするものたち*/
#include <windows.h>
#include <math.h>
#include <string>
#include <atlstr.h>

/*シンボルたち*/
#define WIN_CLASS_NAME TEXT("for_dentaku")
#define Win_AP_NAME TEXT("dentaku")

/*エディットボックス*/
#define EDITBOX 70
/*ボタン*/
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
/*計算上のエラー*/
#define CLCT_ERROR_WARI 1
#define CLCT_ERROR_ROOT 2
#define CLCT_ERROR_FORMULA -1

/*リンク*/
//libm.a

/*ここで使う関数たち*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

/*必要なクラス*/
class Calculation {//セットされた数式の計算をする
public:
	bool IsError;
	int ErrorNum;//0(NULL)ならエラーなし
public:
	Calculation(void);//初期化
	~Calculation(void);//後処理
	void Set(std::string str);//式のセット
	double Do(void);//セットされた式の計算
private:
	std::string m_formula;
	int m_idx;//計算をするときに、次に読み込む所についてのインデックス
private:
	void Judgment(void);//式の判定
	double GetAns(void);//式の計算の実行（式の値を求める）
	double GetArgumentValue(void);//項の値を求める
	double GetValue(void);
};
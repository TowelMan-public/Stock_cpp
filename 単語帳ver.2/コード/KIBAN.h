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

struct BASIC_KIBAN {//単語情報
	CString word;//単語名
	CString junl;//分類名
	CString info;//内容
	CString file;//画像名
};
class KIBAN {
public:
	TCHAR* word;//単語名
	TCHAR* junl;//分類名
	TCHAR* info;//内容文字列
	TCHAR* file;//画像パス等
	size_t size_w;//単語名のサイズ
	size_t size_j;//分類名のサイズ
	size_t size_i;//内容のサイズ
	size_t size_f;//画像パス等のサイズ
	bool TF;//外部が読みこんでよいか否か(true:読み込んでよい fales:読み込んではならない)
public:
	KIBAN(void);//コンストラクタ
	~KIBAN(void);//デストラクタ
	void set(void);//リセット
	void set(TCHAR* str, size_t sz);//単語情報のセット（ひとつづ）
	int virtual run(TCHAR* a = NULL, size_t sz = 0);//登録等の実行(何も指定しなかったら登録・指定したら変更の実行）
	void operator=(BASIC_KIBAN date);//単語情報のセット（構造体を使って一括にセット）
};

class KIBAN_KENSAKU :
	public KIBAN {
private:
	CString* Cjunl;//分類名（検索候補用）
	int Count;
public:
	KIBAN_KENSAKU(void);//コンストラクタ
	~KIBAN_KENSAKU(void);//デストラクタ
	int virtual run(void);//検索の実行（オーバーライド）
	int GetStats(void);//総検索候補数の取得等
	BASIC_KIBAN operator[](int a);//指定された情報を返す（1～Countの間で指定)
};
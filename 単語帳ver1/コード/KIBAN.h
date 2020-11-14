#pragma once
#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <string>
#include "lpfnWndProc.h"

struct MyError {
	int error;
	HWND hWnd;
};

class KIBAN {
public:
	KIBAN();
	~KIBAN();
	int Set(TCHAR* baf, size_t size);
	void Set(void);
	int Ran(HWND hWnd);
public:
	TCHAR* word;
	TCHAR* junl;
	TCHAR* file;
	TCHAR* info;
	size_t size_w;
	size_t size_j;
	size_t size_f;
	size_t size_i;
	bool TF;
private:
	int Research(void);
	int Register(void);
private:
	int flag;
};



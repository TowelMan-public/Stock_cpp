#pragma once

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <string>
#include <gdiplus.h>
#include <olectl.h>
#include "resource.h"
#include "KIBAN.h"

#pragma	comment(lib, "Gdiplus.lib")

//•K—v‚È’è‹`•¨
#define ID_B_1 1
#define ID_B_2 2
#define ID_E_1 3
#define ID_E_2 4
#define ID_E_3 5
#define TCHAR_WORD 50
#define TCHAR_INFO 2054
#define TCHAR_JUNL 50
#define TCHAR_FILE 256


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK Dia_rProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK Dia_iProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

IStream* getFileIStream(const char* lpszPath, HWND hWnd);
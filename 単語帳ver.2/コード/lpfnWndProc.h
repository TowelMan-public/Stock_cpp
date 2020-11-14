#pragma once

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <string>
#include <gdiplus.h>
#include <olectl.h>
#include <string>
#include "resource.h"
#include "KIBAN.h"

#pragma	comment(lib, "Gdiplus.lib")

//�K�v�Ȓ�`��
#define ID_B_1 7701
#define ID_B_2 7702
#define ID_B_3 7703
#define ID_E 7704
#define TCHAR_WORD 50
#define TCHAR_INFO 2054
#define TCHAR_JUNL 50
#define TCHAR_FILE 256
#define MY_CREARE_DLG3 0x7777//�������I���{�b�N�X�̐������b�Z�[�W�i���C���E�B���h�E�Ŏ��s�j
#define Aplecat_Name _T("�P�꒠ver.2")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK Dia_r1Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK Dia_r2Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK Dia_iProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

IStream* getFileIStream(const char* lpszPath, HWND hWnd);
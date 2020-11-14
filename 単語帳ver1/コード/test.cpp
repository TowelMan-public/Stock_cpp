#define STRICT

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <gdiplus.h>
#include <olectl.h>
#include "lpfnWndProc.h"

#pragma	comment(lib, "Gdiplus.lib")

// �V���{����`�y�у}�N��
#define Aplecat_Name _T("�P�꒠ver1")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyError error{ 0,0 };
    Gdiplus::GdiplusStartupInput gpSI;
    ULONG_PTR lpToken;
    Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);
    try {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX); // �\���̂̃T�C�Y
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // �N���X�̃X�^�C��
        wcex.lpfnWndProc = WndProc; // �E�C���h�E�v���V�[�W��
        wcex.cbClsExtra = 0; // �⏕������
        wcex.cbWndExtra = 0; // �⏕������
        wcex.hInstance = hInstance;  // �C���X�^���X�n���h��
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // �A�C�R��
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\��
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // �w�i�u���V
        wcex.lpszMenuName = NULL; // ���j���[��
        wcex.lpszClassName = Aplecat_Name; // �N���X��
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // �������A�C�R��

        if (!RegisterClassExA(&wcex)) throw (error);

        HWND hWnd = CreateWindow(
            Aplecat_Name,//�A�v���P�[�V�����̖��O
            Aplecat_Name,//�^�C�g���o�[�Ɍ���镶����
            WS_OVERLAPPEDWINDOW,//��������E�B���h�E�̃^�C�v
            CW_USEDEFAULT, CW_USEDEFAULT,//�ŏ��ɒu���|�W�V���� (x, y)
            1200, 608,//�ŏ��̃T�C�Y (��, ����)
            NULL,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
            NULL,//���j���[�o�[�i���̃T���v���ł͎g�p�����j
            hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
            NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
        );
        //  �E�B���h�E�������ł��Ȃ������ꍇ
        if (!hWnd)throw(error);
        ShowWindow(hWnd, nCmdShow);
        error.hWnd = hWnd;
        // ���C���̃��b�Z�[�W���[�v:
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Gdiplus::GdiplusShutdown(lpToken);
        return (int)msg.wParam;

    }
    catch (MyError Error) {
        if (Error.hWnd) {
            switch (Error.error) {
            case 1:
                MessageBox(Error.hWnd,
                    _T("�_�C�A���{�b�N�X�̍쐬�Ɏ��s���܂���!"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            case 2:
                MessageBox(Error.hWnd,
                    _T("�����_�C�A���{�b�N�X�ŒP����������悤�Ƃ��Ă�Ƃ��ɒP��̓o�^�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            case 3:
                MessageBox(Error.hWnd,
                    _T("�o�^�_�C�A���{�b�N�X�ŒP����������悤�Ƃ��Ă�Ƃ��ɒP��̓o�^�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            case 5:
                MessageBox(Error.hWnd,
                    _T("�P��̌������o�^�����Ă���Œ��ɃG���[���������܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            case 7:
                MessageBox(Error.hWnd,
                    _T("�摜�t�@�C���̃��[�h�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            default:
                MessageBox(Error.hWnd,
                    _T("���̑��̃G���[���������܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            }
        }
        else {
            MessageBox(NULL,
                _T("�E�B���h�E�����Ɏ��s���܂���!"),
                Aplecat_Name,
                MB_ICONERROR);
           Gdiplus::GdiplusShutdown(lpToken);
           PostQuitMessage(0);
           return 1;
        }
    }
    catch (...) {
        MessageBox(NULL,
            _T("�\�����Ȃ��G���[���������܂���!"),
            Aplecat_Name,
            MB_ICONERROR);
        Gdiplus::GdiplusShutdown(lpToken);
        PostQuitMessage(0);
        return 1;
    }
}
#define STRICT

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <gdiplus.h>
#include <olectl.h>
#include "lpfnWndProc.h"

#pragma	comment(lib, "Gdiplus.lib")

// �V���{����`�y�у}�N��
#ifndef Aplecat_Name
#define Aplecat_Name _T("�P�꒠ver.2")
#endif //Aplecat_Name

void NoneErrorHandler(const wchar_t* expression, const wchar_t* function,
    const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
    MyError error{ 12,(HWND)1 };//�G���[�n���h��
    throw error;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyError error{ 0,NULL };
    Gdiplus::GdiplusStartupInput gpSI;
    ULONG_PTR lpToken;
    Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);
    WNDCLASSEX wcex;//�E�B���h�E�N���X
    HWND hWnd = NULL;//���C���E�B���h�E�n���h��
    _invalid_parameter_handler oldHandler = _set_invalid_parameter_handler(NoneErrorHandler);
    try {
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

        hWnd = CreateWindow(
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
        // ���C���̃��b�Z�[�W���[�v:
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)>0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Gdiplus::GdiplusShutdown(lpToken);
        oldHandler = _set_invalid_parameter_handler(oldHandler);
        return (int)msg.wParam;
    }
    catch (MyError Error) {
        if (Error.hWnd) {
            switch (Error.error) {
            case 0:
                MessageBox(NULL,
                    _T("���C���E�B���h�E�̏�������Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 1:
                MessageBox(NULL,
                    _T("�����_�C�A���O�{�b�N�X�̍쐬�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 2:
                MessageBox(NULL,
                    _T("�o�^�_�C�A���{�b�N�X�̍쐬�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 3:
                MessageBox(NULL,
                    _T("�������I���_�C�A���O�{�b�N�X�̍쐬�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 4:
                MessageBox(NULL,
                    _T("�摜�̃��[�h���ɏd��ȃG���[���������܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 6:
                MessageBox(NULL,
                    _T("���C���E�B���h�E�̕`�揈���Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 7:
                MessageBox(NULL,
                    _T("�����_�C�A���{�b�N�X�̏�������Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 8:
                MessageBox(NULL,
                    _T("�o�^�_�C�A���{�b�N�X�̏�������Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 9:
                MessageBox(NULL,
                    _T("�������_�C�A���O�{�b�N�X�̏�������Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 10:
                MessageBox(NULL,
                    _T("�G�f�B�b�g�{�b�N�X����̕�����̎擾�Ɏ��s���܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 11:
                MessageBox(NULL,
                    _T("�������I���_�C�A���{�b�N�X�̓���ŏd��ȃG���[���������܂����I"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            }
            Gdiplus::GdiplusShutdown(lpToken);
            oldHandler = _set_invalid_parameter_handler(oldHandler);
            DestroyWindow(hWnd);
            return -1;
        }
        else {
            MessageBox(NULL,
                _T("�E�B���h�E�̐����Ɏ��s���܂����I"),
                Aplecat_Name,
                MB_ICONERROR);
            Gdiplus::GdiplusShutdown(lpToken);
            oldHandler = _set_invalid_parameter_handler(oldHandler);
            return -1;
        }
    }
    catch (...) {
        MessageBox(NULL,
            _T("�\�����Ȃ��G���[���������܂���!"),
            Aplecat_Name,
            MB_ICONERROR);
        Gdiplus::GdiplusShutdown(lpToken);
        oldHandler = _set_invalid_parameter_handler(oldHandler);
        return 1;
    }
}
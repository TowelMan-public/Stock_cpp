#include "Dentaku.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style             = CS_HREDRAW | CS_VREDRAW;//��{�X�^�C��
	winc.lpfnWndProc       = WndProc;//�v���L�V�֐��ւ̃|�C���^
	winc.cbClsExtra        = NULL;//�N���X�\���̂̒ǉ��̈�i�K�v�Ȃ��j
	winc.cbWndExtra        = NULL;//�E�B���h�E�\���̂̒ǉ��̈�i�K�v�Ȃ��j
	winc.hInstance         = hInstance;//�C���X�^���X�n���h��
	winc.hIcon             = LoadIcon(NULL, IDI_APPLICATION);//�A�C�R��(���W���[��,���O)
	winc.hCursor           = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\���i���W���[��,���O�j
	winc.hbrBackground     = (HBRUSH)GetStockObject(WHITE_BRUSH);//�N���C�A���g�G���A�̔w�i�F(�u���V�̃n���h����n��)
	winc.lpszMenuName      = NULL;//�N���X���j���[
	winc.lpszClassName     = WIN_CLASS_NAME;//�N���X��

	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindow(
		WIN_CLASS_NAME, Win_AP_NAME,//�N���X���A�A�v����
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,//�E�B���h�E�̃X�^�C��
		0, 0, 130, 215,//�ꏊx�A�ꏊy�A���A����
		NULL, NULL,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		hInstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);

	if (hwnd == NULL) return 0;
	ShowWindow(hwnd, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
#include "chat.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;//��{�X�^�C��
	winc.lpfnWndProc = MW::WndProc;//�v���L�V�֐��ւ̃|�C���^
	winc.cbClsExtra = NULL;//�N���X�\���̂̒ǉ��̈�i�K�v�Ȃ��j
	winc.cbWndExtra = NULL;//�E�B���h�E�\���̂̒ǉ��̈�i�K�v�Ȃ��j
	winc.hInstance = hInstance;//�C���X�^���X�n���h��
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);//�A�C�R��(���W���[��,���O)
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\���i���W���[��,���O�j
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�N���C�A���g�G���A�̔w�i�F(�u���V�̃n���h����n��)
	winc.lpszMenuName = _TEXT("MAIN_MENU");;//�N���X���j���[
	winc.lpszClassName = WIN_CLASS_NAME;//�N���X��

	if (!RegisterClass(&winc)) return 0;

	hwnd = CreateWindow(
		WIN_CLASS_NAME, WIN_AP_NAME,//�N���X���A�A�v����
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,//�E�B���h�E�̃X�^�C��
		0, 0, 800, 660,//�ꏊx�A�ꏊy�A���A����
		NULL, NULL,//�e�E�B���h�E�n���h���A���j���[�̃n���h��
		hInstance, NULL//�C���X�^���X�n���h���Avoid*�I�ȓz
	);

	if (hwnd == NULL) return 0;
	ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
	}

	return msg.wParam;
}
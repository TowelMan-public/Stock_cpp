#include "KIBAN.h"
#include "lpfnWndProc.h"

KIBAN::KIBAN(void) {
	flag = 0;
	info = word = junl = file = NULL;
	size_i = size_w = size_j = size_f = 0;
	TF = true;
}

KIBAN::~KIBAN(void) {
	delete[] word;
	delete[] junl;
	delete[] file;
	delete[] info;
}

int KIBAN::Set(TCHAR* baf, size_t size) {
	//�߂�l��0�̎��͉�������������
	switch (flag) {
	case 0://���
		word = new TCHAR[size];
		size_w = size;
		_tcscpy_s(word, size, baf);
		return ++flag;//1
	case 1://���
		junl = new TCHAR[size];
		size_j = size;
		_tcscpy_s(junl, size, baf);
		return ++flag;//2
	case 2://�O��
		info = new TCHAR[size];
		size_i = size;
		_tcscpy_s(info, size, baf);		
		return ++flag;//3
	case 3://�l��
		file = new TCHAR[size];
		size_f = size;
		_tcscpy_s(file, size, baf);
		return ++flag;//4
	default://���蓾�Ȃ�			
		return 0;
	}
}

void KIBAN::Set(void) {
	//���̌����E�o�^���s���Ƃ��ɌĂ΂�鏉���������o�֐�
	delete[] word;
	delete[] junl;
	delete[] file;
	delete[] info;
	flag = 0;
	info = word = junl = file = NULL;
	size_i = size_w = size_j = size_f = 0;
	TF = true;
}

int KIBAN::Ran(HWND hWnd) {
	try {
		int a;
		switch (flag) {
		case 2://����
			TF = false;
			if (Research() == -1) {
				Set();
				return 3;
			}
			TF = true;
			return 1;
		case 3:
		case 4://�o�^
			TF = false;
			a = Register();
			Set();
			if (a == -1)return 2;
			else return 0;
		default:
			return 4;//�������Ȃ�
		}
	}
	catch (...) {
		MyError error = { 5,hWnd };
		throw error;
	}
}

int KIBAN::Research(void) {
	//�߂�l�ɂ���
	//�߂�l=0:���̒P��͑��݂���
	//�߂�l=-1:���̒P��͑��݂��Ȃ�
	//�����Ɖ摜�����邩�Ȃ���
	static DWORD wReadSize;
	CString str(_T("words\\"));
	CString jstr(junl);
	CString wstr(word);
	str += jstr;
	str += _T("\\");
	str += wstr;
	str += _T(".txt");
	HANDLE hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return -1;//�t�@�C���͑��݂��Ȃ�����(���̒P��͑��݂��Ȃ��Ƃ�������)
	info = new TCHAR[GetFileSize(hFile, NULL)];
	if(!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL))return -1;
	size_i = wReadSize;
	CloseHandle(hFile);
	//�摜�p�X
	str = _T("pictures\\");
	str += jstr;
	str += _T("\\");
	str += wstr;
	str += _T(".txt");
	hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;//�摜���Ȃ������疢��`����
	file = new TCHAR[GetFileSize(hFile, NULL)];
	if (!ReadFile(hFile, file, GetFileSize(hFile, NULL), &wReadSize, NULL))return -1;
	size_f = wReadSize;
	CloseHandle(hFile);
	return 0;
}

int KIBAN::Register(void) {
	HANDLE hFile;
	static DWORD wReadSize;
	CString str;
	CString jstr(junl);
	CString wstr(word);
	str = _T("words\\");
	str += jstr;
	if (!CreateDirectory(str.GetBuffer(), NULL))return -1;
	str += _T("\\");
	str += wstr;
	str += _T(".txt");
	hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return -1;//�G���[�E��������`
	WriteFile(hFile, info, size_i, &wReadSize, NULL);
	CloseHandle(hFile);
	if (flag == 3)return 0;
	str = _T("pictures\\");
	str += jstr;
	if (!CreateDirectory(str.GetBuffer(), NULL))return -1;
	str += _T("\\");
	str += wstr;
	str += _T(".txt");	
	hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return -1;//�G���[�E����`
	WriteFile(hFile, file, size_f, &wReadSize, NULL);
	CloseHandle(hFile);
	return 0;
}
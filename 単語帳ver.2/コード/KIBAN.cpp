#include "KIBAN.h"
#include "lpfnWndProc.h"

KIBAN::KIBAN(void) {//�R���X�g���N�^
	word = junl = info = file = NULL;
	size_w = size_j = size_i = size_f = 0;
	TF = false;
}

KIBAN::~KIBAN(void){//�f�X�g���N�^
	delete[] word, junl, info, file;
}

void KIBAN::set(void) {//���Z�b�g
	delete[] word, junl, info, file;
	word = junl = info = file = NULL;
	size_w = size_j = size_i = size_f = 0;
	TF = false;
}

void KIBAN::set(TCHAR* str, size_t sz) {//�P����̃Z�b�g�i�ЂƂÁj
	//str�F�Z�b�g���镶����@sz�Fstr�̃T�C�Y
	if (word == NULL) {//�P�ꖼ
		word = new TCHAR[TCHAR_WORD];
		_tcscpy_s(word, sz, str);
		size_w = sz;
		return;
	}
	else if (junl == NULL) {//���ޖ�
		junl = new TCHAR[TCHAR_JUNL];
		_tcscpy_s(junl, sz, str);
		size_j = sz;
		return;
	}
	else if (info == NULL) {//���e
		info = new TCHAR[TCHAR_INFO];
		_tcscpy_s(info, sz, str);
		size_i = sz;
		return;
	}
	else if (file == NULL) {//�摜�p�X
		file = new TCHAR[TCHAR_FILE];
		_tcscpy_s(file, sz, str);
		size_f = sz;
		return;
	}
}

int KIBAN::run(TCHAR* a , size_t sz) {//�o�^���̎��s(�����w�肵�Ȃ�������o�^�E�w�肵����ύX�̎��s�j
	HANDLE hFile;//�t�@�C���n���h��
	CString str,str2;//�p�X���쐬�p������
	DWORD WiteSize;//�������i�`�������j
	int index;//�g���q�����o�����Ɏg���C���f�b�N�X�L���p�ϐ�
	if (a == NULL) {//�o�^
		str = _T("words\\");//������i�p�X�j�̍쐬�E���e
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\");
		if (!MakeSureDirectoryPathExists(str.GetBuffer()))return -1;//�t�H���_�쐬
		str += _T("info.txt");
		hFile= CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		if (!WriteFile(hFile, info, size_i, &WiteSize, NULL)) {//���e�̓o�^
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		if (file == NULL) {
			TF = true;
			return 0;
		}
		str = _T("words\\");//������i�p�X�j�̍쐬�E�摜�p�X
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic");
		str2 = file;//�R�s�[���̉摜�i���[�U�[�w��j�̃p�X
		index=str2.Find(_T('.'), 0);//�R�s�[���̉摜�i���[�U�[�w��j�̃p�X����g���q���擾
		if (index == -1)return -1;
		str += (str2.Right(str2.GetLength()-index));//������i�p�X�j�Ɋg���q������
		if (!CopyFileA(str2, str, TRUE))return -1;//�摜�̃R�s�[�i�摜�̓o�^�j
		str = _T("words\\");//������i�p�X�j�̍쐬�E�摜�̊g���q�̃p�X
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic.txt");
		hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		if (!WriteFile(hFile, str2.Right(str2.GetLength() - index), _tcslen(str2.Right(str2.GetLength() - index)) * sizeof(TCHAR), &WiteSize, NULL)) {//�g���q�̓o�^
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		TF = true;
		return 0;
	}
	else {//�ύX
		str = _T("words\\");//������i�p�X�j�̍쐬�E���e
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\info.txt");
		if(!DeleteFileA(str.GetBuffer()))return -1;//���̒P��̓��e�̍폜
		hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		_tcscpy_s(info, size_i, a);
		if (!WriteFile(hFile, info, size_i, &WiteSize, NULL)) {//�ύX��̓��e�̓o�^
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		TF = true;
		return 0;
	}
}

void KIBAN::operator=(BASIC_KIBAN date) {//�P����̃Z�b�g�i�\���̂��g���Ĉꊇ�ɃZ�b�g
	set();
	if (date.word == _T("\n\n\n\n"))return;//�G���[��
	//�e���̃T�C�Y�̎擾
	size_w= TCHAR_WORD * sizeof(TCHAR);
	size_j = TCHAR_JUNL * sizeof(TCHAR);
	size_i = TCHAR_INFO * sizeof(TCHAR);
	if (date.file != _T("\n\n\n\n")) 
		size_f = TCHAR_FILE * sizeof(TCHAR);
	//��
	delete[] word, junl, info, file;
	word = junl = info = file = NULL;
	//�̈�̊m��
	word = new TCHAR[TCHAR_WORD];
	junl = new TCHAR[TCHAR_JUNL];
	info = new TCHAR[TCHAR_INFO];
	if (date.file != _T("\n\n\n\n")) 
		file = new TCHAR[TCHAR_FILE];
	//������̃R�s�[�i�e���̎擾�j
 	_tcscpy_s(word, size_w, date.word.GetBuffer());
	_tcscpy_s(junl, size_j, date.junl.GetBuffer());
	_tcscpy_s(info, size_i, date.info.GetBuffer());
	if (date.file  != _T("\n\n\n\n"))
	_tcscpy_s(file, size_f, date.file.GetBuffer());
	//�t���O�̃Z�b�g
	TF = true;
}

KIBAN_KENSAKU::KIBAN_KENSAKU(void) {//�R���X�g���N�^
	Cjunl = NULL;
	Count = -100;//GetStats�֐��̎��s�����O��Count�̒l
}

KIBAN_KENSAKU::~KIBAN_KENSAKU(void) {//�f�X�X�g���N�^
	delete[] Cjunl;
}

int KIBAN_KENSAKU::run(void) {//�����̎��s�i�I�[�o�[���C�h�j
	DWORD wReadSize;//������
	CString str;//�p�X���쐬�p������
	TCHAR* baf;//�g���q�ۑ��p�����|�C���^�i�g���Ƃ��ɂ͔z����m�ۂ���j
	HANDLE hFile;//�t�@�C���n���h��
	std::fstream ffs;//�摜�̊g���q���擾����Ƃ��Ɏg��
	std::string ssstr;//�摜�̊g���q���擾����Ƃ��Ƀp�X�쐬�Ɏg��
	if (junl != NULL) {//���S��v�����̎��E���ޖ����w�肳��Ă���
		str = _T("words\\");//������i�p�X�j�̍쐬�E���e
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\info.txt");
		hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)return -1;
		info = new TCHAR[GetFileSize(hFile, NULL)/sizeof(TCHAR)];
		if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//���e�̎擾
			CloseHandle(hFile);
			return -1;
		}
		size_i = wReadSize;
		CloseHandle(hFile);
		str = _T("words\\");//������i�p�X�j�̍쐬�E�摜�̕����񂪓����Ă���t�@�C��
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic.txt");
		ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//�g���q�̓����Ă���t�@�C���̃I�[�v��
		ssstr = "";
		ffs >> ssstr;//�g���q�擾
		ffs.close();//�t�@�C�������
		if (ssstr == _T(""))return 0;//�摜��
		str = _T("words\\");//�摜�p�X�̍쐬
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic");
		str += _T(ssstr.c_str());
		size_f = str.GetLength() * sizeof(TCHAR);
		file = new TCHAR[TCHAR_FILE];
		_tcscpy_s(file, TCHAR_FILE, str.GetBuffer());//�摜�p�X�̎擾(�쐬������̃R�s�[)
		return 0;
	}
	else {//���ޖ������w��
		int gggg;
		switch (GetStats()) {
		case -1://�G���[
			return -1;
		case 1://������₪��̂݁����ʓI�Ɋ��S��v����
			junl = new TCHAR[Cjunl[0].GetLength()];
			junl = Cjunl[0].GetBuffer();//���ޖ��̎擾
			size_j = Cjunl[0].GetLength() * sizeof(TCHAR);//���ޖ��̃T�C�Y�擾
			str = _T("words\\");//���e�̓����Ă���p�X�̍쐬
			str += word;
			str += _T("\\");
			str += junl;
			str += _T("\\info.txt");
			hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)return -1;
			info = new TCHAR[GetFileSize(hFile, NULL) / sizeof(TCHAR)];
			if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//���e�̎擾
				CloseHandle(hFile);
				return -1;
			}
			size_i = wReadSize;
			CloseHandle(hFile);
			str = _T("words\\");//�摜�̊g���q�̓����Ă���p�X�̍쐬
			str += word;
			str += _T("\\");
			str += Cjunl[0];
			str += _T("\\pic.txt");
			ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//�g���q�̓����Ă���t�@�C���̃I�[�v��
			ssstr = "";
			ffs >> ssstr;
			ffs.close();//�t�@�C�������
			if (ssstr == _T(""))return 0;//�摜��
			str = _T("words\\");//�摜�p�X�̍쐬
			str += word;
			str += _T("\\");
			str += junl;
			str += _T("\\pic");
			str += _T(ssstr.c_str());
			size_f = str.GetLength() * sizeof(TCHAR);
			file = new TCHAR[TCHAR_FILE];
			_tcscpy_s(file, TCHAR_FILE, str.GetBuffer()); // �摜�̃p�X�̎擾�i�쐬������̃R�s�[�j
			return 0;
		default:
			return 0;
		}
	}
	return -1;
}

BASIC_KIBAN KIBAN_KENSAKU::operator[](int a) {//�w�肳�ꂽ����Ԃ��i1�`Count�̊ԂŎw��)
	BASIC_KIBAN date,E;//date�͖߂�l�p E�̓G���[�̎��̖߂�l�p
	CString str;//�p�X���쐬�p
	std::string ssstr;
	std::fstream ffs;
	DWORD wReadSize;//������
	HANDLE hFile;
	E.word = _T("\n\n\n\n");//�G���[���̃Z�b�g
	if (Count == 1) {//��₪������Ȃ����̏���
		date.word = word;
		date.junl = junl;
		date.info = info;
		date.file = file == NULL ? _T("\n\n\n\n") : file;
		return date;
	}
	date.word = word;//�P�ꖼ�̃Z�b�g
	date.junl = Cjunl[a - 1];//���ޖ��̃Z�b�g
	str = _T("words\\");//���e�̃p�X�쐬
	str += word;
	str += _T("\\");
	str += date.junl.GetBuffer();
	str += _T("\\info.txt");
	hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return E;
	info = new TCHAR[GetFileSize(hFile, NULL) / sizeof(TCHAR)];//����͈ꎞ�o�b�t�@�Ƃ���
	if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//���e�̎擾
		CloseHandle(hFile);
		return E;
	}
	CloseHandle(hFile);
	date.info = info;
	delete[] info;//�K�v�Ȃ��Ȃ���
	info = NULL;
	date.file = _T("\n\n\n\n");//�摜���w��̎��p
	str = _T("words\\");//�摜�̊g���q�̓����Ă�p�X�쐬
	str += word;
	str += _T("\\");
	str += date.junl.GetBuffer();
	str += _T("\\pic.txt");
	str = _T("words\\");//�摜�̊g���q�̓����Ă���p�X�̍쐬
	str += word;
	str += _T("\\");
	str += Cjunl[0];
	str += _T("\\pic.txt");
	ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//�g���q�̓����Ă���t�@�C���̃I�[�v��
	ssstr = "";
	ffs >> ssstr;
	ffs.close();//�t�@�C�������
	str = _T("words\\");//�摜�p�X�̍쐬
	str += word;
	str += _T("\\");
	str += Cjunl[a-1];
	str += _T("\\pic");
	str += _T(ssstr.c_str());
	date.file = str;//�摜�̃p�X�Z�b�g�i�쐬��������R�s�[)
	delete[] file;//����Ȃ��Ȃ���
	file = NULL;
	return date;
}

int KIBAN_KENSAKU::GetStats(void){//��������␔�̎擾��
	if (Count != -100)return Count;//�������łɂ��̊֐������s����Ă���
	int cc = 0;//�w�肵���f�B���N�g���ȉ��̑S�Ẵt�@�C���̌���
	HANDLE hFind;//�t�@�C�������p�n���h��
	CString str;//�Ώۃp�X�쐬�p
	WIN32_FIND_DATA FindDate;//�t�@�C�������p�\����
	str = _T("words\\");
	str += word;
	str += _T("\\*.*");
	hFind=FindFirstFile(str.GetBuffer(), &FindDate);//�����J�n�I
	if (hFind == INVALID_HANDLE_VALUE)return -1;//������Ȃ�����
	Count = 1;
	do {
		++cc;
		if (FindDate.cFileName != _T("") && FindDate.cFileName != _T(".."))++Count;//�f�B���N�g���̐�
	}
	while (FindNextFileA(hFind, &FindDate));//�������̐����J�E���g
	if (GetLastError() != ERROR_NO_MORE_FILES) {//�G���[�̎�
		MyError e = { 10,(HWND)1 };
		FindClose(hFind);
		throw e;
	}
	FindClose(hFind);
	hFind = FindFirstFile(str.GetBuffer(), &FindDate);
	if (hFind == (HANDLE)-1)return -1;
	Cjunl = new CString[Count];//����߂ɐ錾
	for (int i = 0,j=0; j < cc; i++) {//���ޖ��̎擾
		if (!FindNextFile(hFind, &FindDate)) break;
		if (FindDate.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			Cjunl[j] = (FindDate.cFileName);
			if (Cjunl[j] != _T("")&& Cjunl[j] != _T(".."))
				Count = ++j;
		}
	}
	if (GetLastError() != ERROR_NO_MORE_FILES) {//�G���[�̎�
		MyError e = { 10,(HWND)1 };
		FindClose(hFind);
		throw e;
	}
	FindClose(hFind);
	return Count;
}
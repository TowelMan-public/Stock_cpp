#include "KIBAN.h"
#include "lpfnWndProc.h"

KIBAN::KIBAN(void) {//コンストラクタ
	word = junl = info = file = NULL;
	size_w = size_j = size_i = size_f = 0;
	TF = false;
}

KIBAN::~KIBAN(void){//デストラクタ
	delete[] word, junl, info, file;
}

void KIBAN::set(void) {//リセット
	delete[] word, junl, info, file;
	word = junl = info = file = NULL;
	size_w = size_j = size_i = size_f = 0;
	TF = false;
}

void KIBAN::set(TCHAR* str, size_t sz) {//単語情報のセット（ひとつづ）
	//str：セットする文字列　sz：strのサイズ
	if (word == NULL) {//単語名
		word = new TCHAR[TCHAR_WORD];
		_tcscpy_s(word, sz, str);
		size_w = sz;
		return;
	}
	else if (junl == NULL) {//分類名
		junl = new TCHAR[TCHAR_JUNL];
		_tcscpy_s(junl, sz, str);
		size_j = sz;
		return;
	}
	else if (info == NULL) {//内容
		info = new TCHAR[TCHAR_INFO];
		_tcscpy_s(info, sz, str);
		size_i = sz;
		return;
	}
	else if (file == NULL) {//画像パス
		file = new TCHAR[TCHAR_FILE];
		_tcscpy_s(file, sz, str);
		size_f = sz;
		return;
	}
}

int KIBAN::run(TCHAR* a , size_t sz) {//登録等の実行(何も指定しなかったら登録・指定したら変更の実行）
	HANDLE hFile;//ファイルハンドル
	CString str,str2;//パス等作成用文字列
	DWORD WiteSize;//文字数（形式だけ）
	int index;//拡張子を取り出す時に使うインデックス記憶用変数
	if (a == NULL) {//登録
		str = _T("words\\");//文字列（パス）の作成・内容
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\");
		if (!MakeSureDirectoryPathExists(str.GetBuffer()))return -1;//フォルダ作成
		str += _T("info.txt");
		hFile= CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		if (!WriteFile(hFile, info, size_i, &WiteSize, NULL)) {//内容の登録
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		if (file == NULL) {
			TF = true;
			return 0;
		}
		str = _T("words\\");//文字列（パス）の作成・画像パス
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic");
		str2 = file;//コピー元の画像（ユーザー指定）のパス
		index=str2.Find(_T('.'), 0);//コピー元の画像（ユーザー指定）のパスから拡張子を取得
		if (index == -1)return -1;
		str += (str2.Right(str2.GetLength()-index));//文字列（パス）に拡張子をつける
		if (!CopyFileA(str2, str, TRUE))return -1;//画像のコピー（画像の登録）
		str = _T("words\\");//文字列（パス）の作成・画像の拡張子のパス
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic.txt");
		hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		if (!WriteFile(hFile, str2.Right(str2.GetLength() - index), _tcslen(str2.Right(str2.GetLength() - index)) * sizeof(TCHAR), &WiteSize, NULL)) {//拡張子の登録
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		TF = true;
		return 0;
	}
	else {//変更
		str = _T("words\\");//文字列（パス）の作成・内容
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\info.txt");
		if(!DeleteFileA(str.GetBuffer()))return -1;//元の単語の内容の削除
		hFile = CreateFile(str.GetBuffer(), GENERIC_WRITE, 0, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!hFile) return -1;
		_tcscpy_s(info, size_i, a);
		if (!WriteFile(hFile, info, size_i, &WiteSize, NULL)) {//変更後の内容の登録
			CloseHandle(hFile);
			return -1;
		}
		CloseHandle(hFile);
		TF = true;
		return 0;
	}
}

void KIBAN::operator=(BASIC_KIBAN date) {//単語情報のセット（構造体を使って一括にセット
	set();
	if (date.word == _T("\n\n\n\n"))return;//エラー時
	//各情報のサイズの取得
	size_w= TCHAR_WORD * sizeof(TCHAR);
	size_j = TCHAR_JUNL * sizeof(TCHAR);
	size_i = TCHAR_INFO * sizeof(TCHAR);
	if (date.file != _T("\n\n\n\n")) 
		size_f = TCHAR_FILE * sizeof(TCHAR);
	//壊す
	delete[] word, junl, info, file;
	word = junl = info = file = NULL;
	//領域の確保
	word = new TCHAR[TCHAR_WORD];
	junl = new TCHAR[TCHAR_JUNL];
	info = new TCHAR[TCHAR_INFO];
	if (date.file != _T("\n\n\n\n")) 
		file = new TCHAR[TCHAR_FILE];
	//文字列のコピー（各情報の取得）
 	_tcscpy_s(word, size_w, date.word.GetBuffer());
	_tcscpy_s(junl, size_j, date.junl.GetBuffer());
	_tcscpy_s(info, size_i, date.info.GetBuffer());
	if (date.file  != _T("\n\n\n\n"))
	_tcscpy_s(file, size_f, date.file.GetBuffer());
	//フラグのセット
	TF = true;
}

KIBAN_KENSAKU::KIBAN_KENSAKU(void) {//コンストラクタ
	Cjunl = NULL;
	Count = -100;//GetStats関数の実行される前のCountの値
}

KIBAN_KENSAKU::~KIBAN_KENSAKU(void) {//デスストラクタ
	delete[] Cjunl;
}

int KIBAN_KENSAKU::run(void) {//検索の実行（オーバーライド）
	DWORD wReadSize;//文字数
	CString str;//パス等作成用文字列
	TCHAR* baf;//拡張子保存用文字ポインタ（使うときには配列を確保する）
	HANDLE hFile;//ファイルハンドル
	std::fstream ffs;//画像の拡張子を取得するときに使う
	std::string ssstr;//画像の拡張子を取得するときにパス作成に使う
	if (junl != NULL) {//完全一致検索の時・分類名が指定されている
		str = _T("words\\");//文字列（パス）の作成・内容
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\info.txt");
		hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)return -1;
		info = new TCHAR[GetFileSize(hFile, NULL)/sizeof(TCHAR)];
		if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//内容の取得
			CloseHandle(hFile);
			return -1;
		}
		size_i = wReadSize;
		CloseHandle(hFile);
		str = _T("words\\");//文字列（パス）の作成・画像の文字列が入っているファイル
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic.txt");
		ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//拡張子の入っているファイルのオープン
		ssstr = "";
		ffs >> ssstr;//拡張子取得
		ffs.close();//ファイルを閉じる
		if (ssstr == _T(""))return 0;//画像無
		str = _T("words\\");//画像パスの作成
		str += word;
		str += _T("\\");
		str += junl;
		str += _T("\\pic");
		str += _T(ssstr.c_str());
		size_f = str.GetLength() * sizeof(TCHAR);
		file = new TCHAR[TCHAR_FILE];
		_tcscpy_s(file, TCHAR_FILE, str.GetBuffer());//画像パスの取得(作成したやつのコピー)
		return 0;
	}
	else {//分類名が未指定
		int gggg;
		switch (GetStats()) {
		case -1://エラー
			return -1;
		case 1://検索候補が一つのみ→結果的に完全一致検索
			junl = new TCHAR[Cjunl[0].GetLength()];
			junl = Cjunl[0].GetBuffer();//分類名の取得
			size_j = Cjunl[0].GetLength() * sizeof(TCHAR);//分類名のサイズ取得
			str = _T("words\\");//内容の入っているパスの作成
			str += word;
			str += _T("\\");
			str += junl;
			str += _T("\\info.txt");
			hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)return -1;
			info = new TCHAR[GetFileSize(hFile, NULL) / sizeof(TCHAR)];
			if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//内容の取得
				CloseHandle(hFile);
				return -1;
			}
			size_i = wReadSize;
			CloseHandle(hFile);
			str = _T("words\\");//画像の拡張子の入っているパスの作成
			str += word;
			str += _T("\\");
			str += Cjunl[0];
			str += _T("\\pic.txt");
			ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//拡張子の入っているファイルのオープン
			ssstr = "";
			ffs >> ssstr;
			ffs.close();//ファイルを閉じる
			if (ssstr == _T(""))return 0;//画像無
			str = _T("words\\");//画像パスの作成
			str += word;
			str += _T("\\");
			str += junl;
			str += _T("\\pic");
			str += _T(ssstr.c_str());
			size_f = str.GetLength() * sizeof(TCHAR);
			file = new TCHAR[TCHAR_FILE];
			_tcscpy_s(file, TCHAR_FILE, str.GetBuffer()); // 画像のパスの取得（作成したやつのコピー）
			return 0;
		default:
			return 0;
		}
	}
	return -1;
}

BASIC_KIBAN KIBAN_KENSAKU::operator[](int a) {//指定された情報を返す（1～Countの間で指定)
	BASIC_KIBAN date,E;//dateは戻り値用 Eはエラーの時の戻り値用
	CString str;//パス等作成用
	std::string ssstr;
	std::fstream ffs;
	DWORD wReadSize;//文字数
	HANDLE hFile;
	E.word = _T("\n\n\n\n");//エラー時のセット
	if (Count == 1) {//候補が一つしかない時の処理
		date.word = word;
		date.junl = junl;
		date.info = info;
		date.file = file == NULL ? _T("\n\n\n\n") : file;
		return date;
	}
	date.word = word;//単語名のセット
	date.junl = Cjunl[a - 1];//分類名のセット
	str = _T("words\\");//内容のパス作成
	str += word;
	str += _T("\\");
	str += date.junl.GetBuffer();
	str += _T("\\info.txt");
	hFile = CreateFile(str.GetBuffer(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)return E;
	info = new TCHAR[GetFileSize(hFile, NULL) / sizeof(TCHAR)];//今回は一時バッファとして
	if (!ReadFile(hFile, info, GetFileSize(hFile, NULL), &wReadSize, NULL)) {//内容の取得
		CloseHandle(hFile);
		return E;
	}
	CloseHandle(hFile);
	date.info = info;
	delete[] info;//必要なくなった
	info = NULL;
	date.file = _T("\n\n\n\n");//画像未指定の時用
	str = _T("words\\");//画像の拡張子の入ってるパス作成
	str += word;
	str += _T("\\");
	str += date.junl.GetBuffer();
	str += _T("\\pic.txt");
	str = _T("words\\");//画像の拡張子の入っているパスの作成
	str += word;
	str += _T("\\");
	str += Cjunl[0];
	str += _T("\\pic.txt");
	ffs.open(str.GetBuffer(), std::ios::in | std::ios::out);//拡張子の入っているファイルのオープン
	ssstr = "";
	ffs >> ssstr;
	ffs.close();//ファイルを閉じる
	str = _T("words\\");//画像パスの作成
	str += word;
	str += _T("\\");
	str += Cjunl[a-1];
	str += _T("\\pic");
	str += _T(ssstr.c_str());
	date.file = str;//画像のパスセット（作成したやつをコピー)
	delete[] file;//いらなくなった
	file = NULL;
	return date;
}

int KIBAN_KENSAKU::GetStats(void){//総検索候補数の取得等
	if (Count != -100)return Count;//もうすでにこの関数が実行されている
	int cc = 0;//指定したディレクトリ以下の全てのファイルの件数
	HANDLE hFind;//ファイル検索用ハンドル
	CString str;//対象パス作成用
	WIN32_FIND_DATA FindDate;//ファイル検索用構造体
	str = _T("words\\");
	str += word;
	str += _T("\\*.*");
	hFind=FindFirstFile(str.GetBuffer(), &FindDate);//検索開始！
	if (hFind == INVALID_HANDLE_VALUE)return -1;//見つからなかった
	Count = 1;
	do {
		++cc;
		if (FindDate.cFileName != _T("") && FindDate.cFileName != _T(".."))++Count;//ディレクトリの数
	}
	while (FindNextFileA(hFind, &FindDate));//検索候補の数をカウント
	if (GetLastError() != ERROR_NO_MORE_FILES) {//エラーの時
		MyError e = { 10,(HWND)1 };
		FindClose(hFind);
		throw e;
	}
	FindClose(hFind);
	hFind = FindFirstFile(str.GetBuffer(), &FindDate);
	if (hFind == (HANDLE)-1)return -1;
	Cjunl = new CString[Count];//一つ多めに宣言
	for (int i = 0,j=0; j < cc; i++) {//分類名の取得
		if (!FindNextFile(hFind, &FindDate)) break;
		if (FindDate.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			Cjunl[j] = (FindDate.cFileName);
			if (Cjunl[j] != _T("")&& Cjunl[j] != _T(".."))
				Count = ++j;
		}
	}
	if (GetLastError() != ERROR_NO_MORE_FILES) {//エラーの時
		MyError e = { 10,(HWND)1 };
		FindClose(hFind);
		throw e;
	}
	FindClose(hFind);
	return Count;
}
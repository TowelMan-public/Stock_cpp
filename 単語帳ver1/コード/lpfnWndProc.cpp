#include "lpfnWndProc.h"

//必要な定義物(内部リンゲージ)
static HWND MasterHWND;
static HWND button_r;
static HWND button_i;
static HWND Dialog_r;
static HWND Dialog_i;
static HWND Dialog_r_ed1;
static HWND Dialog_r_ed2;
static HWND Dialog_i_ed1;
static HWND Dialog_i_ed2;
static HWND Dialog_i_ed3;
static HWND Dialog_i_ed4;
static HWND EditBox_info;
static bool flag_i = false;
static bool flag_r = false;
static KIBAN kiban;
static LOGFONT FONT{ 40,0,0,0, FW_BOLD,TRUE,TRUE,FALSE,
    SHIFTJIS_CHARSET ,OUT_DEFAULT_PRECIS ,CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY ,0,'\0' };

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MyError error = { 1,hWnd };
    PAINTSTRUCT ps;
    HDC hdc;
    HFONT Font_w;
    IStream* is;

    Gdiplus::Bitmap* g_bmp;
    Gdiplus::Graphics* g;

    try {
        switch (message)//繰り返しやるメイン動作
        {
        case WM_CREATE:
            MasterHWND = hWnd;
            EditBox_info = CreateWindow(//説明文のエディットボックス(メインウィンドウ上に作成)
                TEXT("EDIT"), NULL,
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                ES_AUTOHSCROLL | ES_AUTOVSCROLL |
                ES_LEFT | ES_MULTILINE,
                0, 300, 1200, 304, hWnd, (HMENU)ID_E_1,
                ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );
            button_r = CreateWindow(//検索ボタン（メインウィンドウ上に作成）
                _T("BUTTON"),//アプリケーションの名前
                _T("検索"),//タイトルバーに現れる文字列
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//生成するウィンドウのタイプ
                0, 0,//最初に置くポジション (x, y)
                50, 50,//最初のサイズ (幅, 高さ)
                hWnd,//このウィンドウの親ウィンドウのハンドル
                (HMENU)(ID_B_1),//メニューバー（このサンプルでは使用せず）
                ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain関数の最初のパラメータ
                NULL//WM_CREATE情報（このアプリケーションでは使用せず）
            );
            button_i = CreateWindow(//登録ボタン（メインウィンドウ上に作成）
                _T("BUTTON"),//アプリケーションの名前
                _T("登録"),//タイトルバーに現れる文字列
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//生成するウィンドウのタイプ
                50, 0,//最初に置くポジション (x, y)
                50, 50,//最初のサイズ (幅, 高さ)
                hWnd,//このウィンドウの親ウィンドウのハンドル
                (HMENU)(ID_B_2),//メニューバー（このサンプルでは使用せず）
                ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain関数の最初のパラメータ
                NULL//WM_CREATE情報（このアプリケーションでは使用せず）
            );
            if (!EditBox_info || !button_r || !button_i) {//作成できなかった場合はウィンドウの作成ができなかった扱い
                error.hWnd = NULL;
                throw(error);
            }
            break;
        case WM_PAINT://描画        
            hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 0, 50, _T("単語"), _tcslen(_T("単語")));
            TextOut(hdc, 0, 150, _T("分類"), _tcslen(_T("分類")));
            if (kiban.TF) {//書いていいかどうか
                SetWindowText(EditBox_info, kiban.info);//内容をエディットボックスに描画 
                Font_w = CreateFontIndirect(&FONT);
                if (Font_w == NULL)throw error;
                SelectObject(hdc, Font_w);
                TextOut(hdc, 50, 60, kiban.word, lstrlen(kiban.word));//単語
                TextOut(hdc, 50, 160, kiban.junl, lstrlen(kiban.junl));//分類
                SelectObject(hdc, GetStockObject(SYSTEM_FONT));
                DeleteObject(Font_w);
                if (kiban.file != NULL) {
                    is = getFileIStream(kiban.file, hWnd);//メモリ上の画像ファイルのハンドル取得
                    if (is == NULL) {//戻り値がNULLの場合、int例外の0へ
                        throw 0;
                        EndPaint(hWnd, &ps);
                        throw 0;
                    }
                    g_bmp = new Gdiplus::Bitmap(is);//GDI+の画像のオブジェクト
                    is->Release();//メモリ開放
                    g = new Gdiplus::Graphics(hdc);//GDIの描画オブジェクト
                    g->DrawImage(g_bmp, 500, 0);//描画
                    delete g;
                }
            }
            EndPaint(hWnd, &ps);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case ID_B_1:
                if (!flag_r && !flag_i) {//何にもダイアルボックスが開かれてないか
                    flag_r = true;
                    Dialog_r = CreateDialog(//検索ダイアルボックス作成
                        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
                        MAKEINTRESOURCE(IDD_DIALOG1),
                        hWnd,
                        (DLGPROC)Dia_rProc
                    );
                    if (!Dialog_r)throw(error);
                    ShowWindow(Dialog_r, SW_SHOW);
                    Dialog_r_ed1 = GetDlgItem(Dialog_r, IDC_EDIT1);//エディットボックスのハンドル取得
                    if (!Dialog_r_ed1)throw(error);
                    Dialog_r_ed2 = GetDlgItem(Dialog_r, IDC_EDIT2);
                    if (!Dialog_r_ed2)throw(error);
                }
                break;
            case ID_B_2:
                if (!flag_r && !flag_i) {//何にもダイアルボックスが開かれてないか
                    flag_i = true;
                    Dialog_i = CreateDialog(//登録ダイアルボックス作成
                        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
                        MAKEINTRESOURCE(IDD_DIALOG2),
                        hWnd,
                        (DLGPROC)Dia_iProc
                    );
                    if (!Dialog_i)throw(error);
                    ShowWindow(Dialog_i, SW_SHOW);
                    Dialog_i_ed1 = GetDlgItem(Dialog_i, IDC_EDIT1);
                    if (!Dialog_i_ed1)throw(error);
                    Dialog_i_ed2 = GetDlgItem(Dialog_i, IDC_EDIT2);
                    if (!Dialog_i_ed2)throw(error);
                    Dialog_i_ed3 = GetDlgItem(Dialog_i, IDC_EDIT3);
                    if (!Dialog_i_ed3)throw(error);
                    Dialog_i_ed4 = GetDlgItem(Dialog_i, IDC_EDIT4);
                    if (!Dialog_i_ed4)throw(error);
                }
                break;
            }
            break;
        case WM_DESTROY://×
            DestroyWindow(hWnd);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
        }
        return 0;
    }
    catch (int E) {
        switch (E) {
        case 0://画像が開けなくてパスが疑わしい時
            MessageBox(hWnd,// オーナーウィンドウのハンドル
                _T("あなたがこの単語に指定した画像パスが無効です。\nこのソフトの場所を移動したり登録した画像を移動したりするとこういうことが起る場合があります。"),// メッセージボックス内のテキスト
                _T("パスが無効です。"),// メッセージボックスのタイトル
                NULL// メッセージボックスのスタイル
            );
            return 0;
        }
    }
    return 0;
}

BOOL CALLBACK Dia_rProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    
    MyError error = { 2,hWnd };
    TCHAR* str;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        //ダイアログボックスが生成されたとき
        kiban.Set();
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            kiban.Set();
            str = new TCHAR[TCHAR_WORD];
            if (GetWindowText(Dialog_r_ed1, str, sizeof(TCHAR) * TCHAR_WORD) == 0)goto THIS_INFO_FOR_UESER;
            if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_WORD))
                throw(error);
            delete[] str;
            str = new TCHAR[TCHAR_JUNL];
            if (GetWindowText(Dialog_r_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) == 0)goto THIS_INFO_FOR_UESER;
            if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_JUNL))throw(error);
            delete[] str;
            switch (kiban.Ran(hWnd)) {
            case 3://単語が存在しない
                MessageBox(hWnd,// オーナーウィンドウのハンドル
                    _T("その単語は存在しませんでした"),// メッセージボックス内のテキスト
                    _T("失敗"),// メッセージボックスのタイトル
                    NULL// メッセージボックスのスタイル
                );
                return TRUE;
            }
            DestroyWindow(hWnd);
            flag_r = false;
            InvalidateRect(MasterHWND, NULL, TRUE);
            return TRUE;
        case IDCANCEL:
            //モードレスダイアログボックスを破棄       
            DestroyWindow(hWnd);
            flag_r = false;
            break;
            return TRUE;
        }
    default:
        return FALSE;
    }
    return TRUE;
THIS_INFO_FOR_UESER:
    MessageBox(hWnd,// オーナーウィンドウのハンドル
        _T("まだ入力されていないところがあります。\nその項目の内容を入力してからご検索下さい"),// メッセージボックス内のテキスト
        _T("ご案内"),// メッセージボックスのタイトル
        NULL// メッセージボックスのスタイル
    );
    return TRUE;
}

BOOL CALLBACK Dia_iProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    MyError error = { 3,hWnd };
    TCHAR* str;
    switch (iMsg)
    {
    case WM_INITDIALOG:
        //ダイアログボックスが生成されたとき
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            kiban.Set();
            str = new TCHAR[TCHAR_JUNL];
            if (GetWindowText(Dialog_i_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) == 0)goto THIS_INFO_FOR_UESER;
            if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_JUNL))throw(error);
            delete[] str;
            str = new TCHAR[TCHAR_WORD];
            if (GetWindowText(Dialog_i_ed1, str, sizeof(TCHAR) * TCHAR_WORD) == 0)goto THIS_INFO_FOR_UESER;
            if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_WORD))throw(error);
            delete[] str;
            str = new TCHAR[TCHAR_INFO];
            if (GetWindowText(Dialog_i_ed4, str, sizeof(TCHAR) * TCHAR_INFO) == 0)goto THIS_INFO_FOR_UESER;
            if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_INFO)) throw(error);
            delete[] str;
            str = new TCHAR[TCHAR_FILE];
            if (GetWindowText(Dialog_i_ed3, str, sizeof(TCHAR) * TCHAR_FILE) != 0)
                if (!kiban.Set(str, sizeof(TCHAR) * TCHAR_FILE))throw(error);
            delete[] str;
            switch (kiban.Ran(hWnd)) {
            case 0://登録に成功した
                MessageBox(hWnd,// オーナーウィンドウのハンドル
                    _T("単語の登録に成功しました"),// メッセージボックス内のテキスト
                    _T("成功しました！"),// メッセージボックスのタイトル
                    NULL// メッセージボックスのスタイル
                );
                break;
            case 1://単語が存在した
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 2://登録に失敗した
                MessageBox(hWnd,// オーナーウィンドウのハンドル
                    _T("単語の登録に失敗しました"),// メッセージボックス内のテキスト
                    _T("失敗！"),// メッセージボックスのタイトル
                    NULL// メッセージボックスのスタイル
                );
                break;
            }
            DestroyWindow(hWnd);
            flag_i = false;
            break;
            return TRUE;
        case IDCANCEL:
            //モードレスダイアログボックスを破棄       
            DestroyWindow(hWnd);
            flag_i = false;
            break;
            return TRUE;
        }
        return TRUE;
    default:
        return FALSE;
    }
THIS_INFO_FOR_UESER:
    MessageBox(hWnd,// オーナーウィンドウのハンドル
        _T("まだ画像パス以外で入力されていないところがあります。\nその項目の内容を入力してからご登録下さい"),// メッセージボックス内のテキスト
        _T("ご案内"),// メッセージボックスのタイトル
        NULL// メッセージボックスのスタイル
    );
    return TRUE;
}
IStream* getFileIStream(const char* lpszPath,HWND hWnd) {

    HANDLE hFile;
    HGLOBAL hBuf;
    LPVOID lpBuf;
    IStream* isFile;
    DWORD dwFileSize, dwLoadSize;

    MyError error = { 7,hWnd };

    /* 画像ファイルオープン */
    hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    /* ファイルサイズ取得 */
    dwFileSize = GetFileSize(hFile, NULL);

    /* 画像ファイルデータ格納用メモリブロック確保 */
    hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    if (hBuf == NULL)throw error;
    /* メモリブロックをロックしアドレスを取得 */
    lpBuf = GlobalLock(hBuf);
    /* 画像ファイルのデータをメモリブロックに読み込む */
    if (!ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL))return NULL;

    CloseHandle(hFile);

    /* メモリブロックのロック解除 */
    GlobalUnlock(hBuf);

    /* メモリブロックからIStreamを作成 */
    CreateStreamOnHGlobal(hBuf, TRUE, &isFile);

    /* IStreamを返す */
    return isFile;
}
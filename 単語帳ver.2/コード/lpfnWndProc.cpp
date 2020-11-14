#include "lpfnWndProc.h"

//必要な定義物(内部リンゲージ)
static bool flag_i = false;//登録ダイログボックスが開いているかのフラグ
static bool flag_r = false;//検索候補選択ダイアログボックスが開いているかのフラグ
static KIBAN kiban;//単語情報の取り扱い及び登録処理
static KIBAN_KENSAKU *Kensaku;//単語情報の受け渡し及び検索処理

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MyError error = { 0,hWnd };//エラーハンドル
    PAINTSTRUCT ps;//描画処理で使う
    HDC hdc;//描画処理で使う
    HFONT Font_w;//フォントハンドル
    bool DeFlag = false;//WM_DESTROY:の実行を一度のみにするためのフラグ
    static IStream* is=NULL;//メモリ上にロードされているもののハンドル
    static HWND EditBox_info;//メインウィンドウのエディットボックスのハンドル
    static TCHAR* str;//文字列取り扱い用

    static HWND Dialog_r1;//検索ダイアログボックスのハンドル
    static HWND Dialog_r2;//検索候補選択ダイアログボックスのハンドル
    static HWND Dialog_i;//登録ダイアログボックスのハンドル

    Gdiplus::Bitmap* g_bmp;//GDI+の画像オブジェクト
    Gdiplus::Graphics* g;//GDI+の描画用オブジェクト

    static LOGFONT FONT{ 40,0,0,0, FW_BOLD,TRUE,TRUE,FALSE,
    SHIFTJIS_CHARSET ,OUT_DEFAULT_PRECIS ,CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY ,0,'\0' };//定義されたフォント（構造体）

    switch (message)//繰り返しやるメイン動作
    {
    case WM_CREATE://初期動作
        EditBox_info = CreateWindow(//内容（説明文）のエディットボックス(メインウィンドウ上に作成)
            TEXT("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,
            0, 300, 1200, 304, hWnd, (HMENU)ID_E,
            ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );
        if (!EditBox_info) {//作成できなかった場合はウィンドウの作成ができなかった扱い
            throw(error);
        }
        if (!CreateWindow(//検索ボタン（メインウィンドウ上に作成）
            _T("BUTTON"),//アプリケーションの名前
            _T("検索"),//タイトルバーに現れる文字列
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//生成するウィンドウのタイプ
            0, 0,//最初に置くポジション (x, y)
            50, 50,//最初のサイズ (幅, 高さ)
            hWnd,//このウィンドウの親ウィンドウのハンドル
            (HMENU)(ID_B_1),//メニューバー（このサンプルでは使用せず）
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain関数の最初のパラメータ
            NULL//WM_CREATE情報（このアプリケーションでは使用せず）
        ))throw error;
        if (!CreateWindow(//登録ボタン（メインウィンドウ上に作成）
            _T("BUTTON"),//アプリケーションの名前
            _T("登録"),//タイトルバーに現れる文字列
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//生成するウィンドウのタイプ
            50, 0,//最初に置くポジション (x, y)
            50, 50,//最初のサイズ (幅, 高さ)
            hWnd,//このウィンドウの親ウィンドウのハンドル
            (HMENU)(ID_B_2),//メニューバー（このサンプルでは使用せず）
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain関数の最初のパラメータ
            NULL//WM_CREATE情報（このアプリケーションでは使用せず）
        ))throw error;
        if (!CreateWindow(//変更ボタン（メインウィンドウ上に作成）
            _T("BUTTON"),//アプリケーションの名前
            _T("変更"),//タイトルバーに現れる文字列
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//生成するウィンドウのタイプ
            100, 0,//最初に置くポジション (x, y)
            50, 50,//最初のサイズ (幅, 高さ)
            hWnd,//このウィンドウの親ウィンドウのハンドル
            (HMENU)(ID_B_3),//メニューバー（このサンプルでは使用せず）
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain関数の最初のパラメータ
            NULL//WM_CREATE情報（このアプリケーションでは使用せず）
        ))throw error;
        break;
    case WM_PAINT://描画
        error.error = 6;
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 0, 50, _T("単語"), _tcslen(_T("単語")));
        TextOut(hdc, 0, 150, _T("分類"), _tcslen(_T("分類")));
        if (kiban.TF) {//書いていいかどうか
            if (!SetWindowText(EditBox_info, kiban.info))throw error;//内容をエディットボックスに描画 
            Font_w = CreateFontIndirect(&FONT);//フォントハンドルの作成
            if (Font_w == NULL)throw error;
            SelectObject(hdc, Font_w);//フォントの選択
            TextOut(hdc, 50, 60, kiban.word, lstrlen(kiban.word));//単語
            TextOut(hdc, 50, 160, kiban.junl, lstrlen(kiban.junl));//分類
            SelectObject(hdc, GetStockObject(SYSTEM_FONT));//フォントのセット
            DeleteObject(Font_w);//フォントハンドルを壊す
            if (kiban.file != NULL) {//画像が指定されている
                is = getFileIStream(kiban.file, hWnd);//メモリ上の画像ファイルのハンドル取得
                if (is == NULL) {//戻り値がNULLの場合、指定された画像がない
                    EndPaint(hWnd, &ps);
                    MessageBox(NULL,
                        _T("指定された画像が存在しません!"),
                        "画像関係のエラー",
                        MB_ICONERROR);
                }
                g_bmp = new Gdiplus::Bitmap(is);//GDI+の画像のオブジェクト
                is->Release();//メモリ開放
                g = new Gdiplus::Graphics(hdc);//GDIの描画オブジェクト
                g->DrawImage(g_bmp, 500, 0);//描画
                delete g;
                g = NULL;
            }
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_B_1://検索ボタンが押された
            error.error = 1;
            if (!flag_r) {//検索ダイアログボックスor検索候補選択ダイアルボックスが開かれてないか
                flag_r = true;
                if (!(Dialog_r1 = CreateDialog(//検索ダイアルボックス作成
                    ((HINSTANCE)GetModuleHandle(0)),
                    MAKEINTRESOURCE(IDD_DIALOG1),
                    hWnd,
                    (DLGPROC)Dia_r1Proc
                )))throw error;
                ShowWindow(Dialog_r1, SW_SHOW);
            }
            else {//既に開かれている
                MessageBox(NULL,
                    _T("既に検索ダイアログか検索候補ダイアログが開いています。!"),
                    "メインウィンドウ",
                    MB_ICONERROR);
            }
            break;
        case ID_B_2://登録ボタン
            error.error = 2;
            if (!flag_i) {//登録ダイアルボックスが開かれてないか
                flag_i = true;
                if (!(Dialog_i = CreateDialog(//登録ダイアルボックス作成
                    ((HINSTANCE)GetModuleHandle(0)),
                    MAKEINTRESOURCE(IDD_DIALOG2),
                    hWnd,
                    (DLGPROC)Dia_iProc
                ))) throw(error);
                ShowWindow(Dialog_i, SW_SHOW);
            }
            else {//既に開かれている
                MessageBox(NULL,
                    _T("既に登録ダイアログが開いています。!"),
                    "メインウィンドウ",
                    MB_ICONERROR);
            }
            break;
        case ID_B_3://変更ボタンが押された
            error.error = 10;
            if (!flag_i && kiban.TF) {//登録ダイアログボックスが開いているか
                flag_i = true;
                if (MessageBox(NULL,
                    _T("単語の内容をメインウィンドウのように変更しますか？"),
                    "確認メッセージ",
                    MB_YESNO) != IDYES) {//いいえが選択された
                    flag_i = false;
                    break;
                }
                str = new TCHAR[TCHAR_INFO];
                if (!GetWindowText(EditBox_info, str, sizeof(TCHAR) * TCHAR_INFO))throw error;//エディットボックスの文字列取得
                //if (!SendMessageA(EditBox_info, WM_GETTEXT, sizeof(TCHAR) * TCHAR_JUNL, (LPARAM)str))throw error;
                if (kiban.run(str, sizeof(TCHAR) * TCHAR_JUNL) == -1) {//失敗
                    MessageBox(NULL,
                        _T("単語の変更に失敗しました!"),
                        "メインウィンドウ",
                        MB_ICONERROR);
                }
                else {//成功
                    MessageBox(NULL,
                        _T("単語の変更に成功しました。"),
                        "メインウィンドウ",
                        MB_ICONERROR);
                }
                delete[] str;
                str = NULL;
            }
            else {
                if (!kiban.TF)//書き込み等が不可能な状態
                    MessageBox(NULL,
                        _T("今はデータの書き換え等ができません!"),
                        "メインウィンドウ",
                        MB_ICONERROR);
                else //既に登録ダイアルボックスが開いている
                    MessageBox(NULL,
                        _T("先に登録を済ましてください!"),
                        "メインウィンドウ",
                        MB_ICONERROR);
            }
            flag_i = false;
            break;
        }
        break;
    case MY_CREARE_DLG3://検索候補選択ボックスの生成メッセージ
        if (!(Dialog_r2 = CreateDialog(//検索ダイアルボックス作成
            ((HINSTANCE)GetModuleHandle(0)),
            MAKEINTRESOURCE(IDD_DIALOG3),
            hWnd,
            (DLGPROC)Dia_r2Proc
        )))throw error;
        ShowWindow(Dialog_r2, SW_SHOW);
        break;
    case WM_DESTROY://×   
        if (!DeFlag) {
            DeFlag = true;
        }
        else {
            PostQuitMessage(0);//メッセージループ終了
            break;
        }
        if (flag_r && IsWindowVisible(Dialog_r2)) {//ここから検索候補選択ダイアルボックスが開いていたらそれを壊す
            DestroyWindow(Dialog_r2);
        }//ここまで
        else if (Dialog_r1 != NULL && IsWindowVisible(Dialog_r1)) DestroyWindow(Dialog_r1);//検索ダイアログボックスが開いていたらそれを壊す
        if (flag_i && Dialog_i != NULL && IsWindowVisible(Dialog_i))DestroyWindow(Dialog_i);//登録ダイアログボックスが開いていたらそれを壊す
        g = NULL;
        delete[] str;
        str = NULL;
        delete Kensaku;
        Kensaku = NULL;
        PostQuitMessage(0);//メッセージループ終了
        return -1;
        break;
    default://何もしない
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

BOOL CALLBACK Dia_r1Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND Dialog_r1_ed1;//エディットボックス1のハンドル
    static HWND Dialog_r1_ed2;//エディットボックス1のハンドル
    HWND Dialog_r2;//検索候補選択ダイアルボックスを生成するときに使う
    MyError error = { 2,hWnd };//エラーハンドル
    TCHAR* str=NULL;//文字列取り扱い用

    switch (iMsg)
    {
    case WM_INITDIALOG:
        //ダイアログボックスが生成されたとき
        error.error = 7;
        Dialog_r1_ed1 = GetDlgItem(hWnd, IDC_EDIT1);//エディットボックス1のハンドル取得
        if (!Dialog_r1_ed1)throw(error);
        Dialog_r1_ed2 = GetDlgItem(hWnd, IDC_EDIT2);//エディットボックス1のハンドル取得
        if (!Dialog_r1_ed2)throw(error);
        Kensaku = new KIBAN_KENSAKU;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK://検索ボタンが押された
            if (flag_i) {//登録ダイアルボックスが開いている
                MessageBox(NULL,
                    _T("先に単語の登録を澄ましてください"),
                    "お願い",
                    MB_ICONERROR);
                return TRUE;
            }
            Kensaku->set();//リセット
            str = new TCHAR[TCHAR_WORD];
            if (GetWindowText(Dialog_r1_ed1, str, sizeof(TCHAR) * TCHAR_WORD) != 0) {//単語名が入力されているか
                Kensaku->set(str, sizeof(TCHAR) * TCHAR_WORD);//単語名のセット
                delete[] str;
                str = NULL;
                str = NULL;
                str = new TCHAR[TCHAR_JUNL];
                if (GetWindowText(Dialog_r1_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) != 0)//分類名が入力されているか
                    Kensaku->set(str, sizeof(TCHAR) * TCHAR_JUNL);//分類名のセット
                delete[] str;
                str = NULL;
                if (Kensaku->run() == -1) {//単語がヒットしなかった
                    MessageBox(NULL,
                        _T("単語が存在しませんでした！"),
                        "失敗",
                        MB_ICONERROR);
                    delete Kensaku;////ここから自身を壊す
                    Kensaku = NULL;
                    flag_r = false;
                    DestroyWindow(hWnd);////ここまで
                    return TRUE;
                }
                if (Kensaku->GetStats() == 1) {//単語が一つだけヒット
                    
                    kiban = (*Kensaku)[0];//受け渡し
                    MessageBox(NULL,
                        _T("単語の検索に成功しました。"),
                        "成功",
                        MB_ICONERROR);
                    delete Kensaku;////ここから自身を壊す
                    Kensaku = NULL;
                    flag_r = false;
                    InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//再描画
                    DestroyWindow(hWnd);////ここまで
                    return TRUE;
                }//ここからは検索候補選択ダイアログボックスの生成・引継ぎ
                DestroyWindow(hWnd);//自身をこの一行で壊す
                SendMessageA(FindWindow(Aplecat_Name, NULL), MY_CREARE_DLG3,NULL,NULL);//検索候補選択ボックスの生成メッセージ（メインウィンドウにて）
                return true;
            }
            else {//単語名が未入力
                MessageBox(NULL,
                    _T("単語を入力してください！"),
                    "注意",
                    MB_ICONERROR);
                return TRUE;
            }
        case IDCANCEL://キャンセルボタンが押された
            //モードレスダイアログボックスを破棄  
            delete Kensaku;////ここで自身を壊す
            Kensaku = NULL;
            flag_r = false;
            DestroyWindow(hWnd);////ここまで
            break;
            return TRUE;
        }
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK Dia_r2Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static HWND StaticBox = NULL;//スタティックボックスのハンドル
    static HWND RSwitch[5] = { NULL,NULL,NULL,NULL,NULL };//チェックボタンのハンドルの配列
    MyError error = { 0,hWnd };//エラーハンドル
    std::string sstr;//数値が絡む文字列の作成用
    CString str,word;//文字列作成用
    int CPoint=0;//チェックボックスのチェック位置等に使われる
    static int now;//今表示されているやつを表すための変数。
    switch (iMsg) {
    case WM_INITDIALOG://初期動作
        now = 0;
        error.error = 9;
        StaticBox = CreateWindow(
            "static",                           // スタティックコントロールのクラス名
            "text is here.",                    // 表示する文字列
            SS_SIMPLE | WS_CHILD | WS_VISIBLE,  // 詳細はあとで。
            5, 5,                               // 左上の座標
            500, 100,                           // 幅と高さ
            hWnd,     // 親ウインドウのウインドウハンドル
            NULL,                               // メニューハンドル。NULLでよい。
            ((HINSTANCE)GetModuleHandle(0)), // アプリケーションのインスタンスハンドル。
            NULL                                // ウインドウ作成データ。NULLでよい
        );
        if (!StaticBox)throw error;
        RSwitch[0]= GetDlgItem(hWnd, IDC_CHECK1);//チェックボックスのハンドル取得
        RSwitch[1] = GetDlgItem(hWnd, IDC_CHECK2);//チェックボックスのハンドル取得
        RSwitch[2] = GetDlgItem(hWnd, IDC_CHECK3);//チェックボックスのハンドル取得
        RSwitch[3] = GetDlgItem(hWnd, IDC_CHECK4);//チェックボックスのハンドル取得
        RSwitch[4] = GetDlgItem(hWnd, IDC_CHECK5);//チェックボックスのハンドル取得
        for (int i = 0; i < 5; i++) if (!RSwitch[i]) throw error;//エラー判定
        str = _T("次の中から検索したい単語を選択してください。 ");////ここからスタティックボックスの文字列作成
        sstr = "";//////ここから数値が絡む文字列の作成
        sstr = std::to_string(Kensaku->GetStats());
        sstr += "件中  1～";
        sstr += std::to_string(Kensaku->GetStats()>=5?5: Kensaku->GetStats());
        sstr += "件目";//////ここまで
        str += sstr.c_str();////ここまで
        if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//スタティックボックスに文字列を表示
        for (int i = 1; i <= 5 && i <= Kensaku->GetStats(); i++) {//チェックボックスに最初のやつを表示
            str = _T("単語名::");////文字列の作成
            str += Kensaku->word;
            str += _T("　分類名::");
            str += (*Kensaku)[i].junl;//ここまで
            if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//チェックボックスの表示のセット
        }
        return TRUE;
    case WM_COMMAND:
        error.error = 11;
        switch (LOWORD(wParam))
        {
        case ID_NEXT://次へボタンが押された
            if (Kensaku->GetStats() <= (now + 1) * 5) {//これ以上候補がない
                MessageBox(hWnd,
                    _T("これ以上の単語はヒットしていません！"),
                    "注意",
                    MB_ICONERROR);
                return TRUE;
            }
            for (int i = 1; i <= 5; i++) {//チェックボックスに反映
                CPoint = i + 5*(now + 1);//何個目の検索候補か
                if (CPoint > Kensaku->GetStats()) {//検索候補数を超えてる
                    if (!SetWindowTextA(RSwitch[i - 1], _T("")))throw error;//チェックボックスに文字列を表示
                }
                else {
                    str = _T("単語名::");////ここから文字列作成
                    str += Kensaku->word;
                    str += _T("　分類名::");
                    str += (*Kensaku)[CPoint].junl;////ここまで
                    if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//チェックボックスに文字列を表示
                }
            }
            ++now;
            str = _T("次の中から検索したい単語を選択してください。  ");////文字列の作成
            sstr = "";//////ここから数値が絡む文字列の作成
            sstr += std::to_string(Kensaku->GetStats());
            sstr += std::to_string((now)*5+1);
            sstr += "件中～";
            sstr += std::to_string(Kensaku->GetStats() >= 5 ? (now + 1) * 5 : Kensaku->GetStats());
            sstr += "件目";//////ここまで
            str += _T(sstr.c_str());////ここまで
            if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//スタティックボックスに文字列を表示
            return TRUE;
        case ID_BACK://戻るボタンが押された
            if (now<=0) {//これ以上戻れない
                MessageBox(hWnd,
                    _T("今の状態で一件目から表示されています！"),
                    "注意",
                    MB_ICONERROR);
                return TRUE;
            }
            for (int i = 1; i <= 5; i++) {//チェックボックスに反映
                CPoint = i + 5*(now-1);//何個目の検索候補か
                str = _T("単語名::");////ここから文字列作成
                str += Kensaku->word;
                str += _T("　分類名::");
                str += (*Kensaku)[CPoint].junl;////ここまで
                if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//チェックボックスに文字列を表示
            }
            --now;
            str = _T("次の中から検索したい単語を選択してください。  ");////文字列の作成
            sstr = "";//////ここから数値が絡む文字列の作成
            sstr += std::to_string(Kensaku->GetStats());
            sstr += "件中";
            sstr += std::to_string((now) * 5 + 1);
            sstr += "～";
            sstr += std::to_string(Kensaku->GetStats() >= 5 ? (now + 1) * 5 : Kensaku->GetStats());
            sstr += "件目";//////ここまで
            str += _T(sstr.c_str());////ここまで
            if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//スタティックボックスに文字列を表示
            return TRUE;
        case IDOK://決定ボタンが押された
            if (flag_i) {//登録ダイアルボックスが開いている
                MessageBox(hWnd,
                    _T("先に単語の登録を済ませてください。"),
                    "案内",
                    MB_ICONERROR);
                return TRUE;
            }
            CPoint = -100;//初期値（判定用）
            for (int i = 0; i < 5; i++) {
                if (BST_CHECKED == SendMessage(RSwitch[i], BM_GETCHECK, 0, 0)) {//チェックがついている
                    if (CPoint == -100)CPoint = i + 1;//1つ目
                    else CPoint = -1;//二つ目
                }
            }
            if (CPoint < 0) {//「一つのみにチェックを入れる」という条件を満たしていない
                MessageBox(hWnd,
                    _T("チェックボックスのチェックは必ず一つにチェックしてください！"),
                    "注意",
                    MB_ICONERROR);
                return TRUE;
            }
            if ((Kensaku->GetStats() - now * 5) % 5 < CPoint) {
                MessageBox(hWnd,
                    _T("そのチェックは対応していません！"),
                    "注意",
                    MB_ICONERROR);
                return TRUE;
            }
            kiban = (*Kensaku)[now * 5 + CPoint];//該当の単語情報をkibanにセット
            delete Kensaku;////ここから自身を壊す
            Kensaku = NULL;
            flag_r = false;
            InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//再描画
            DestroyWindow(hWnd);////ここまで
            return TRUE;
        case IDCANCEL:
            delete Kensaku;////ここから自身を壊す
            Kensaku = NULL;
            flag_r = false;
            DestroyWindow(hWnd);////ここまで
        }
    default:
        return FALSE;
    }
}

BOOL CALLBACK Dia_iProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND Dialog_i_ed1;//エディットボックス1のハンドル
    static HWND Dialog_i_ed2;//エディットボックス2のハンドル
    static HWND Dialog_i_ed3;//エディットボックス3のハンドル
    static HWND Dialog_i_ed4;//エディットボックス4のハンドル
    MyError error = { 3,hWnd };//エラーハンドル
    TCHAR* str=NULL;//文字列取り扱い用
    switch (iMsg)
    {
    case WM_INITDIALOG:
        //ダイアログボックスが生成されたとき
        error.error = 8;
        Dialog_i_ed1 = GetDlgItem(hWnd, IDC_EDIT1);//エディットボックス1のハンドル取得
        if (!Dialog_i_ed1)throw(error);
        Dialog_i_ed2 = GetDlgItem(hWnd, IDC_EDIT2);//エディットボックス2のハンドル取得
        if (!Dialog_i_ed2)throw(error);
        Dialog_i_ed3 = GetDlgItem(hWnd, IDC_EDIT3);//エディットボックス3のハンドル取得
        if (!Dialog_i_ed3)throw(error);
        Dialog_i_ed4 = GetDlgItem(hWnd, IDC_EDIT4);//エディットボックス4のハンドル取得
        if (!Dialog_i_ed4)throw(error);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK://登録ボタンが押された
            kiban.set();//リセット
            str = new TCHAR[TCHAR_JUNL];
            if (GetWindowText(Dialog_i_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) != 0) {//単語名が入力されている
                kiban.set(str, sizeof(TCHAR) * TCHAR_JUNL);
                delete[] str;
                str = NULL;
                str = new TCHAR[TCHAR_WORD];
                if (GetWindowText(Dialog_i_ed1, str, sizeof(TCHAR) * TCHAR_WORD) != 0) {//分類名が入力されている
                    kiban.set(str, sizeof(TCHAR) * TCHAR_WORD);
                    delete[] str;
                    str = NULL;
                    str = new TCHAR[TCHAR_INFO];
                    if (GetWindowText(Dialog_i_ed4, str, sizeof(TCHAR) * TCHAR_INFO) != 0) {//内容が入力されている
                        kiban.set(str, sizeof(TCHAR) * TCHAR_INFO);
                        delete[] str;
                        str = NULL;
                        str = new TCHAR[TCHAR_FILE];
                        if (GetWindowText(Dialog_i_ed3, str, sizeof(TCHAR) * TCHAR_FILE) != 0)//画像パスが入力されている
                            kiban.set(str, sizeof(TCHAR) * TCHAR_FILE);
                        delete[] str;
                        str = NULL;
                        if (kiban.run() == -1) {//失敗
                            MessageBox(hWnd,// オーナーウィンドウのハンドル
                                _T("単語の登録に失敗しました"),// メッセージボックス内のテキスト
                                _T("失敗！"),// メッセージボックスのタイトル
                                NULL// メッセージボックスのスタイル
                            );
                            flag_i = false;////ここから自身を壊す
                            DestroyWindow(hWnd);////ここまで
                            break;
                        }
                        else {//成功
                            MessageBox(hWnd,// オーナーウィンドウのハンドル
                                _T("単語の登録に成功しました"),// メッセージボックス内のテキスト
                                _T("成功しました！"),// メッセージボックスのタイトル
                                NULL// メッセージボックスのスタイル
                            );
                            flag_i = false;////ここから自身を壊す
                            InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//再描画
                            DestroyWindow(hWnd);////ここまで
                            break;
                        }
                    }
                }
            }//ここからは必須項目が入力されていない場合の処理
            delete[] str;
            str = NULL;
            MessageBox(hWnd,// オーナーウィンドウのハンドル
                _T("単語・分類・内容は必ず入力してください！\n必須項目です。"),// メッセージボックス内のテキスト
                _T("注意"),// メッセージボックスのタイトル
                NULL// メッセージボックスのスタイル
            );
            break;
            return TRUE;
        case IDCANCEL://キャンセルボタンが押された
            //モードレスダイアログボックスを破棄
            flag_i = false;////ここから自身を壊す
            DestroyWindow(hWnd);////ここまで
            break;
            return TRUE;
        }
        return TRUE;
    default:
        return FALSE;
    }
    return TRUE;
}
IStream* getFileIStream(const char* lpszPath,HWND hWnd) {

    HANDLE hFile;//ファイルハンドル
    HGLOBAL hBuf;//確保されたメモリのハンドル
    LPVOID lpBuf;//ロックされたメモリ領域のハンドル
    IStream* isFile;//画像がロードされたメモリのハンドル
    DWORD dwFileSize, dwLoadSize;//ファイルのサイズ

    MyError error = { 4,hWnd };//エラーハンドル

    /* 画像ファイルオープン */
    hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFile)return NULL;
    /* ファイルサイズ取得 */
    dwFileSize = GetFileSize(hFile, NULL);
    if (!dwFileSize) {
        CloseHandle(hFile);
        return NULL;
    }
    /* 画像ファイルデータ格納用メモリブロック確保 */
    hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    if (hBuf == NULL) {
        CloseHandle(hFile);
        throw error;
    }
    /* メモリブロックをロックしアドレスを取得 */
    lpBuf = GlobalLock(hBuf);
    if (lpBuf == NULL) {
        CloseHandle(hFile);
        throw error;
    }
    /* 画像ファイルのデータをメモリブロックに読み込む */
    if (!ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL)) {
        CloseHandle(hFile);
        GlobalUnlock(hBuf);
        throw error;
    }

    CloseHandle(hFile);

    /* メモリブロックのロック解除 */
    GlobalUnlock(hBuf);

    /* メモリブロックからIStreamを作成 */
    CreateStreamOnHGlobal(hBuf, TRUE, &isFile);

    /* IStreamを返す */
    return isFile;
}
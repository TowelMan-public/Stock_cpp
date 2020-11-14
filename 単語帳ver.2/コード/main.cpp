#define STRICT

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <gdiplus.h>
#include <olectl.h>
#include "lpfnWndProc.h"

#pragma	comment(lib, "Gdiplus.lib")

// シンボル定義及びマクロ
#ifndef Aplecat_Name
#define Aplecat_Name _T("単語帳ver.2")
#endif //Aplecat_Name

void NoneErrorHandler(const wchar_t* expression, const wchar_t* function,
    const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
    MyError error{ 12,(HWND)1 };//エラーハンドル
    throw error;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyError error{ 0,NULL };
    Gdiplus::GdiplusStartupInput gpSI;
    ULONG_PTR lpToken;
    Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);
    WNDCLASSEX wcex;//ウィンドウクラス
    HWND hWnd = NULL;//メインウィンドウハンドル
    _invalid_parameter_handler oldHandler = _set_invalid_parameter_handler(NoneErrorHandler);
    try {
        wcex.cbSize = sizeof(WNDCLASSEX); // 構造体のサイズ
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // クラスのスタイル
        wcex.lpfnWndProc = WndProc; // ウインドウプロシージャ
        wcex.cbClsExtra = 0; // 補助メモリ
        wcex.cbWndExtra = 0; // 補助メモリ
        wcex.hInstance = hInstance;  // インスタンスハンドル
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // アイコン
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 背景ブラシ
        wcex.lpszMenuName = NULL; // メニュー名
        wcex.lpszClassName = Aplecat_Name; // クラス名
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // 小さいアイコン

        if (!RegisterClassExA(&wcex)) throw (error);

        hWnd = CreateWindow(
            Aplecat_Name,//アプリケーションの名前
            Aplecat_Name,//タイトルバーに現れる文字列
            WS_OVERLAPPEDWINDOW,//生成するウィンドウのタイプ
            CW_USEDEFAULT, CW_USEDEFAULT,//最初に置くポジション (x, y)
            1200, 608,//最初のサイズ (幅, 高さ)
            NULL,//このウィンドウの親ウィンドウのハンドル
            NULL,//メニューバー（このサンプルでは使用せず）
            hInstance,//WinMain関数の最初のパラメータ
            NULL//WM_CREATE情報（このアプリケーションでは使用せず）
        );
        //  ウィンドウが生成できなかった場合
        if (!hWnd)throw(error);
        ShowWindow(hWnd, nCmdShow);
        // メインのメッセージループ:
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
                    _T("メインウィンドウの初期動作に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 1:
                MessageBox(NULL,
                    _T("検索ダイアログボックスの作成に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 2:
                MessageBox(NULL,
                    _T("登録ダイアルボックスの作成に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 3:
                MessageBox(NULL,
                    _T("検索候補選択ダイアログボックスの作成に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 4:
                MessageBox(NULL,
                    _T("画像のロード中に重大なエラーが発生しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 6:
                MessageBox(NULL,
                    _T("メインウィンドウの描画処理に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 7:
                MessageBox(NULL,
                    _T("検索ダイアルボックスの初期動作に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 8:
                MessageBox(NULL,
                    _T("登録ダイアルボックスの初期動作に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 9:
                MessageBox(NULL,
                    _T("検索候補ダイアログボックスの初期動作に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 10:
                MessageBox(NULL,
                    _T("エディットボックスからの文字列の取得に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                break;
            case 11:
                MessageBox(NULL,
                    _T("検索候補選択ダイアルボックスの動作で重大なエラーが発生しました！"),
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
                _T("ウィンドウの生成に失敗しました！"),
                Aplecat_Name,
                MB_ICONERROR);
            Gdiplus::GdiplusShutdown(lpToken);
            oldHandler = _set_invalid_parameter_handler(oldHandler);
            return -1;
        }
    }
    catch (...) {
        MessageBox(NULL,
            _T("予期しないエラーが発生しました!"),
            Aplecat_Name,
            MB_ICONERROR);
        Gdiplus::GdiplusShutdown(lpToken);
        oldHandler = _set_invalid_parameter_handler(oldHandler);
        return 1;
    }
}
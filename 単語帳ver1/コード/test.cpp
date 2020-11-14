#define STRICT

#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include <gdiplus.h>
#include <olectl.h>
#include "lpfnWndProc.h"

#pragma	comment(lib, "Gdiplus.lib")

// シンボル定義及びマクロ
#define Aplecat_Name _T("単語帳ver1")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MyError error{ 0,0 };
    Gdiplus::GdiplusStartupInput gpSI;
    ULONG_PTR lpToken;
    Gdiplus::GdiplusStartup(&lpToken, &gpSI, NULL);
    try {
        WNDCLASSEX wcex;
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

        HWND hWnd = CreateWindow(
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
        error.hWnd = hWnd;
        // メインのメッセージループ:
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
                    _T("ダイアルボックスの作成に失敗しました!"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            case 2:
                MessageBox(Error.hWnd,
                    _T("検索ダイアルボックスで単語を検索しようとしてるときに単語の登録に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            case 3:
                MessageBox(Error.hWnd,
                    _T("登録ダイアルボックスで単語を検索しようとしてるときに単語の登録に失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            case 5:
                MessageBox(Error.hWnd,
                    _T("単語の検索か登録をしている最中にエラーが発生しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            case 7:
                MessageBox(Error.hWnd,
                    _T("画像ファイルのロードに失敗しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
            default:
                MessageBox(Error.hWnd,
                    _T("その他のエラーが発生しました！"),
                    Aplecat_Name,
                    MB_ICONERROR);
                Gdiplus::GdiplusShutdown(lpToken);
                return 1;
            }
        }
        else {
            MessageBox(NULL,
                _T("ウィンドウ生成に失敗しました!"),
                Aplecat_Name,
                MB_ICONERROR);
           Gdiplus::GdiplusShutdown(lpToken);
           PostQuitMessage(0);
           return 1;
        }
    }
    catch (...) {
        MessageBox(NULL,
            _T("予期しないエラーが発生しました!"),
            Aplecat_Name,
            MB_ICONERROR);
        Gdiplus::GdiplusShutdown(lpToken);
        PostQuitMessage(0);
        return 1;
    }
}
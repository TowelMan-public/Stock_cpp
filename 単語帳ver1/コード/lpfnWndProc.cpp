#include "lpfnWndProc.h"

//�K�v�Ȓ�`��(���������Q�[�W)
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
        switch (message)//�J��Ԃ���郁�C������
        {
        case WM_CREATE:
            MasterHWND = hWnd;
            EditBox_info = CreateWindow(//�������̃G�f�B�b�g�{�b�N�X(���C���E�B���h�E��ɍ쐬)
                TEXT("EDIT"), NULL,
                WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
                ES_AUTOHSCROLL | ES_AUTOVSCROLL |
                ES_LEFT | ES_MULTILINE,
                0, 300, 1200, 304, hWnd, (HMENU)ID_E_1,
                ((LPCREATESTRUCT)(lParam))->hInstance, NULL
            );
            button_r = CreateWindow(//�����{�^���i���C���E�B���h�E��ɍ쐬�j
                _T("BUTTON"),//�A�v���P�[�V�����̖��O
                _T("����"),//�^�C�g���o�[�Ɍ���镶����
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//��������E�B���h�E�̃^�C�v
                0, 0,//�ŏ��ɒu���|�W�V���� (x, y)
                50, 50,//�ŏ��̃T�C�Y (��, ����)
                hWnd,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
                (HMENU)(ID_B_1),//���j���[�o�[�i���̃T���v���ł͎g�p�����j
                ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
                NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
            );
            button_i = CreateWindow(//�o�^�{�^���i���C���E�B���h�E��ɍ쐬�j
                _T("BUTTON"),//�A�v���P�[�V�����̖��O
                _T("�o�^"),//�^�C�g���o�[�Ɍ���镶����
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//��������E�B���h�E�̃^�C�v
                50, 0,//�ŏ��ɒu���|�W�V���� (x, y)
                50, 50,//�ŏ��̃T�C�Y (��, ����)
                hWnd,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
                (HMENU)(ID_B_2),//���j���[�o�[�i���̃T���v���ł͎g�p�����j
                ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
                NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
            );
            if (!EditBox_info || !button_r || !button_i) {//�쐬�ł��Ȃ������ꍇ�̓E�B���h�E�̍쐬���ł��Ȃ���������
                error.hWnd = NULL;
                throw(error);
            }
            break;
        case WM_PAINT://�`��        
            hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 0, 50, _T("�P��"), _tcslen(_T("�P��")));
            TextOut(hdc, 0, 150, _T("����"), _tcslen(_T("����")));
            if (kiban.TF) {//�����Ă������ǂ���
                SetWindowText(EditBox_info, kiban.info);//���e���G�f�B�b�g�{�b�N�X�ɕ`�� 
                Font_w = CreateFontIndirect(&FONT);
                if (Font_w == NULL)throw error;
                SelectObject(hdc, Font_w);
                TextOut(hdc, 50, 60, kiban.word, lstrlen(kiban.word));//�P��
                TextOut(hdc, 50, 160, kiban.junl, lstrlen(kiban.junl));//����
                SelectObject(hdc, GetStockObject(SYSTEM_FONT));
                DeleteObject(Font_w);
                if (kiban.file != NULL) {
                    is = getFileIStream(kiban.file, hWnd);//��������̉摜�t�@�C���̃n���h���擾
                    if (is == NULL) {//�߂�l��NULL�̏ꍇ�Aint��O��0��
                        throw 0;
                        EndPaint(hWnd, &ps);
                        throw 0;
                    }
                    g_bmp = new Gdiplus::Bitmap(is);//GDI+�̉摜�̃I�u�W�F�N�g
                    is->Release();//�������J��
                    g = new Gdiplus::Graphics(hdc);//GDI�̕`��I�u�W�F�N�g
                    g->DrawImage(g_bmp, 500, 0);//�`��
                    delete g;
                }
            }
            EndPaint(hWnd, &ps);
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case ID_B_1:
                if (!flag_r && !flag_i) {//���ɂ��_�C�A���{�b�N�X���J����ĂȂ���
                    flag_r = true;
                    Dialog_r = CreateDialog(//�����_�C�A���{�b�N�X�쐬
                        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
                        MAKEINTRESOURCE(IDD_DIALOG1),
                        hWnd,
                        (DLGPROC)Dia_rProc
                    );
                    if (!Dialog_r)throw(error);
                    ShowWindow(Dialog_r, SW_SHOW);
                    Dialog_r_ed1 = GetDlgItem(Dialog_r, IDC_EDIT1);//�G�f�B�b�g�{�b�N�X�̃n���h���擾
                    if (!Dialog_r_ed1)throw(error);
                    Dialog_r_ed2 = GetDlgItem(Dialog_r, IDC_EDIT2);
                    if (!Dialog_r_ed2)throw(error);
                }
                break;
            case ID_B_2:
                if (!flag_r && !flag_i) {//���ɂ��_�C�A���{�b�N�X���J����ĂȂ���
                    flag_i = true;
                    Dialog_i = CreateDialog(//�o�^�_�C�A���{�b�N�X�쐬
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
        case WM_DESTROY://�~
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
        case 0://�摜���J���Ȃ��ăp�X���^�킵����
            MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                _T("���Ȃ������̒P��Ɏw�肵���摜�p�X�������ł��B\n���̃\�t�g�̏ꏊ���ړ�������o�^�����摜���ړ������肷��Ƃ����������Ƃ��N��ꍇ������܂��B"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                _T("�p�X�������ł��B"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
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
        //�_�C�A���O�{�b�N�X���������ꂽ�Ƃ�
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
            case 3://�P�ꂪ���݂��Ȃ�
                MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                    _T("���̒P��͑��݂��܂���ł���"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                    _T("���s"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                    NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
                );
                return TRUE;
            }
            DestroyWindow(hWnd);
            flag_r = false;
            InvalidateRect(MasterHWND, NULL, TRUE);
            return TRUE;
        case IDCANCEL:
            //���[�h���X�_�C�A���O�{�b�N�X��j��       
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
    MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
        _T("�܂����͂���Ă��Ȃ��Ƃ��낪����܂��B\n���̍��ڂ̓��e����͂��Ă��炲����������"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
        _T("���ē�"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
        NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
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
        //�_�C�A���O�{�b�N�X���������ꂽ�Ƃ�
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
            case 0://�o�^�ɐ�������
                MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                    _T("�P��̓o�^�ɐ������܂���"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                    _T("�������܂����I"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                    NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
                );
                break;
            case 1://�P�ꂪ���݂���
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 2://�o�^�Ɏ��s����
                MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                    _T("�P��̓o�^�Ɏ��s���܂���"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                    _T("���s�I"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                    NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
                );
                break;
            }
            DestroyWindow(hWnd);
            flag_i = false;
            break;
            return TRUE;
        case IDCANCEL:
            //���[�h���X�_�C�A���O�{�b�N�X��j��       
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
    MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
        _T("�܂��摜�p�X�ȊO�œ��͂���Ă��Ȃ��Ƃ��낪����܂��B\n���̍��ڂ̓��e����͂��Ă��炲�o�^������"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
        _T("���ē�"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
        NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
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

    /* �摜�t�@�C���I�[�v�� */
    hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    /* �t�@�C���T�C�Y�擾 */
    dwFileSize = GetFileSize(hFile, NULL);

    /* �摜�t�@�C���f�[�^�i�[�p�������u���b�N�m�� */
    hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    if (hBuf == NULL)throw error;
    /* �������u���b�N�����b�N���A�h���X���擾 */
    lpBuf = GlobalLock(hBuf);
    /* �摜�t�@�C���̃f�[�^���������u���b�N�ɓǂݍ��� */
    if (!ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL))return NULL;

    CloseHandle(hFile);

    /* �������u���b�N�̃��b�N���� */
    GlobalUnlock(hBuf);

    /* �������u���b�N����IStream���쐬 */
    CreateStreamOnHGlobal(hBuf, TRUE, &isFile);

    /* IStream��Ԃ� */
    return isFile;
}
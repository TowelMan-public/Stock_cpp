#include "lpfnWndProc.h"

//�K�v�Ȓ�`��(���������Q�[�W)
static bool flag_i = false;//�o�^�_�C���O�{�b�N�X���J���Ă��邩�̃t���O
static bool flag_r = false;//�������I���_�C�A���O�{�b�N�X���J���Ă��邩�̃t���O
static KIBAN kiban;//�P����̎�舵���y�ѓo�^����
static KIBAN_KENSAKU *Kensaku;//�P����̎󂯓n���y�ь�������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MyError error = { 0,hWnd };//�G���[�n���h��
    PAINTSTRUCT ps;//�`�揈���Ŏg��
    HDC hdc;//�`�揈���Ŏg��
    HFONT Font_w;//�t�H���g�n���h��
    bool DeFlag = false;//WM_DESTROY:�̎��s����x�݂̂ɂ��邽�߂̃t���O
    static IStream* is=NULL;//��������Ƀ��[�h����Ă�����̂̃n���h��
    static HWND EditBox_info;//���C���E�B���h�E�̃G�f�B�b�g�{�b�N�X�̃n���h��
    static TCHAR* str;//�������舵���p

    static HWND Dialog_r1;//�����_�C�A���O�{�b�N�X�̃n���h��
    static HWND Dialog_r2;//�������I���_�C�A���O�{�b�N�X�̃n���h��
    static HWND Dialog_i;//�o�^�_�C�A���O�{�b�N�X�̃n���h��

    Gdiplus::Bitmap* g_bmp;//GDI+�̉摜�I�u�W�F�N�g
    Gdiplus::Graphics* g;//GDI+�̕`��p�I�u�W�F�N�g

    static LOGFONT FONT{ 40,0,0,0, FW_BOLD,TRUE,TRUE,FALSE,
    SHIFTJIS_CHARSET ,OUT_DEFAULT_PRECIS ,CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY ,0,'\0' };//��`���ꂽ�t�H���g�i�\���́j

    switch (message)//�J��Ԃ���郁�C������
    {
    case WM_CREATE://��������
        EditBox_info = CreateWindow(//���e�i�������j�̃G�f�B�b�g�{�b�N�X(���C���E�B���h�E��ɍ쐬)
            TEXT("EDIT"), NULL,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
            ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,
            0, 300, 1200, 304, hWnd, (HMENU)ID_E,
            ((LPCREATESTRUCT)(lParam))->hInstance, NULL
        );
        if (!EditBox_info) {//�쐬�ł��Ȃ������ꍇ�̓E�B���h�E�̍쐬���ł��Ȃ���������
            throw(error);
        }
        if (!CreateWindow(//�����{�^���i���C���E�B���h�E��ɍ쐬�j
            _T("BUTTON"),//�A�v���P�[�V�����̖��O
            _T("����"),//�^�C�g���o�[�Ɍ���镶����
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//��������E�B���h�E�̃^�C�v
            0, 0,//�ŏ��ɒu���|�W�V���� (x, y)
            50, 50,//�ŏ��̃T�C�Y (��, ����)
            hWnd,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
            (HMENU)(ID_B_1),//���j���[�o�[�i���̃T���v���ł͎g�p�����j
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
            NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
        ))throw error;
        if (!CreateWindow(//�o�^�{�^���i���C���E�B���h�E��ɍ쐬�j
            _T("BUTTON"),//�A�v���P�[�V�����̖��O
            _T("�o�^"),//�^�C�g���o�[�Ɍ���镶����
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//��������E�B���h�E�̃^�C�v
            50, 0,//�ŏ��ɒu���|�W�V���� (x, y)
            50, 50,//�ŏ��̃T�C�Y (��, ����)
            hWnd,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
            (HMENU)(ID_B_2),//���j���[�o�[�i���̃T���v���ł͎g�p�����j
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
            NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
        ))throw error;
        if (!CreateWindow(//�ύX�{�^���i���C���E�B���h�E��ɍ쐬�j
            _T("BUTTON"),//�A�v���P�[�V�����̖��O
            _T("�ύX"),//�^�C�g���o�[�Ɍ���镶����
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,//��������E�B���h�E�̃^�C�v
            100, 0,//�ŏ��ɒu���|�W�V���� (x, y)
            50, 50,//�ŏ��̃T�C�Y (��, ����)
            hWnd,//���̃E�B���h�E�̐e�E�B���h�E�̃n���h��
            (HMENU)(ID_B_3),//���j���[�o�[�i���̃T���v���ł͎g�p�����j
            ((LPCREATESTRUCT)(lParam))->hInstance,//WinMain�֐��̍ŏ��̃p�����[�^
            NULL//WM_CREATE���i���̃A�v���P�[�V�����ł͎g�p�����j
        ))throw error;
        break;
    case WM_PAINT://�`��
        error.error = 6;
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 0, 50, _T("�P��"), _tcslen(_T("�P��")));
        TextOut(hdc, 0, 150, _T("����"), _tcslen(_T("����")));
        if (kiban.TF) {//�����Ă������ǂ���
            if (!SetWindowText(EditBox_info, kiban.info))throw error;//���e���G�f�B�b�g�{�b�N�X�ɕ`�� 
            Font_w = CreateFontIndirect(&FONT);//�t�H���g�n���h���̍쐬
            if (Font_w == NULL)throw error;
            SelectObject(hdc, Font_w);//�t�H���g�̑I��
            TextOut(hdc, 50, 60, kiban.word, lstrlen(kiban.word));//�P��
            TextOut(hdc, 50, 160, kiban.junl, lstrlen(kiban.junl));//����
            SelectObject(hdc, GetStockObject(SYSTEM_FONT));//�t�H���g�̃Z�b�g
            DeleteObject(Font_w);//�t�H���g�n���h������
            if (kiban.file != NULL) {//�摜���w�肳��Ă���
                is = getFileIStream(kiban.file, hWnd);//��������̉摜�t�@�C���̃n���h���擾
                if (is == NULL) {//�߂�l��NULL�̏ꍇ�A�w�肳�ꂽ�摜���Ȃ�
                    EndPaint(hWnd, &ps);
                    MessageBox(NULL,
                        _T("�w�肳�ꂽ�摜�����݂��܂���!"),
                        "�摜�֌W�̃G���[",
                        MB_ICONERROR);
                }
                g_bmp = new Gdiplus::Bitmap(is);//GDI+�̉摜�̃I�u�W�F�N�g
                is->Release();//�������J��
                g = new Gdiplus::Graphics(hdc);//GDI�̕`��I�u�W�F�N�g
                g->DrawImage(g_bmp, 500, 0);//�`��
                delete g;
                g = NULL;
            }
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_B_1://�����{�^���������ꂽ
            error.error = 1;
            if (!flag_r) {//�����_�C�A���O�{�b�N�Xor�������I���_�C�A���{�b�N�X���J����ĂȂ���
                flag_r = true;
                if (!(Dialog_r1 = CreateDialog(//�����_�C�A���{�b�N�X�쐬
                    ((HINSTANCE)GetModuleHandle(0)),
                    MAKEINTRESOURCE(IDD_DIALOG1),
                    hWnd,
                    (DLGPROC)Dia_r1Proc
                )))throw error;
                ShowWindow(Dialog_r1, SW_SHOW);
            }
            else {//���ɊJ����Ă���
                MessageBox(NULL,
                    _T("���Ɍ����_�C�A���O���������_�C�A���O���J���Ă��܂��B!"),
                    "���C���E�B���h�E",
                    MB_ICONERROR);
            }
            break;
        case ID_B_2://�o�^�{�^��
            error.error = 2;
            if (!flag_i) {//�o�^�_�C�A���{�b�N�X���J����ĂȂ���
                flag_i = true;
                if (!(Dialog_i = CreateDialog(//�o�^�_�C�A���{�b�N�X�쐬
                    ((HINSTANCE)GetModuleHandle(0)),
                    MAKEINTRESOURCE(IDD_DIALOG2),
                    hWnd,
                    (DLGPROC)Dia_iProc
                ))) throw(error);
                ShowWindow(Dialog_i, SW_SHOW);
            }
            else {//���ɊJ����Ă���
                MessageBox(NULL,
                    _T("���ɓo�^�_�C�A���O���J���Ă��܂��B!"),
                    "���C���E�B���h�E",
                    MB_ICONERROR);
            }
            break;
        case ID_B_3://�ύX�{�^���������ꂽ
            error.error = 10;
            if (!flag_i && kiban.TF) {//�o�^�_�C�A���O�{�b�N�X���J���Ă��邩
                flag_i = true;
                if (MessageBox(NULL,
                    _T("�P��̓��e�����C���E�B���h�E�̂悤�ɕύX���܂����H"),
                    "�m�F���b�Z�[�W",
                    MB_YESNO) != IDYES) {//���������I�����ꂽ
                    flag_i = false;
                    break;
                }
                str = new TCHAR[TCHAR_INFO];
                if (!GetWindowText(EditBox_info, str, sizeof(TCHAR) * TCHAR_INFO))throw error;//�G�f�B�b�g�{�b�N�X�̕�����擾
                //if (!SendMessageA(EditBox_info, WM_GETTEXT, sizeof(TCHAR) * TCHAR_JUNL, (LPARAM)str))throw error;
                if (kiban.run(str, sizeof(TCHAR) * TCHAR_JUNL) == -1) {//���s
                    MessageBox(NULL,
                        _T("�P��̕ύX�Ɏ��s���܂���!"),
                        "���C���E�B���h�E",
                        MB_ICONERROR);
                }
                else {//����
                    MessageBox(NULL,
                        _T("�P��̕ύX�ɐ������܂����B"),
                        "���C���E�B���h�E",
                        MB_ICONERROR);
                }
                delete[] str;
                str = NULL;
            }
            else {
                if (!kiban.TF)//�������ݓ����s�\�ȏ��
                    MessageBox(NULL,
                        _T("���̓f�[�^�̏������������ł��܂���!"),
                        "���C���E�B���h�E",
                        MB_ICONERROR);
                else //���ɓo�^�_�C�A���{�b�N�X���J���Ă���
                    MessageBox(NULL,
                        _T("��ɓo�^���ς܂��Ă�������!"),
                        "���C���E�B���h�E",
                        MB_ICONERROR);
            }
            flag_i = false;
            break;
        }
        break;
    case MY_CREARE_DLG3://�������I���{�b�N�X�̐������b�Z�[�W
        if (!(Dialog_r2 = CreateDialog(//�����_�C�A���{�b�N�X�쐬
            ((HINSTANCE)GetModuleHandle(0)),
            MAKEINTRESOURCE(IDD_DIALOG3),
            hWnd,
            (DLGPROC)Dia_r2Proc
        )))throw error;
        ShowWindow(Dialog_r2, SW_SHOW);
        break;
    case WM_DESTROY://�~   
        if (!DeFlag) {
            DeFlag = true;
        }
        else {
            PostQuitMessage(0);//���b�Z�[�W���[�v�I��
            break;
        }
        if (flag_r && IsWindowVisible(Dialog_r2)) {//�������猟�����I���_�C�A���{�b�N�X���J���Ă����炻�����
            DestroyWindow(Dialog_r2);
        }//�����܂�
        else if (Dialog_r1 != NULL && IsWindowVisible(Dialog_r1)) DestroyWindow(Dialog_r1);//�����_�C�A���O�{�b�N�X���J���Ă����炻�����
        if (flag_i && Dialog_i != NULL && IsWindowVisible(Dialog_i))DestroyWindow(Dialog_i);//�o�^�_�C�A���O�{�b�N�X���J���Ă����炻�����
        g = NULL;
        delete[] str;
        str = NULL;
        delete Kensaku;
        Kensaku = NULL;
        PostQuitMessage(0);//���b�Z�[�W���[�v�I��
        return -1;
        break;
    default://�������Ȃ�
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

BOOL CALLBACK Dia_r1Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND Dialog_r1_ed1;//�G�f�B�b�g�{�b�N�X1�̃n���h��
    static HWND Dialog_r1_ed2;//�G�f�B�b�g�{�b�N�X1�̃n���h��
    HWND Dialog_r2;//�������I���_�C�A���{�b�N�X�𐶐�����Ƃ��Ɏg��
    MyError error = { 2,hWnd };//�G���[�n���h��
    TCHAR* str=NULL;//�������舵���p

    switch (iMsg)
    {
    case WM_INITDIALOG:
        //�_�C�A���O�{�b�N�X���������ꂽ�Ƃ�
        error.error = 7;
        Dialog_r1_ed1 = GetDlgItem(hWnd, IDC_EDIT1);//�G�f�B�b�g�{�b�N�X1�̃n���h���擾
        if (!Dialog_r1_ed1)throw(error);
        Dialog_r1_ed2 = GetDlgItem(hWnd, IDC_EDIT2);//�G�f�B�b�g�{�b�N�X1�̃n���h���擾
        if (!Dialog_r1_ed2)throw(error);
        Kensaku = new KIBAN_KENSAKU;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK://�����{�^���������ꂽ
            if (flag_i) {//�o�^�_�C�A���{�b�N�X���J���Ă���
                MessageBox(NULL,
                    _T("��ɒP��̓o�^�𐟂܂��Ă�������"),
                    "���肢",
                    MB_ICONERROR);
                return TRUE;
            }
            Kensaku->set();//���Z�b�g
            str = new TCHAR[TCHAR_WORD];
            if (GetWindowText(Dialog_r1_ed1, str, sizeof(TCHAR) * TCHAR_WORD) != 0) {//�P�ꖼ�����͂���Ă��邩
                Kensaku->set(str, sizeof(TCHAR) * TCHAR_WORD);//�P�ꖼ�̃Z�b�g
                delete[] str;
                str = NULL;
                str = NULL;
                str = new TCHAR[TCHAR_JUNL];
                if (GetWindowText(Dialog_r1_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) != 0)//���ޖ������͂���Ă��邩
                    Kensaku->set(str, sizeof(TCHAR) * TCHAR_JUNL);//���ޖ��̃Z�b�g
                delete[] str;
                str = NULL;
                if (Kensaku->run() == -1) {//�P�ꂪ�q�b�g���Ȃ�����
                    MessageBox(NULL,
                        _T("�P�ꂪ���݂��܂���ł����I"),
                        "���s",
                        MB_ICONERROR);
                    delete Kensaku;////�������玩�g����
                    Kensaku = NULL;
                    flag_r = false;
                    DestroyWindow(hWnd);////�����܂�
                    return TRUE;
                }
                if (Kensaku->GetStats() == 1) {//�P�ꂪ������q�b�g
                    
                    kiban = (*Kensaku)[0];//�󂯓n��
                    MessageBox(NULL,
                        _T("�P��̌����ɐ������܂����B"),
                        "����",
                        MB_ICONERROR);
                    delete Kensaku;////�������玩�g����
                    Kensaku = NULL;
                    flag_r = false;
                    InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//�ĕ`��
                    DestroyWindow(hWnd);////�����܂�
                    return TRUE;
                }//��������͌������I���_�C�A���O�{�b�N�X�̐����E���p��
                DestroyWindow(hWnd);//���g�����̈�s�ŉ�
                SendMessageA(FindWindow(Aplecat_Name, NULL), MY_CREARE_DLG3,NULL,NULL);//�������I���{�b�N�X�̐������b�Z�[�W�i���C���E�B���h�E�ɂāj
                return true;
            }
            else {//�P�ꖼ��������
                MessageBox(NULL,
                    _T("�P�����͂��Ă��������I"),
                    "����",
                    MB_ICONERROR);
                return TRUE;
            }
        case IDCANCEL://�L�����Z���{�^���������ꂽ
            //���[�h���X�_�C�A���O�{�b�N�X��j��  
            delete Kensaku;////�����Ŏ��g����
            Kensaku = NULL;
            flag_r = false;
            DestroyWindow(hWnd);////�����܂�
            break;
            return TRUE;
        }
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK Dia_r2Proc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    static HWND StaticBox = NULL;//�X�^�e�B�b�N�{�b�N�X�̃n���h��
    static HWND RSwitch[5] = { NULL,NULL,NULL,NULL,NULL };//�`�F�b�N�{�^���̃n���h���̔z��
    MyError error = { 0,hWnd };//�G���[�n���h��
    std::string sstr;//���l�����ޕ�����̍쐬�p
    CString str,word;//������쐬�p
    int CPoint=0;//�`�F�b�N�{�b�N�X�̃`�F�b�N�ʒu���Ɏg����
    static int now;//���\������Ă�����\�����߂̕ϐ��B
    switch (iMsg) {
    case WM_INITDIALOG://��������
        now = 0;
        error.error = 9;
        StaticBox = CreateWindow(
            "static",                           // �X�^�e�B�b�N�R���g���[���̃N���X��
            "text is here.",                    // �\�����镶����
            SS_SIMPLE | WS_CHILD | WS_VISIBLE,  // �ڍׂ͂��ƂŁB
            5, 5,                               // ����̍��W
            500, 100,                           // ���ƍ���
            hWnd,     // �e�E�C���h�E�̃E�C���h�E�n���h��
            NULL,                               // ���j���[�n���h���BNULL�ł悢�B
            ((HINSTANCE)GetModuleHandle(0)), // �A�v���P�[�V�����̃C���X�^���X�n���h���B
            NULL                                // �E�C���h�E�쐬�f�[�^�BNULL�ł悢
        );
        if (!StaticBox)throw error;
        RSwitch[0]= GetDlgItem(hWnd, IDC_CHECK1);//�`�F�b�N�{�b�N�X�̃n���h���擾
        RSwitch[1] = GetDlgItem(hWnd, IDC_CHECK2);//�`�F�b�N�{�b�N�X�̃n���h���擾
        RSwitch[2] = GetDlgItem(hWnd, IDC_CHECK3);//�`�F�b�N�{�b�N�X�̃n���h���擾
        RSwitch[3] = GetDlgItem(hWnd, IDC_CHECK4);//�`�F�b�N�{�b�N�X�̃n���h���擾
        RSwitch[4] = GetDlgItem(hWnd, IDC_CHECK5);//�`�F�b�N�{�b�N�X�̃n���h���擾
        for (int i = 0; i < 5; i++) if (!RSwitch[i]) throw error;//�G���[����
        str = _T("���̒����猟���������P���I�����Ă��������B ");////��������X�^�e�B�b�N�{�b�N�X�̕�����쐬
        sstr = "";//////�������琔�l�����ޕ�����̍쐬
        sstr = std::to_string(Kensaku->GetStats());
        sstr += "����  1�`";
        sstr += std::to_string(Kensaku->GetStats()>=5?5: Kensaku->GetStats());
        sstr += "����";//////�����܂�
        str += sstr.c_str();////�����܂�
        if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//�X�^�e�B�b�N�{�b�N�X�ɕ������\��
        for (int i = 1; i <= 5 && i <= Kensaku->GetStats(); i++) {//�`�F�b�N�{�b�N�X�ɍŏ��̂��\��
            str = _T("�P�ꖼ::");////������̍쐬
            str += Kensaku->word;
            str += _T("�@���ޖ�::");
            str += (*Kensaku)[i].junl;//�����܂�
            if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//�`�F�b�N�{�b�N�X�̕\���̃Z�b�g
        }
        return TRUE;
    case WM_COMMAND:
        error.error = 11;
        switch (LOWORD(wParam))
        {
        case ID_NEXT://���փ{�^���������ꂽ
            if (Kensaku->GetStats() <= (now + 1) * 5) {//����ȏ��₪�Ȃ�
                MessageBox(hWnd,
                    _T("����ȏ�̒P��̓q�b�g���Ă��܂���I"),
                    "����",
                    MB_ICONERROR);
                return TRUE;
            }
            for (int i = 1; i <= 5; i++) {//�`�F�b�N�{�b�N�X�ɔ��f
                CPoint = i + 5*(now + 1);//���ڂ̌�����₩
                if (CPoint > Kensaku->GetStats()) {//������␔�𒴂��Ă�
                    if (!SetWindowTextA(RSwitch[i - 1], _T("")))throw error;//�`�F�b�N�{�b�N�X�ɕ������\��
                }
                else {
                    str = _T("�P�ꖼ::");////�������當����쐬
                    str += Kensaku->word;
                    str += _T("�@���ޖ�::");
                    str += (*Kensaku)[CPoint].junl;////�����܂�
                    if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//�`�F�b�N�{�b�N�X�ɕ������\��
                }
            }
            ++now;
            str = _T("���̒����猟���������P���I�����Ă��������B  ");////������̍쐬
            sstr = "";//////�������琔�l�����ޕ�����̍쐬
            sstr += std::to_string(Kensaku->GetStats());
            sstr += std::to_string((now)*5+1);
            sstr += "�����`";
            sstr += std::to_string(Kensaku->GetStats() >= 5 ? (now + 1) * 5 : Kensaku->GetStats());
            sstr += "����";//////�����܂�
            str += _T(sstr.c_str());////�����܂�
            if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//�X�^�e�B�b�N�{�b�N�X�ɕ������\��
            return TRUE;
        case ID_BACK://�߂�{�^���������ꂽ
            if (now<=0) {//����ȏ�߂�Ȃ�
                MessageBox(hWnd,
                    _T("���̏�Ԃňꌏ�ڂ���\������Ă��܂��I"),
                    "����",
                    MB_ICONERROR);
                return TRUE;
            }
            for (int i = 1; i <= 5; i++) {//�`�F�b�N�{�b�N�X�ɔ��f
                CPoint = i + 5*(now-1);//���ڂ̌�����₩
                str = _T("�P�ꖼ::");////�������當����쐬
                str += Kensaku->word;
                str += _T("�@���ޖ�::");
                str += (*Kensaku)[CPoint].junl;////�����܂�
                if (!SetWindowText(RSwitch[i - 1], str.GetBuffer()))throw error;//�`�F�b�N�{�b�N�X�ɕ������\��
            }
            --now;
            str = _T("���̒����猟���������P���I�����Ă��������B  ");////������̍쐬
            sstr = "";//////�������琔�l�����ޕ�����̍쐬
            sstr += std::to_string(Kensaku->GetStats());
            sstr += "����";
            sstr += std::to_string((now) * 5 + 1);
            sstr += "�`";
            sstr += std::to_string(Kensaku->GetStats() >= 5 ? (now + 1) * 5 : Kensaku->GetStats());
            sstr += "����";//////�����܂�
            str += _T(sstr.c_str());////�����܂�
            if (!SetWindowText(StaticBox, str.GetBuffer()))throw error;//�X�^�e�B�b�N�{�b�N�X�ɕ������\��
            return TRUE;
        case IDOK://����{�^���������ꂽ
            if (flag_i) {//�o�^�_�C�A���{�b�N�X���J���Ă���
                MessageBox(hWnd,
                    _T("��ɒP��̓o�^���ς܂��Ă��������B"),
                    "�ē�",
                    MB_ICONERROR);
                return TRUE;
            }
            CPoint = -100;//�����l�i����p�j
            for (int i = 0; i < 5; i++) {
                if (BST_CHECKED == SendMessage(RSwitch[i], BM_GETCHECK, 0, 0)) {//�`�F�b�N�����Ă���
                    if (CPoint == -100)CPoint = i + 1;//1��
                    else CPoint = -1;//���
                }
            }
            if (CPoint < 0) {//�u��݂̂Ƀ`�F�b�N������v�Ƃ��������𖞂����Ă��Ȃ�
                MessageBox(hWnd,
                    _T("�`�F�b�N�{�b�N�X�̃`�F�b�N�͕K����Ƀ`�F�b�N���Ă��������I"),
                    "����",
                    MB_ICONERROR);
                return TRUE;
            }
            if ((Kensaku->GetStats() - now * 5) % 5 < CPoint) {
                MessageBox(hWnd,
                    _T("���̃`�F�b�N�͑Ή����Ă��܂���I"),
                    "����",
                    MB_ICONERROR);
                return TRUE;
            }
            kiban = (*Kensaku)[now * 5 + CPoint];//�Y���̒P�����kiban�ɃZ�b�g
            delete Kensaku;////�������玩�g����
            Kensaku = NULL;
            flag_r = false;
            InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//�ĕ`��
            DestroyWindow(hWnd);////�����܂�
            return TRUE;
        case IDCANCEL:
            delete Kensaku;////�������玩�g����
            Kensaku = NULL;
            flag_r = false;
            DestroyWindow(hWnd);////�����܂�
        }
    default:
        return FALSE;
    }
}

BOOL CALLBACK Dia_iProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND Dialog_i_ed1;//�G�f�B�b�g�{�b�N�X1�̃n���h��
    static HWND Dialog_i_ed2;//�G�f�B�b�g�{�b�N�X2�̃n���h��
    static HWND Dialog_i_ed3;//�G�f�B�b�g�{�b�N�X3�̃n���h��
    static HWND Dialog_i_ed4;//�G�f�B�b�g�{�b�N�X4�̃n���h��
    MyError error = { 3,hWnd };//�G���[�n���h��
    TCHAR* str=NULL;//�������舵���p
    switch (iMsg)
    {
    case WM_INITDIALOG:
        //�_�C�A���O�{�b�N�X���������ꂽ�Ƃ�
        error.error = 8;
        Dialog_i_ed1 = GetDlgItem(hWnd, IDC_EDIT1);//�G�f�B�b�g�{�b�N�X1�̃n���h���擾
        if (!Dialog_i_ed1)throw(error);
        Dialog_i_ed2 = GetDlgItem(hWnd, IDC_EDIT2);//�G�f�B�b�g�{�b�N�X2�̃n���h���擾
        if (!Dialog_i_ed2)throw(error);
        Dialog_i_ed3 = GetDlgItem(hWnd, IDC_EDIT3);//�G�f�B�b�g�{�b�N�X3�̃n���h���擾
        if (!Dialog_i_ed3)throw(error);
        Dialog_i_ed4 = GetDlgItem(hWnd, IDC_EDIT4);//�G�f�B�b�g�{�b�N�X4�̃n���h���擾
        if (!Dialog_i_ed4)throw(error);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK://�o�^�{�^���������ꂽ
            kiban.set();//���Z�b�g
            str = new TCHAR[TCHAR_JUNL];
            if (GetWindowText(Dialog_i_ed2, str, sizeof(TCHAR) * TCHAR_JUNL) != 0) {//�P�ꖼ�����͂���Ă���
                kiban.set(str, sizeof(TCHAR) * TCHAR_JUNL);
                delete[] str;
                str = NULL;
                str = new TCHAR[TCHAR_WORD];
                if (GetWindowText(Dialog_i_ed1, str, sizeof(TCHAR) * TCHAR_WORD) != 0) {//���ޖ������͂���Ă���
                    kiban.set(str, sizeof(TCHAR) * TCHAR_WORD);
                    delete[] str;
                    str = NULL;
                    str = new TCHAR[TCHAR_INFO];
                    if (GetWindowText(Dialog_i_ed4, str, sizeof(TCHAR) * TCHAR_INFO) != 0) {//���e�����͂���Ă���
                        kiban.set(str, sizeof(TCHAR) * TCHAR_INFO);
                        delete[] str;
                        str = NULL;
                        str = new TCHAR[TCHAR_FILE];
                        if (GetWindowText(Dialog_i_ed3, str, sizeof(TCHAR) * TCHAR_FILE) != 0)//�摜�p�X�����͂���Ă���
                            kiban.set(str, sizeof(TCHAR) * TCHAR_FILE);
                        delete[] str;
                        str = NULL;
                        if (kiban.run() == -1) {//���s
                            MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                                _T("�P��̓o�^�Ɏ��s���܂���"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                                _T("���s�I"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                                NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
                            );
                            flag_i = false;////�������玩�g����
                            DestroyWindow(hWnd);////�����܂�
                            break;
                        }
                        else {//����
                            MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                                _T("�P��̓o�^�ɐ������܂���"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                                _T("�������܂����I"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                                NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
                            );
                            flag_i = false;////�������玩�g����
                            InvalidateRect(FindWindow(Aplecat_Name, NULL), NULL, TRUE);//�ĕ`��
                            DestroyWindow(hWnd);////�����܂�
                            break;
                        }
                    }
                }
            }//��������͕K�{���ڂ����͂���Ă��Ȃ��ꍇ�̏���
            delete[] str;
            str = NULL;
            MessageBox(hWnd,// �I�[�i�[�E�B���h�E�̃n���h��
                _T("�P��E���ށE���e�͕K�����͂��Ă��������I\n�K�{���ڂł��B"),// ���b�Z�[�W�{�b�N�X���̃e�L�X�g
                _T("����"),// ���b�Z�[�W�{�b�N�X�̃^�C�g��
                NULL// ���b�Z�[�W�{�b�N�X�̃X�^�C��
            );
            break;
            return TRUE;
        case IDCANCEL://�L�����Z���{�^���������ꂽ
            //���[�h���X�_�C�A���O�{�b�N�X��j��
            flag_i = false;////�������玩�g����
            DestroyWindow(hWnd);////�����܂�
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

    HANDLE hFile;//�t�@�C���n���h��
    HGLOBAL hBuf;//�m�ۂ��ꂽ�������̃n���h��
    LPVOID lpBuf;//���b�N���ꂽ�������̈�̃n���h��
    IStream* isFile;//�摜�����[�h���ꂽ�������̃n���h��
    DWORD dwFileSize, dwLoadSize;//�t�@�C���̃T�C�Y

    MyError error = { 4,hWnd };//�G���[�n���h��

    /* �摜�t�@�C���I�[�v�� */
    hFile = CreateFile(lpszPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hFile)return NULL;
    /* �t�@�C���T�C�Y�擾 */
    dwFileSize = GetFileSize(hFile, NULL);
    if (!dwFileSize) {
        CloseHandle(hFile);
        return NULL;
    }
    /* �摜�t�@�C���f�[�^�i�[�p�������u���b�N�m�� */
    hBuf = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
    if (hBuf == NULL) {
        CloseHandle(hFile);
        throw error;
    }
    /* �������u���b�N�����b�N���A�h���X���擾 */
    lpBuf = GlobalLock(hBuf);
    if (lpBuf == NULL) {
        CloseHandle(hFile);
        throw error;
    }
    /* �摜�t�@�C���̃f�[�^���������u���b�N�ɓǂݍ��� */
    if (!ReadFile(hFile, lpBuf, GetFileSize(hFile, NULL), &dwLoadSize, NULL)) {
        CloseHandle(hFile);
        GlobalUnlock(hBuf);
        throw error;
    }

    CloseHandle(hFile);

    /* �������u���b�N�̃��b�N���� */
    GlobalUnlock(hBuf);

    /* �������u���b�N����IStream���쐬 */
    CreateStreamOnHGlobal(hBuf, TRUE, &isFile);

    /* IStream��Ԃ� */
    return isFile;
}
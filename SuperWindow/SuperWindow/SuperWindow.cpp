#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <strsafe.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc_About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc_OpenText(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK DlgProc_OpenImage(HWND, UINT, WPARAM, LPARAM);

BOOL WndProc_OnCreate(HWND, LPCREATESTRUCT);
void WndProc_OnCommand(HWND, int, HWND, UINT);
void WndProc_OnMenuSelect(HWND, HMENU, int, HMENU, UINT);
void WndProc_OnContextMenu(HWND, HWND, UINT, UINT);
void WndProc_OnDestroy(HWND);
void WndProc_OnPaint(HWND);
void DialogProc_OnCommand(HWND, int, HWND, UINT);
BOOL DialogProc_OnInitDialog(HWND, HWND, LPARAM);
void DialogProcOpenText_OnCommand(HWND, int, HWND, UINT);
BOOL DialogProcOpenText_OnInitDialog(HWND, HWND, LPARAM);
void DialogProcOpenImage_OnCommand(HWND, int, HWND, UINT);

void MessageBoxFunc(LPCTSTR);

LPTSTR g_lpszClassName = TEXT("SuperWindow");
LPTSTR g_lpszApplicationTitle = TEXT("Developer: Dyagel Andrew");

HWND g_hEdit;
HWND g_hListBox;
HWND g_hwndDlg;
HMENU g_hMenuPopup;

int APIENTRY _tWinMain(HINSTANCE This,
                       HINSTANCE hPrevInstance,
                       LPTSTR lpszCmdLine,
                       int mode)
{
    HWND hWnd;
    HWND hDlgMode = NULL;
    MSG msg;
    WNDCLASSEX wc;

    memset(&wc, 0, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = This;
    wc.lpszClassName = g_lpszClassName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(This, MAKEINTRESOURCE(IDI_ICON_LOGO));
    wc.hCursor = LoadCursor(This, MAKEINTRESOURCE(IDC_CURSOR_CUSTOM));
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_CUSTOM);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = CreateSolidBrush(COLORREF RGB(127, 127, 0));
    wc.hIconSm = NULL;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Error register window class!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindowEx(NULL,
                          g_lpszClassName,
                          g_lpszApplicationTitle,
                          WS_OVERLAPPEDWINDOW | WS_CAPTION,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          HWND_DESKTOP,
                          NULL,
                          This,
                          NULL);
    if(!hWnd)
    {
        MessageBox(NULL, TEXT("Can't create window!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    ShowWindow(hWnd, mode);
    UpdateWindow(hWnd);

    HACCEL hAccel;
    hAccel = LoadAccelerators(This, MAKEINTRESOURCE(IDR_ACCELERATOR1));

    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(hDlgMode == 0 || !IsDialogMessage(hDlgMode, &msg))
        {
            if(!TranslateAccelerator(hWnd, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    switch(msg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, WndProc_OnCreate);
        HANDLE_MSG(hwnd, WM_COMMAND, WndProc_OnCommand);
        HANDLE_MSG(hwnd, WM_MENUSELECT, WndProc_OnMenuSelect);
        HANDLE_MSG(hwnd, WM_CONTEXTMENU, WndProc_OnContextMenu);
        HANDLE_MSG(hwnd, WM_DESTROY, WndProc_OnDestroy);
        HANDLE_MSG(hwnd, WM_PAINT, WndProc_OnPaint);
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK DlgProc_About(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hwndDlg, WM_INITDIALOG, DialogProc_OnInitDialog);
        HANDLE_MSG(hwndDlg, WM_COMMAND, DialogProc_OnCommand);
    }
    return FALSE;
}

INT_PTR CALLBACK DlgProc_OpenText(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hwndDlg, WM_INITDIALOG, DialogProcOpenText_OnInitDialog);
        HANDLE_MSG(hwndDlg, WM_COMMAND, DialogProcOpenText_OnCommand);
    }
    return FALSE;
}

INT_PTR CALLBACK DlgProc_OpenImage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hwndDlg, WM_COMMAND, DialogProcOpenImage_OnCommand);
    }
    return FALSE;
}

void MessageBoxFunc(LPCTSTR lpszMenuCommand)
{
    LPCTSTR lpszContentMessageBox;
    LPCTSTR lpszTitleMessageBox = TEXT("Menu items");
    TCHAR buffer[150] = TEXT("Have been chosen menu item ");
    StringCchCat(buffer, 150, lpszMenuCommand);
    lpszContentMessageBox = buffer;
    MessageBox(NULL, lpszContentMessageBox,
               lpszTitleMessageBox, MB_OK);
}

BOOL WndProc_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    HWND hButtonExit;
    HWND hStatic;
    HWND hButtonSave;
    HWND hButtonAdd;

    if(!(hStatic = CreateWindowEx(NULL, TEXT("Static"), TEXT("Edited field"), WS_CHILD | WS_VISIBLE | SS_SIMPLE,
                                  20, 10, 150, 20, hwnd, (HMENU)(IDC_STATIC1),
                                  NULL, NULL)))
    {
        return FALSE;
    }
    if(!(g_hEdit = CreateWindowEx(NULL, TEXT("Edit"), TEXT("Editor"),
                                  WS_CHILD | WS_BORDER | WS_VISIBLE,
                                  20, 50, 150, 40, hwnd, (HMENU)(IDC_EDIT1),
                                  NULL, NULL)))
    {
        return FALSE;
    }
    if(!(g_hListBox = CreateWindowEx(NULL, TEXT("ListBox"), TEXT("List"),
                                     WS_CHILD | WS_BORDER | WS_VISIBLE,
                                     200, 50, 160, 180, hwnd, (HMENU)(IDC_LISTBOX),
                                     NULL, NULL)))
    {
        return FALSE;
    }
    if(!(hButtonSave = CreateWindowEx(NULL, TEXT("Button"), TEXT("To buffer"),
                                      WS_CHILD | WS_BORDER | WS_VISIBLE,
                                      20, 240, 80, 24, hwnd, (HMENU)(IDC_BTN_SAVE),
                                      NULL, NULL)))
    {
        return FALSE;
    }
    if(!(hButtonAdd = CreateWindowEx(NULL, TEXT("Button"), TEXT("To List"),
                                     WS_CHILD | WS_BORDER | WS_VISIBLE,
                                     120, 240, 80, 24, hwnd, (HMENU)(IDC_BTN_ADD),
                                     NULL, NULL)))
    {
        return FALSE;
    }
    if(!(hButtonExit = CreateWindowEx(NULL, TEXT("Button"), TEXT("Exit"),
                                      WS_CHILD | WS_BORDER | WS_VISIBLE,
                                      220, 240, 80, 24, hwnd, (HMENU)(IDCANCEL),
                                      NULL, NULL)))
    {
        return FALSE;
    }

    if(!(AppendMenu(GetSubMenu(GetMenu(hwnd), 0), MF_SEPARATOR,
                    NULL, NULL)))
    {
        MessageBox(NULL, TEXT("Can't append separator!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if(!(AppendMenu(GetSubMenu(GetMenu(hwnd), 0), MF_ENABLED | MF_STRING,
                    ID_FILE_CLOSEDOCUMENT, TEXT("Close document"))))
    {
        MessageBox(NULL, TEXT("Can't append menu item!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if(!(g_hMenuPopup = CreatePopupMenu()))
    {
        MessageBox(NULL, TEXT("Can't create popup menu!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if(!(AppendMenu(g_hMenuPopup, MF_DISABLED | MF_STRING,
                    ID_EDIT_SELECT_POPUP, TEXT("Select"))))
    {
        MessageBox(NULL, TEXT("Can't append popup menu item!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }

    if(!(AppendMenu(g_hMenuPopup, MF_DISABLED | MF_STRING,
                    ID_EDIT_COPY_POPUP, TEXT("Copy"))))
    {
        MessageBox(NULL, TEXT("Can't append popup menu item!"),
                   TEXT("Error"), MB_OK | MB_ICONERROR);
        return FALSE;
    }
    FORWARD_WM_CREATE(hwnd, lpCreateStruct, DefWindowProc);
    return TRUE;
}

void WndProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    LPCTSTR lpszMenuCommand;
    static TCHAR pszTextBuffer[500];
    HDC hDC;
    HDC hCompDC;
    HANDLE hBmp;
    HANDLE hOldBmp;
    BITMAP bmp;
    TCHAR lpszFileSpec[260] = TEXT("\\penguin.bmp");
    RECT rect;

    if(codeNotify == 1)
    {
        switch(id)
        {
            case ID_ACCELERATOR_HELP:
                SendMessage(hwnd, WM_COMMAND, ID_HELP_HELPDESK, 0);
                break;
            case ID_ACCELERATOR_EXIT:
                SendMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
                break;
        }
    }
    else
    {
        switch(id)
        {
            case IDC_BTN_SAVE:
            {
                int cch;
                cch = SendMessage(g_hEdit, WM_GETTEXT, 500, (LPARAM)pszTextBuffer);
                if(cch == 0)
                {
                    MessageBox(hwnd, TEXT("Enter text"), TEXT("Reading \"Edit\""),
                               MB_OK);
                }
                else
                {
                    TCHAR bufferEdit[500] = { 0 };

                    SYSTEMTIME st;
                    GetSystemTime(&st);
                    StringCchPrintf(bufferEdit, 500, TEXT("Time: %d h, %d min, %d sec\n"),
                                    st.wHour, st.wMinute, st.wSecond);
                    StringCchCat(bufferEdit, 500, TEXT("Text in memory: "));
                    StringCchCat(bufferEdit, 500, pszTextBuffer);
                    MessageBox(hwnd, bufferEdit, TEXT("Content of buffer"), MB_OK);
                }
            }
            break;
            case IDC_BTN_ADD:
            {
                int ind;
                ind = SendMessage(g_hListBox, LB_ADDSTRING, 0, (LPARAM)pszTextBuffer);
                if(ind == LB_ERR)
                {
                    MessageBox(hwnd, TEXT("Error!"), TEXT(""), MB_OK);
                }
            }
            break;
            case ID_FILE_CREATE:
                EnableMenuItem(GetMenu(hwnd), ID_EDIT_SELECT, MF_ENABLED);
                EnableMenuItem(g_hMenuPopup, ID_EDIT_SELECT_POPUP, MF_ENABLED);
                lpszMenuCommand = TEXT("\"Create\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_FILE_OPEN:
                lpszMenuCommand = TEXT("\"Open\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_FILE_CLOSEDOCUMENT:
                EnableMenuItem(GetMenu(hwnd), ID_EDIT_SELECT, MF_DISABLED);
                EnableMenuItem(GetMenu(hwnd), ID_EDIT_COPY, MF_DISABLED);
                EnableMenuItem(g_hMenuPopup, ID_EDIT_SELECT_POPUP, MF_DISABLED);
                EnableMenuItem(g_hMenuPopup, ID_EDIT_COPY_POPUP, MF_DISABLED);
                break;
            case ID_FILE_EXIT:
                lpszMenuCommand = TEXT("\"Exit\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_EDIT_SELECT:
                EnableMenuItem(GetMenu(hwnd), ID_EDIT_COPY, MF_ENABLED);
                EnableMenuItem(g_hMenuPopup, ID_EDIT_COPY_POPUP, MF_ENABLED);
                lpszMenuCommand = TEXT("\"Select\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_EDIT_CUT:
                lpszMenuCommand = TEXT("\"Cut\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_EDIT_COPY:
                lpszMenuCommand = TEXT("\"Copy\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_EDIT_PASTE:
                lpszMenuCommand = TEXT("\"Paste\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_EDIT_TEXT:
                DialogBox((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG2),
                          hwnd, (DLGPROC)DlgProc_OpenText);
                break;
            case ID_EDIT_GRAPHIC:
                g_hwndDlg = CreateDialog((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG3),
                             hwnd, (DLGPROC)DlgProc_OpenImage);
                hBmp = LoadImage(NULL, lpszFileSpec, IMAGE_BITMAP, 0, 0,
                                 LR_LOADFROMFILE);
                hDC = GetDC(g_hwndDlg);
                hCompDC = CreateCompatibleDC(hDC);
                GetObject(hBmp, sizeof(BITMAP), &bmp);
                hOldBmp = SelectObject(hCompDC, hBmp);
                SetRect(&rect, 0, 0, 250, 250);
                StretchBlt(hDC, 40, 20, rect.right, rect.bottom, hCompDC,
                           0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
                SelectObject(hCompDC, hOldBmp);

                DeleteDC(hCompDC);
                DeleteObject(hBmp);
                ReleaseDC(g_hwndDlg, hDC);
                break;
            case ID_HELP_HELPDESK:
                lpszMenuCommand = TEXT("\"Help desk\"");
                MessageBoxFunc(lpszMenuCommand);
                break;
            case ID_HELP_ABOUT:
                DialogBox((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG1),
                          hwnd, (DLGPROC)DlgProc_About);
                break;
            case IDCANCEL:
                DestroyWindow(hwnd);
                break;
            case ID_EDIT_SELECT_POPUP:
                if((SendMessage(hwnd, WM_COMMAND, ID_EDIT_SELECT, 0)) == LB_ERR)
                {
                    MessageBox(hwnd, TEXT("Error!"), TEXT(""), MB_OK);
                }
                break;
            case ID_EDIT_COPY_POPUP:
                if((SendMessage(hwnd, WM_COMMAND, ID_EDIT_COPY, 0)) == LB_ERR)
                {
                    MessageBox(hwnd, TEXT("Error!"), TEXT(""), MB_OK);
                }
                break;
        }
    }
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}

void WndProc_OnMenuSelect(HWND hwnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
    HDC hdc;

    RECT rect;

    LPTSTR lpszMsgSpace;
    lpszMsgSpace = TEXT("                                                      ");
    TCHAR buffer[150];
    size_t cCh;

    LoadString((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        (UINT)item, buffer, 150);
    hdc = GetDC(hwnd);
    GetClientRect(hwnd, &rect);
    StringCchLength(lpszMsgSpace, 150, &cCh);
    TextOut(hdc, rect.left + 10, rect.bottom - 30,
            lpszMsgSpace, cCh);
    StringCchLength(buffer, 150, &cCh);
    TextOut(hdc, rect.left + 10, rect.bottom - 30,
            buffer, cCh);
    ReleaseDC(hwnd, hdc);
    FORWARD_WM_MENUSELECT(hwnd, hmenu, item, hmenuPopup, flags, DefWindowProc);
}

void WndProc_OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
    TrackPopupMenuEx(g_hMenuPopup, TPM_LEFTALIGN | TPM_TOPALIGN |
                     TPM_LEFTBUTTON, xPos, yPos,
                     hwnd, NULL);
    FORWARD_WM_CONTEXTMENU(hwnd, hwndContext, xPos, yPos, DefWindowProc);
}

void WndProc_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
    FORWARD_WM_DESTROY(hwnd, DefWindowProc);
}

void WndProc_OnPaint(HWND hwnd)
{
    LPCTSTR lpcszMessagePaint = TEXT("Printing text when prossesing message WM_PAINT.");
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);
    int x = 20;
    int y = 300;
    TextOut(hdc, x, y, lpcszMessagePaint, _tcsclen(lpcszMessagePaint));
    EndPaint(hwnd, &ps);
    FORWARD_WM_PAINT(hwnd, DefWindowProc);
}

void DialogProc_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case IDOK:
            EndDialog(hwnd, IDOK);
    }
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}

BOOL DialogProc_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    LPCTSTR lpszTextAbout = TEXT("Windows Application\nDeveloper: Dyagel A.Y.\nGroup: 60323-1\n");
    TCHAR pszTextBuffer[300] = {0};
    TCHAR pszTimeBuffer[100] = {0};

    SYSTEMTIME st;

    GetLocalTime(&st);

    StringCchPrintf(pszTimeBuffer, 100, TEXT("Date: %02d.%02d.%d\nTime: %02d:%02d:%02d"), st.wDay, st.wMonth, st.wYear,
                    st.wHour, st.wMinute, st.wSecond);
    StringCchCat(pszTextBuffer, 300, lpszTextAbout);
    StringCchCat(pszTextBuffer, 300, pszTimeBuffer);

    SetWindowText(GetDlgItem(hwnd, IDC_STATIC1), pszTextBuffer);

    return TRUE;
    FORWARD_WM_INITDIALOG(hwnd, hwndFocus, lParam, DefWindowProc);
}

void DialogProcOpenText_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HANDLE hFile = NULL;
    TCHAR lpszBuffer[MAX_CHARS] = {0};
    OPENFILENAME ofn;
    BOOL fRet;
    TCHAR lpszFileSpec[260];
    DWORD  nNumberOfBytesToRead = MAX_CHARS * 2;
    LPDWORD lpNumberOfBytesRead = 0;

    switch(id)
    {
        case IDOK:
            EndDialog(hwnd, IDOK);
            break;
        case IDC_BTN_LOAD:
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = lpszFileSpec;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(lpszFileSpec);
            ofn.lpstrFilter = TEXT("All\0*.*\0Text files\0*.TXT\0");
            ofn.nMaxCustFilter = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            fRet = GetOpenFileName(&ofn);
            if(fRet == FALSE)
            {
                MessageBox(hwnd, TEXT("Error"), TEXT("Error! Can't open dialog."),
                           MB_OK);
            }

            hFile = CreateFile(lpszFileSpec, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL, NULL);
            if(hFile == INVALID_HANDLE_VALUE)
            {
                MessageBox(hwnd, TEXT("Error"), TEXT("Error! Can't open file."),
                           MB_OK);
            }

            ReadFile(hFile, (LPVOID)lpszBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL);
            SendDlgItemMessage(hwnd, IDC_EDIT2, WM_SETTEXT, NULL, (LPARAM)lpszBuffer);

            if(hFile)
            {
                CloseHandle(hFile);
            }
            break;

    }
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}

BOOL DialogProcOpenText_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    LPCTSTR lpszText = TEXT("Step 1. Beginning of work");

    SetWindowText(GetDlgItem(hwnd, IDC_EDIT2), lpszText);

    return TRUE;
    FORWARD_WM_INITDIALOG(hwnd, hwndFocus, lParam, DefWindowProc);
}

void DialogProcOpenImage_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case IDOK:
            DestroyWindow(g_hwndDlg);
            break;
    }
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}

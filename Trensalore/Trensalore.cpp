// Trensalore.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Trensalore.h"

#pragma comment(lib,"comctl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Find(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Replace(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Goto(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitCommonControls();
	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TRENSALORE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRENSALORE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRENSALORE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TRENSALORE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Window Creation failed"), TEXT("InitInstance Error!"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hEdit, hStatus;
	RECT rcStatus, rcClient;
	int iStatusHeight, iEditHeight, iStatusWidths[] = { 100, 200, -1 };
	char szFileName[MAX_PATH] = "";
	SYSTEMTIME tmeCurrent;

	switch (message)
	{
	case WM_CREATE:
	{
		// Create an edit box
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
			_T("EDIT"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
			ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			300,
			100,
			hWnd,
			(HMENU)IDC_MAIN_EDIT,
			GetModuleHandle(NULL),
			NULL);
		HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit,
			WM_SETFONT,
			(WPARAM)hfDefault,
			MAKELPARAM(FALSE, 0));
		SendMessage(hEdit,
			WM_SETTEXT,
			NULL,
			(LPARAM)"Insert text here...");
		// Create an statusbar
		hStatus = CreateWindowEx(0,
			STATUSCLASSNAME,
			NULL,
			WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
			0,
			0,
			0,
			0,
			hWnd,
			(HMENU)IDC_MAIN_STATUS,
			GetModuleHandle(NULL),
			NULL);
		if (!hStatus){
			MessageBox(hWnd, "Failed to Create StatusBar", "Statusbar Creation", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		SendMessage(hStatus, SB_SETPARTS, sizeof(iStatusWidths) / sizeof(int), (LPARAM)iStatusWidths);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)"Hi there :)");

		SetWindowText(hWnd, "Untitled - Trensalore");
	}
	break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEW:
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");

			SetWindowText(hWnd, "Untitled - Trensalore");
			break;
		case ID_FILE_OPEN:
			//Open Dialog
			OPENFILENAME openFileDialog;

			ZeroMemory(&openFileDialog, sizeof(openFileDialog));
			openFileDialog.lStructSize = sizeof(openFileDialog);
			openFileDialog.hwndOwner = hWnd;
			openFileDialog.lpstrFilter = "Angelic Text (*.Atxt)\0*.Atxt\0Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
			openFileDialog.lpstrFile = szFileName;
			openFileDialog.nMaxFile = MAX_PATH;
			openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			openFileDialog.lpstrDefExt = "Atxt";

			if (GetOpenFileName(&openFileDialog)){
				MessageBox(0, szFileName, _T("File Name"),
					MB_OK | MB_ICONINFORMATION);

				LoadTextFileToEdit(hEdit, szFileName);
				SetWindowText(hWnd, lstrcat(szFileName, " - Trensalore"));

			}
			break;
		case ID_FILE_SAVEAS:
			//Save Dialog
			OPENFILENAME saveFileDialog;

			ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
			saveFileDialog.lStructSize = sizeof(saveFileDialog);
			saveFileDialog.hwndOwner = hWnd;
			saveFileDialog.lpstrFilter = "Angelic Text (*.Atxt)\0*.Atxt\0Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
			saveFileDialog.lpstrFile = szFileName;
			saveFileDialog.nMaxFile = MAX_PATH;
			saveFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			saveFileDialog.lpstrDefExt = "Atxt";

			if (GetSaveFileName(&saveFileDialog)){
				MessageBox(0, szFileName, _T("File Name"),
					MB_OK | MB_ICONINFORMATION);

				SaveTextFileFromEdit(hEdit, szFileName);
				SetWindowText(hWnd, lstrcat(szFileName, " - Trensalore"));
			}
			break;
		case ID_FILE_SAVE:

			break;
		case ID_FILE_PAGESETUP:
			PAGESETUPDLG psd;    // common dialog box structure
			//HWND hwnd;           // owner window

			// Initialize PAGESETUPDLG
			ZeroMemory(&psd, sizeof(psd));
			psd.lStructSize = sizeof(psd);
			psd.hwndOwner = hWnd;
			psd.hDevMode = NULL; // Don't forget to free or store hDevMode.
			psd.hDevNames = NULL; // Don't forget to free or store hDevNames.
			psd.Flags = PSD_INTHOUSANDTHSOFINCHES | PSD_MARGINS | PSD_ENABLEPAGEPAINTHOOK;
			psd.rtMargin.top = 1000;
			psd.rtMargin.left = 1250;
			psd.rtMargin.right = 1250;
			psd.rtMargin.bottom = 1000;
			psd.lpfnPagePaintHook = PaintHook;

			if (PageSetupDlg(&psd) == TRUE)
			{
				// check paper size and margin values here.
			}

			break;
		case ID_FILE_PRINT:
			PRINTDLG pd;
			//HWND hwnd;

			// Initialize PRINTDLG
			ZeroMemory(&pd, sizeof(pd));
			pd.lStructSize = sizeof(pd);
			pd.hwndOwner = hWnd;
			pd.hDevMode = NULL;     // Don't forget to free or store hDevMode.
			pd.hDevNames = NULL;     // Don't forget to free or store hDevNames.
			pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
			pd.nCopies = 1;
			pd.nFromPage = 0xFFFF;
			pd.nToPage = 0xFFFF;
			pd.nMinPage = 1;
			pd.nMaxPage = 0xFFFF;

			if (PrintDlg(&pd) == TRUE)
			{
				// GDI calls to render output. 

				// Delete DC when done.
				DeleteDC(pd.hDC);
			}

			break;
		case ID_EDIT_UNDO:
			if (SendMessage(hEdit, EM_CANUNDO, 0, 0))
				SendMessage(hEdit, EM_UNDO, 0, 0);
			break;
		case ID_EDIT_CUT:
			SendMessage(hEdit, WM_CUT, 0, 0);
			break;
		case ID_EDIT_COPY:
			SendMessage(hEdit, WM_COPY, 0, 0);
			break;
		case ID_EDIT_PASTE:
			SendMessage(hEdit, WM_PASTE, 0, 0);
			break;
		case ID_EDIT_DELETE:
			SendMessage(hEdit, WM_CLEAR, 0, 0);
			break;
		case ID_EDIT_FIND:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), hWnd, Find);
			break;
		case ID_EDIT_FINDNEXT:
			FINDREPLACE fr;       // common dialog box structure
			//HWND hwnd;            // owner window
			CHAR szFindWhat[80];  // buffer receiving string
			HWND hdlg;
			hdlg = NULL;     // handle to Find dialog box

			// Initialize FINDREPLACE
			ZeroMemory(&fr, sizeof(fr));
			fr.lStructSize = sizeof(fr);
			fr.hwndOwner = hWnd;
			fr.lpstrFindWhat = szFindWhat;
			fr.wFindWhatLen = 80;
			fr.Flags = 0;

			hdlg = FindText(&fr);

			break;
		case ID_EDIT_REPLACE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_REPLACE), hWnd, Replace);
			break;
		case ID_EDIT_GOTO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_GOTO), hWnd, Goto);
			break;
		case ID_EDIT_SELECTALL:
			SendMessage(hEdit, EM_SETSEL, 0, -1);
			break;
		case ID_EDIT_TIME:
			TCHAR str[20], str1[20];
			GetTimeFormat(
				MAKELCID(LANG_USER_DEFAULT, SORT_DEFAULT),
				0,
				&tmeCurrent,
				NULL,
				str,
				20);

			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)str);
			break;
		case ID_FORMAT_WORDWRAP:
			break;
		case ID_FORMAT_FONT:
			CHOOSEFONT cf;            // common dialog box structure
			static LOGFONT lf;        // logical font structure
			static DWORD rgbCurrent;  // current text color
			HFONT hfont, hfontPrev;
			DWORD rgbPrev;

			// Initialize CHOOSEFONT
			ZeroMemory(&cf, sizeof(cf));
			cf.lStructSize = sizeof(cf);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;
			cf.rgbColors = rgbCurrent;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

			if (ChooseFont(&cf) == TRUE)
			{
				//hfont = CreateFontIndirect(cf.lpLogFont);
				//hfontPrev = (HFONT)SelectObject(hdc, hfont);
				//rgbCurrent = cf.rgbColors;
				//rgbPrev = SetTextColor(hdc, rgbCurrent);

			}

			break;
		case ID_VIEW_STATUSBAR:
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		// Make the edit control the size of the window's client area. 

		//MoveWindow(hEdit,
		//	0, 0,                  // starting x- and y-coordinates 
		//	LOWORD(lParam),        // width of client area 
		//	HIWORD(lParam)-1,        // height of client area 
		//	TRUE);                 // repaint window 
		SendMessage(hStatus, WM_SIZE, 0, 0);

		GetWindowRect(hStatus, &rcStatus);
		iStatusHeight = rcStatus.bottom - rcStatus.top;

		// Calculate remaining height and size edit

		GetClientRect(hWnd, &rcClient);

		iEditHeight = rcClient.bottom - iStatusHeight;

		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, iEditHeight, SWP_NOZORDER);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for find box.
INT_PTR CALLBACK Find(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for replace box.
INT_PTR CALLBACK Replace(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for goto box.
INT_PTR CALLBACK Goto(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;

			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL)
			{
				DWORD dwRead;

				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0; // Add null terminator
					if (SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE; // It worked!
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;

		dwTextLength = GetWindowTextLength(hEdit);
		// No need to bother if there's no text.
		if (dwTextLength > 0)
		{
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
			if (pszText != NULL)
			{
				if (GetWindowText(hEdit, pszText, dwBufferSize))
				{
					DWORD dwWritten;

					if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

BOOL CALLBACK PaintHook(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPRECT lprc;
	COLORREF crMargRect;
	HDC hdc, hdcOld;

	switch (uMsg)
	{
		// Draw the margin rectangle. 
	case WM_PSD_MARGINRECT:
		hdc = (HDC)wParam;
		lprc = (LPRECT)lParam;

		// Get the system highlight color. 
		crMargRect = GetSysColor(COLOR_HIGHLIGHT);

		// Create a dash-dot pen of the system highlight color and 
		// select it into the DC of the sample page. 
		hdcOld = (HDC)SelectObject(hdc, CreatePen(PS_DASHDOT, .5, crMargRect));

		// Draw the margin rectangle. 
		Rectangle(hdc, lprc->left, lprc->top, lprc->right, lprc->bottom);

		// Restore the previous pen to the DC. 
		SelectObject(hdc, hdcOld);
		return TRUE;

	default:
		return FALSE;
	}
	return TRUE;
}

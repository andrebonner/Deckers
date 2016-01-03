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
	char szFileName[MAX_PATH];

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
			(LPARAM)L"Insert text here...");
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
			MessageBox(hWnd, L"Failed to Create StatusBar", L"Statusbar Creation", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}

		SendMessage(hStatus, SB_SETPARTS, sizeof(iStatusWidths) / sizeof(int), (LPARAM)iStatusWidths);
		SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"Hi there :)");
		
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
			break;
		case ID_FILE_OPEN:
			//Open Dialog
			OPENFILENAME openFileDialog;
			
			ZeroMemory(&openFileDialog, sizeof(openFileDialog));
			openFileDialog.lStructSize = sizeof(openFileDialog);
			openFileDialog.hwndOwner = hWnd;
			openFileDialog.lpstrFilter = L"Angelic Text (*.Atxt)\0*.Dtxt\0Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
			openFileDialog.lpstrFile = (LPWSTR)szFileName;
			openFileDialog.nMaxFile = MAX_PATH;
			openFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			openFileDialog.lpstrDefExt = L"Atxt";

			if (GetOpenFileName(&openFileDialog)){
				MessageBox(0, (LPWSTR)szFileName, _T("File Name"),
					MB_OK | MB_ICONINFORMATION);

			}
			break;
		case ID_FILE_SAVEAS:
			//Save Dialog
			OPENFILENAME saveFileDialog;
			
			ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
			saveFileDialog.lStructSize = sizeof(saveFileDialog);
			saveFileDialog.hwndOwner = hWnd;
			saveFileDialog.lpstrFilter = L"Angelic Text (*.Atxt)\0*.Dtxt\0Text Files (*.txt)\0*txt\0All Files (*.*)\0*.*\0";
			saveFileDialog.lpstrFile = (LPWSTR)szFileName;
			saveFileDialog.nMaxFile = MAX_PATH;
			saveFileDialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			saveFileDialog.lpstrDefExt = L"Atxt";

			if (GetSaveFileName(&saveFileDialog)){
				MessageBox(0, (LPWSTR)szFileName, _T("File Name"),
					MB_OK | MB_ICONINFORMATION);

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
			psd.lpfnPagePaintHook = NULL;

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
			break;
		case ID_EDIT_REPLACE:
			break;
		case ID_EDIT_GOTO:
			break;
		case ID_EDIT_SELECTALL:
			break;
		case ID_EDIT_TIME:
			break;
		case ID_FORMAT_WORDWRAP:
			break;
		case ID_FORMAT_FONT:
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
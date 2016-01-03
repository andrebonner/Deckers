#include <windows.h>
#include <cstdio>
#include <cmath>
#include "Resource.h"

//---------------------------------------------------------------------------
HWND hWnd;
LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//---------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN_DLG),
	          hWnd, reinterpret_cast<DLGPROC>(DlgProc));

	return FALSE;
}
//---------------------------------------------------------------------------
LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// These variables will carry the values in the text boxes
	LPTSTR strPrincipal = new char[20],
		  strInterest = new char[20], strPeriods = new char[20],
		   strInterestEarned = new char[20], strAmountEarned = new char[20];
	// These are handled for the various controls
	HWND hWndPrincipal, hWndInterest, hWndPeriods, hWndCompound,
		 hWndInterestEarned, hWndAmountEarned, hWndCalculate;

	double Principal, AnnualRate, InterestEarned;
	double FutureValue, RatePerPeriod;
	int    NumberOfPeriods, CompoundType;
	double i;
	int n;

	hWndPrincipal      = GetDlgItem(hWndDlg, IDC_PRINCIPAL);
	hWndInterest       = GetDlgItem(hWndDlg, IDC_ANNUAL_RATE);
	hWndPeriods        = GetDlgItem(hWndDlg, IDC_NBR_OF_PERIODS);
	hWndCompound       = GetDlgItem(hWndDlg, IDC_COMPOUND);
	hWndInterestEarned = GetDlgItem(hWndDlg, IDC_INTEREST_EARNED);
	hWndAmountEarned   = GetDlgItem(hWndDlg, IDC_AMOUNT_EARNED);
	hWndCalculate      = GetDlgItem(hWndDlg, IDC_CALCULATE_BTN);

	switch(Msg)
	{
	case WM_INITDIALOG:
		// Identify each control
		SetWindowText(hWndPrincipal, "0.00");
		SetWindowText(hWndInterest, "7.55");
		SetWindowText(hWndPeriods, "0");
		SetWindowText(hWndInterestEarned, "0.00");
		SetWindowText(hWndAmountEarned, "0.00");

		CheckRadioButton(hWndDlg, IDC_COMPOUND, IDC_ANNUALLY, IDC_COMPOUND);
		return TRUE;

	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_CALCULATE_BTN:
			GetWindowText(hWndPrincipal, strPrincipal, 20);
			GetWindowText(hWndInterest,  strInterest,  10);
			GetWindowText(hWndPeriods,   strPeriods,    8);

			Principal = atof(strPrincipal);
			AnnualRate = atof(strInterest) / 100;

			if( IsDlgButtonChecked(hWndDlg, IDC_COMPOUND) == BST_CHECKED )
				CompoundType = 12;
			else if( IsDlgButtonChecked(hWndDlg, IDC_QUATERLY) == BST_CHECKED )
				CompoundType = 4;
			else if( IsDlgButtonChecked(hWndDlg, IDC_SEMIANNUALLY) == BST_CHECKED )
				CompoundType = 2;
			else // if( IsDlgButtonChecked(hWndDlg, IDC_ANNUALLY) == BST_CHECKED )
				CompoundType = 1;

			NumberOfPeriods = atoi(strPeriods);
			i = AnnualRate / CompoundType;
			n = CompoundType * NumberOfPeriods;

			RatePerPeriod = AnnualRate / NumberOfPeriods;
			FutureValue = Principal * pow(1 + i, n);
			InterestEarned = FutureValue - Principal;

			sprintf_s(strInterestEarned, sizeof(double),"$%.2f", InterestEarned);
			sprintf_s(strAmountEarned, sizeof(double), "$%.2f", FutureValue);

			SetWindowText(hWndInterestEarned, strInterestEarned);
			SetWindowText(hWndAmountEarned,   strAmountEarned);
			return TRUE;

		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			return TRUE;
		}
		break;
	}

	return FALSE;
}
//---------------------------------------------------------------------------
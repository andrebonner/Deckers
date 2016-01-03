#include <windows.h>
#include <cstdio>
#include "resource.h"

using namespace std;

HWND hWnd;
HINSTANCE hInst;
LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//---------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONTROLS_DLG),
		hWnd, reinterpret_cast<DLGPROC>(DlgProc));

	hInst = hInstance;

	return 0;
}
//---------------------------------------------------------------------------
LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{

	char strHourlySalary[20], strMonday1[10], strTuesday1[10], strWednesday1[10],
		strThursday1[10], strFriday1[10], strSaturday1[10], strSunday1[10],
		strMonday2[10], strTuesday2[10], strWednesday2[10],
		strThursday2[10], strFriday2[10], strSaturday2[10], strSunday2[10];

	double monday1 = 0.00, tuesday1 = 0.00, wednesday1 = 0.00, thursday1 = 0.00,
		friday1 = 0.00, saturday1 = 0.00, sunday1 = 0.00,
		monday2 = 0.00, tuesday2 = 0.00, wednesday2 = 0.00, thursday2 = 0.00,
		friday2 = 0.00, saturday2 = 0.00, sunday2 = 0.00;

	HWND hWndHourlySalary, hWndMonday1, hWndTuesday1, hWndWednesday1,
		hWndThursday1, hWndFriday1, hWndSaturday1, hWndSunday1,
		hWndMonday2, hWndTuesday2, hWndWednesday2,
		hWndThursday2, hWndFriday2, hWndSaturday2, hWndSunday2,
		hWndRegularHours, hWndOvertimeHours,
		hWndRegularAmount, hWndOvertimeAmount,
		hWndNetPay;

	double totalHoursWeek1, totalHoursWeek2, ovtSalary;
	double regHours1 = 0.00, regHours2 = 0.00, ovtHours1 = 0.00, ovtHours2 = 0.00;
	double regAmount1 = 0.00, regAmount2 = 0.00, ovtAmount1 = 0.00, ovtAmount2 = 0.00;

	char   strRegularHours[20], strOvertimeHours[20];
	double regularHours, overtimeHours;
	char   strRegularAmount[20], strOvertimeAmount[20], strNetPay[20];
	double regularAmount, overtimeAmount, netPay;

	double hourlySalary = 0.00;

	hWndHourlySalary = GetDlgItem(hWndDlg, IDC_HOURLY_SALARY);
	hWndMonday1 = GetDlgItem(hWndDlg, IDC_MONDAY1);
	hWndTuesday1 = GetDlgItem(hWndDlg, IDC_TUESDAY1);
	hWndWednesday1 = GetDlgItem(hWndDlg, IDC_WEDNESDAY1);
	hWndThursday1 = GetDlgItem(hWndDlg, IDC_THURSDAY1);
	hWndFriday1 = GetDlgItem(hWndDlg, IDC_FRIDAY1);
	hWndSaturday1 = GetDlgItem(hWndDlg, IDC_SATURDAY1);
	hWndSunday1 = GetDlgItem(hWndDlg, IDC_SUNDAY1);

	hWndMonday2 = GetDlgItem(hWndDlg, IDC_MONDAY2);
	hWndTuesday2 = GetDlgItem(hWndDlg, IDC_TUESDAY2);
	hWndWednesday2 = GetDlgItem(hWndDlg, IDC_WEDNESDAY2);
	hWndThursday2 = GetDlgItem(hWndDlg, IDC_THURSDAY2);
	hWndFriday2 = GetDlgItem(hWndDlg, IDC_FRIDAY2);
	hWndSaturday2 = GetDlgItem(hWndDlg, IDC_SATURDAY2);
	hWndSunday2 = GetDlgItem(hWndDlg, IDC_SUNDAY2);

	hWndRegularHours = GetDlgItem(hWndDlg, IDC_REGULAR_HOURS);
	hWndOvertimeHours = GetDlgItem(hWndDlg, IDC_OVERTIME_HOURS);
	hWndRegularAmount = GetDlgItem(hWndDlg, IDC_REGULAR_AMOUNT);
	hWndOvertimeAmount = GetDlgItem(hWndDlg, IDC_OVERTIME_AMOUNT);
	hWndNetPay = GetDlgItem(hWndDlg, IDC_NET_PAY);

	switch (Msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hWndHourlySalary, "0.00");

		SetWindowText(hWndMonday1, "0.00");
		SetWindowText(hWndTuesday1, "0.00");
		SetWindowText(hWndWednesday1, "0.00");
		SetWindowText(hWndThursday1, "0.00");
		SetWindowText(hWndFriday1, "0.00");
		SetWindowText(hWndSaturday1, "0.00");
		SetWindowText(hWndSunday1, "0.00");

		SetWindowText(hWndMonday2, "0.00");
		SetWindowText(hWndTuesday2, "0.00");
		SetWindowText(hWndWednesday2, "0.00");
		SetWindowText(hWndThursday2, "0.00");
		SetWindowText(hWndFriday2, "0.00");
		SetWindowText(hWndSaturday2, "0.00");
		SetWindowText(hWndSunday2, "0.00");

		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_PROCESS_BTN:
			// Retrieve the hourly salary
			GetWindowText(hWndHourlySalary, strHourlySalary, 20);
			hourlySalary = atof(strHourlySalary);
			if (hourlySalary > 120){
				MessageBox(hWndDlg, TEXT("Value must be less than 120"), TEXT("Hourly Salary Value"), MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}

			// Get the time for each day
			GetWindowText(hWndMonday1, strMonday1, 10);
			monday1 = atof(strMonday1);
			GetWindowText(hWndTuesday1, strTuesday1, 10);
			tuesday1 = atof(strTuesday1);
			GetWindowText(hWndWednesday1, strWednesday1, 10);
			wednesday1 = atof(strWednesday1);
			GetWindowText(hWndThursday1, strThursday1, 10);
			thursday1 = atof(strThursday1);
			GetWindowText(hWndFriday1, strFriday1, 10);
			friday1 = atof(strFriday1);
			GetWindowText(hWndSaturday1, strSaturday1, 10);
			saturday1 = atof(strSaturday1);
			GetWindowText(hWndSunday1, strSunday1, 10);
			sunday1 = atof(strSunday1);

			GetWindowText(hWndMonday2, strMonday2, 10);
			monday2 = atof(strMonday2);
			GetWindowText(hWndTuesday2, strTuesday2, 10);
			tuesday2 = atof(strTuesday2);
			GetWindowText(hWndWednesday2, strWednesday2, 10);
			wednesday2 = atof(strWednesday2);
			GetWindowText(hWndThursday2, strThursday2, 10);
			thursday2 = atof(strThursday2);
			GetWindowText(hWndFriday2, strFriday2, 10);
			friday2 = atof(strFriday2);
			GetWindowText(hWndSaturday2, strSaturday2, 10);
			saturday2 = atof(strSaturday2);
			GetWindowText(hWndSunday2, strSunday2, 10);
			sunday2 = atof(strSunday2);

			// Calculate the total number of hours for each week
			totalHoursWeek1 = monday1 + tuesday1 + wednesday1 + thursday1 +
				friday1 + saturday1 + sunday1;
			totalHoursWeek2 = monday2 + tuesday2 + wednesday2 + thursday2 +
				friday2 + saturday2 + sunday2;

			// The overtime is paid time and half
			ovtSalary = hourlySalary * 1.5;

			// If the employee worked under 40 hours, there is no overtime
			if (totalHoursWeek1 < 40)
			{
				regHours1 = totalHoursWeek1;
				regAmount1 = hourlySalary * regHours1;
				ovtHours1 = 0.00;
				ovtAmount1 = 0.00;
			} // If the employee worked over 40 hours, calculate the overtime
			else if (totalHoursWeek1 >= 40)
			{
				regHours1 = 40;
				regAmount1 = hourlySalary * 40;
				ovtHours1 = totalHoursWeek1 - 40;
				ovtAmount1 = ovtHours1 * ovtSalary;
			}

			if (totalHoursWeek2 < 40)
			{
				regHours2 = totalHoursWeek2;
				regAmount2 = hourlySalary * regHours2;
				ovtHours2 = 0.00;
				ovtAmount2 = 0.00;
			}
			else if (totalHoursWeek2 >= 40)
			{
				regHours2 = 40;
				regAmount2 = hourlySalary * 40;
				ovtHours2 = totalHoursWeek2 - 40;
				ovtAmount2 = ovtHours2 * ovtSalary;
			}

			regularHours = regHours1 + regHours2;
			overtimeHours = ovtHours1 + ovtHours2;
			regularAmount = regAmount1 + regAmount2;
			overtimeAmount = ovtAmount1 + ovtAmount2;
			netPay = regularAmount + overtimeAmount;

			sprintf_s(strRegularHours, sizeof(double),"%.2f", regularHours);
			sprintf_s(strOvertimeHours, sizeof(double),"%.2f", overtimeHours);
			sprintf_s(strRegularAmount, sizeof(double),"%.2f", regularAmount);
			sprintf_s(strOvertimeAmount, sizeof(double),"%.2f", overtimeAmount);
			sprintf_s(strNetPay, sizeof(double),"%.2f", netPay);

			SetWindowText(hWndRegularHours, strRegularHours);
			SetWindowText(hWndRegularAmount, strRegularAmount);
			SetWindowText(hWndOvertimeHours, strOvertimeHours);
			SetWindowText(hWndOvertimeAmount, strOvertimeAmount);

			SetWindowText(hWndNetPay, strNetPay);

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
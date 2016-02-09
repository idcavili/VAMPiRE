// CustoMatrix.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CustoMatrix.h"
#include <Windows.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define MAX_LOADSTRING 100
#define WM_SOCKET 104

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// the main window class name
HWND hRxPort, hRxLabel, hTxPort, hTxLabel, hEditIn;
sockaddr sockAddrClient;
struct sockaddr_in server, from;
int nMaxClients, nClient;
char *szHistory;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
SOCKET Socket;
char RxPort[5];

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	
	

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CUSTOMATRIX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CUSTOMATRIX));
	

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
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

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUSTOMATRIX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CUSTOMATRIX);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
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
	case WM_CREATE:
	{
		hRxPort = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"EDIT",
			NULL,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			130, 10,
			100, 20,
			hWnd, NULL,
			hInst,
			NULL);

		hRxLabel = CreateWindowEx(NULL,
			L"STATIC",
			L"OSC Port:",
			WS_CHILD | WS_VISIBLE,
			0, 10,
			130, 20,
			hWnd, NULL,
			hInst,
			NULL);
		/*hTxPort = CreateWindowEx(WS_EX_CLIENTEDGE,
			L"EDIT",
			NULL,
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			130, 40,
			100, 20,
			hWnd, NULL,
			hInst,
			NULL);*/
			/*hTxLabel = CreateWindowEx(NULL,
				L"STATIC",
				L"OSC Transmit Port:",
				WS_CHILD | WS_VISIBLE,
				0, 40,
				130, 20,
				hWnd, NULL,
				hInst,
				NULL);*/
		WSADATA data;
		int wsastatus = WSAStartup(MAKEWORD(2, 2), &data);
		if (wsastatus != 0)
		{
			MessageBox(hWnd,
				L"Winsock Initialization Failed",
				L"Critical Error",
				MB_ICONERROR);
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}
		Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (Socket == INVALID_SOCKET)
		{
			MessageBox(hWnd,
				L"Socket creation failed",
				L"Critical Error",
				MB_ICONERROR);
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			break;
		}
		SendMessage(hRxPort,
			WM_GETTEXT,
			sizeof(RxPort),
			reinterpret_cast<LPARAM>(RxPort));
		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = (USHORT)RxPort;
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SOCKET:
	{
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			for (int n = 0; n <= nMaxClients; n++)
			{
				char szIncoming[1024];
				ZeroMemory(szIncoming, sizeof(szIncoming));

				int inDataLength = recv(Socket[&n],
					(char*)szIncoming,
					sizeof(szIncoming) / sizeof(szIncoming[0]),
					0);

				if (inDataLength != -1)
				{
					strncat(szHistory, szIncoming, inDataLength);
					strcat(szHistory, "\r\n");

					SendMessage(hEditIn,
						WM_SETTEXT,
						sizeof(szIncoming) - 1,
						reinterpret_cast<LPARAM>(&szHistory));
				}
			}
		}
		break;

		case FD_CLOSE:
		{
			MessageBox(hWnd,
				L"Client closed connection",
				L"Connection closed!",
				MB_ICONINFORMATION | MB_OK);
		}
		break;

		case FD_ACCEPT:
		{
			if (nClient < nMaxClients)
			{
				int size = sizeof(sockaddr);
				Socket[&nClient] = accept(wParam, &sockAddrClient, &size);
				if (Socket[&nClient] == INVALID_SOCKET)
				{
					int nret = WSAGetLastError();
					WSACleanup();
				}

			}
			nClient++;
		}
		break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}

	// Message handler for about box.
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	{
		HWND hDlg;
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
}
\ No newline at end of file

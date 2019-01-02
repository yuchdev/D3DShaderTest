// Tutorial-02.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tutorial-02.h"
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst = NULL;							// current instance
HWND hMainWnd = NULL;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

LPDIRECT3D9 m_pD3D = NULL;
LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
BOOL InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TUTORIAL02, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	RECT rc;
	GetClientRect(hMainWnd, &rc);
	if (!InitD3D(hMainWnd, rc.right-rc.left, rc.bottom-rc.top))
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TUTORIAL02));

	// Main message loop:
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			struct VertPosDiffuse 
			{
				D3DXVECTOR4 m_pos;
				D3DCOLOR m_color;
				VertPosDiffuse(D3DXVECTOR4 pos, D3DCOLOR color)
					: m_pos(pos), m_color(color)
				{}
			};
			
			VertPosDiffuse v[] = 
			{
				VertPosDiffuse(D3DXVECTOR4(  0,   0, 0, 1), D3DCOLOR_XRGB(255, 0, 0)),
				VertPosDiffuse(D3DXVECTOR4(400,   0, 0, 1), D3DCOLOR_XRGB(0, 0, 255)),
				VertPosDiffuse(D3DXVECTOR4(400, 400, 0, 1), D3DCOLOR_XRGB(0, 255, 0))
			};

			m_pd3dDevice->BeginScene();
			m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_STENCIL| D3DCLEAR_ZBUFFER, 0x808080, 0, 0);

			m_pd3dDevice->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
			m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v, sizeof(VertPosDiffuse));
			m_pd3dDevice->EndScene();
			m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		}
	}
	return (int) msg.wParam;
}


//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TUTORIAL02));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_TUTORIAL02);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hMainWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hMainWnd)
   {
      return FALSE;
   }

   ShowWindow(hMainWnd, nCmdShow);
   UpdateWindow(hMainWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ERASEBKGND:
		return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight)
{
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if (m_pD3D==NULL)
		return FALSE;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferWidth = iWindowWidth;
	d3dpp.BackBufferHeight = iWindowHeight;
	d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	HRESULT hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice);

	return !FAILED(hr);
}


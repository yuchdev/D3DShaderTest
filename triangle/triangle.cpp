// Tutorial-02.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include <d3d9.h>
#include <d3dx9.h>

static const int MAX_LOADSTRING = 100;

/// @brief Triangles application window class
class ApplicationWindow
{
public:

    friend int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

    // @brief Registers the window class
    // This function and its usage are only necessary if you want this code
    // to be compatible with Win32 systems prior to the 'RegisterClassEx'
    // function that was added to Windows 95. It is important to call this function
    // so that the application will get 'well formed' small icons associated
    // with it
    static ATOM MyRegisterClass(HINSTANCE hInstance);

    /// @brief Saves instance handle and creates main window
    /// In this function, we save the instance handle in a class member and
    /// create and display the main program window.
    static BOOL InitInstance(HINSTANCE, int);

    /// @brief Initialize Direct3D subsystem
    static BOOL InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight);

    /// @brief Run window messages processing
    /// WM_COMMAND	- process the application menu
    /// WM_PAINT	- Paint the main window
    /// WM_DESTROY	- post a quit message and return
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

    /// @brief Window title
    static LPSTR WindowTitle() { return m_wndTitle; }

    /// @brief Window class
    static LPSTR WindowClass() { return m_wndClass; }

    /// @brief Window handle
    static HWND WindowHandle() { return m_hMainWnd; }

    /// @brief Direct3D device
    static LPDIRECT3DDEVICE9 Direct3DDevice() { return m_d3dDevice; }

    /// @brief Setter of window handle
    static void SetWindowHandle(HWND handle) { m_hMainWnd = handle; }

    /// @brief Setter of application handle
    static void SetInstanceHandle(HINSTANCE handle) { m_hInst = handle; }

private:

    /// Direct3D 
    static LPDIRECT3D9 m_D3D;

    /// Direct3D device
    static LPDIRECT3DDEVICE9 m_d3dDevice;

    /// Application handle
    static HINSTANCE m_hInst;

    /// Window handle
    static HWND m_hMainWnd;

    /// Window display title
    static CHAR m_wndTitle[MAX_LOADSTRING];

    /// Window class
    static CHAR m_wndClass[MAX_LOADSTRING];
};

/// Init static class members
LPDIRECT3D9 ApplicationWindow::m_D3D = NULL;
LPDIRECT3DDEVICE9 ApplicationWindow::m_d3dDevice = NULL;
HINSTANCE ApplicationWindow::m_hInst = NULL;
HWND ApplicationWindow::m_hMainWnd = NULL;
CHAR ApplicationWindow::m_wndTitle[MAX_LOADSTRING] = {};
CHAR ApplicationWindow::m_wndClass[MAX_LOADSTRING] = {};


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, ApplicationWindow::m_wndTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TRIANGLE, ApplicationWindow::m_wndClass, MAX_LOADSTRING);
    ApplicationWindow::MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!ApplicationWindow::InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    RECT rc;
    GetClientRect(ApplicationWindow::WindowHandle(), &rc);
    if (!ApplicationWindow::InitD3D(ApplicationWindow::WindowHandle(), rc.right - rc.left, rc.bottom - rc.top)) {
        return FALSE;
    }

    // Main message loop
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRIANGLE));
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while(msg.message != WM_QUIT)
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

            ApplicationWindow::Direct3DDevice()->BeginScene();
            ApplicationWindow::Direct3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_STENCIL| D3DCLEAR_ZBUFFER, 0x808080, 0, 0);

            ApplicationWindow::Direct3DDevice()->SetFVF(D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
            ApplicationWindow::Direct3DDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v, sizeof(VertPosDiffuse));
            ApplicationWindow::Direct3DDevice()->EndScene();
            ApplicationWindow::Direct3DDevice()->Present(NULL, NULL, NULL, NULL);
        }
    }
    return (int) msg.wParam;
}

ATOM ApplicationWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRIANGLE));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= NULL;
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= ApplicationWindow::WindowClass();
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL ApplicationWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    ApplicationWindow::SetInstanceHandle(hInstance);
    
    HWND handle = CreateWindow(ApplicationWindow::WindowClass(), 
        ApplicationWindow::WindowTitle(), 
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        800, 600, 
        NULL, NULL, hInstance, NULL);

    if (!handle) {
        return FALSE;
    }

    ApplicationWindow::SetWindowHandle(handle);

    ShowWindow(ApplicationWindow::WindowHandle(), nCmdShow);
    UpdateWindow(ApplicationWindow::WindowHandle());

    return TRUE;
}

LRESULT CALLBACK ApplicationWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL ApplicationWindow::InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight)
{
    m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_D3D==NULL)
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

    HRESULT hr = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice);

    return !FAILED(hr);
}


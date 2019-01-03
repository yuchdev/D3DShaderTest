#include "resource.h"

#include <fstream>
#include <sstream>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>

#define NOMINMAX
#define EXIT_ON_FAILURE(hr) if(FAILED((hr))) { return FALSE; }

static const int MAX_LOADSTRING = 256;

/// @brief Textures application window class
class ApplicationWindow
{
public:

    /// Access from WinMain
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

    /// Shaders
    static LPDIRECT3DPIXELSHADER9 m_pixelShader;
    static LPDIRECT3DVERTEXSHADER9 m_vertexShader;

    /// Shader tables
    static LPD3DXCONSTANTTABLE m_vertexShaderTable;
    static LPD3DXCONSTANTTABLE m_pixelShaderTable;

    /// Texture to load from file
    static LPDIRECT3DTEXTURE9 m_3dTexture;
};

/// Init static class members
LPDIRECT3D9 ApplicationWindow::m_D3D = NULL;
LPDIRECT3DDEVICE9 ApplicationWindow::m_d3dDevice = NULL;
HINSTANCE ApplicationWindow::m_hInst = NULL;
HWND ApplicationWindow::m_hMainWnd = NULL;
CHAR ApplicationWindow::m_wndTitle[MAX_LOADSTRING] = {};
CHAR ApplicationWindow::m_wndClass[MAX_LOADSTRING] = {};
LPDIRECT3DPIXELSHADER9 ApplicationWindow::m_pixelShader = NULL;
LPDIRECT3DVERTEXSHADER9 ApplicationWindow::m_vertexShader = NULL;
LPD3DXCONSTANTTABLE ApplicationWindow::m_vertexShaderTable = NULL;
LPD3DXCONSTANTTABLE ApplicationWindow::m_pixelShaderTable = NULL;
LPDIRECT3DTEXTURE9 ApplicationWindow::m_3dTexture = NULL;


std::string GetFileContent(const std::string& filename)
{
    std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, ApplicationWindow::m_wndTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_TEXTURE, ApplicationWindow::m_wndClass, MAX_LOADSTRING);
    ApplicationWindow::MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!ApplicationWindow::InitInstance(hInstance, nCmdShow))
        return FALSE;

    RECT rc;
    GetClientRect(ApplicationWindow::m_hMainWnd, &rc);
    if (!ApplicationWindow::InitD3D(ApplicationWindow::m_hMainWnd, rc.right - rc.left, rc.bottom - rc.top)) {
        return FALSE;
    }

    // Main message loop
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEXTURE));
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
                D3DXVECTOR3 m_pos;
                D3DCOLOR m_color;
                VertPosDiffuse(D3DXVECTOR3 pos, D3DCOLOR color)
                    : m_pos(pos), m_color(color)
                {}
            };

            VertPosDiffuse v[] = 
            {
                VertPosDiffuse(D3DXVECTOR3(-1, -1, 0), D3DCOLOR_XRGB(0, 0, 0)),
                VertPosDiffuse(D3DXVECTOR3( 1, -1, 0), D3DCOLOR_XRGB(255, 0, 0)),
                VertPosDiffuse(D3DXVECTOR3(-1,  1, 0), D3DCOLOR_XRGB(0, 255, 0)),
                VertPosDiffuse(D3DXVECTOR3( 1,  1, 0), D3DCOLOR_XRGB(255, 255, 0))
            };
            ApplicationWindow::m_d3dDevice->BeginScene();
            ApplicationWindow::m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_STENCIL|D3DCLEAR_ZBUFFER, 0xff808080, 1, 0);

            ApplicationWindow::m_d3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
            ApplicationWindow::m_d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

            D3DXMATRIX mat, matViewProj, matProj, matView;
            static float angle=0.0f;
            angle+=.03f;
            D3DXMatrixRotationY(&mat, angle);
            D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/3, 800.f/600, .01f, 20);
            D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(2, 2, 2), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0, 1, 0));

            D3DXMatrixMultiply(&matViewProj, &matView, &matProj);

            ApplicationWindow::m_d3dDevice->SetPixelShader(ApplicationWindow::m_pixelShader);
            ApplicationWindow::m_d3dDevice->SetVertexShader(ApplicationWindow::m_vertexShader);

            ApplicationWindow::m_vertexShaderTable->SetMatrix(ApplicationWindow::m_d3dDevice, "mWorld", &mat);
            ApplicationWindow::m_vertexShaderTable->SetMatrix(ApplicationWindow::m_d3dDevice, "mViewProjection", &matViewProj);
            ApplicationWindow::m_d3dDevice->SetTexture(0, ApplicationWindow::m_3dTexture);
            ApplicationWindow::m_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            ApplicationWindow::m_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            ApplicationWindow::m_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
            ApplicationWindow::m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VertPosDiffuse));
            ApplicationWindow::m_d3dDevice->EndScene();
            ApplicationWindow::m_d3dDevice->Present(NULL, NULL, NULL, NULL);
        }
    }
    
    return static_cast<int>(msg.wParam);
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
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTURE));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= NULL;
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= m_wndClass;
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
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    HRESULT hr = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice);
    EXIT_ON_FAILURE(hr);

    LPD3DXBUFFER dxErrorBuffer = NULL;
    LPD3DXBUFFER dxShaderBuffer = NULL;

    std::string vertexShaderSrc = GetFileContent("shaders/vertex_shader.hlsl");
    hr = D3DXCompileShader(vertexShaderSrc.c_str(), 
        vertexShaderSrc.size(), 
        NULL, NULL, 
        "main", "vs_3_0", 
        D3DXSHADER_OPTIMIZATION_LEVEL3, &dxShaderBuffer, &dxErrorBuffer, &m_vertexShaderTable);
    EXIT_ON_FAILURE(hr);

    hr = m_d3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(dxShaderBuffer->GetBufferPointer()), &m_vertexShader);
    EXIT_ON_FAILURE(hr);

    dxShaderBuffer->Release();

    std::string pixelShaderSrc = GetFileContent("shaders/pixel_shader.hlsl");
    hr = D3DXCompileShader(pixelShaderSrc.c_str(), 
        pixelShaderSrc.size(), 
        NULL, NULL, 
        "pixel_shader_main", "ps_3_0", 
        D3DXSHADER_OPTIMIZATION_LEVEL3, &dxShaderBuffer, &dxErrorBuffer, &m_pixelShaderTable);
    EXIT_ON_FAILURE(hr);

    hr = m_d3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(dxShaderBuffer->GetBufferPointer()), &m_pixelShader);
    EXIT_ON_FAILURE(hr);

    dxShaderBuffer->Release();

    hr = D3DXCreateTextureFromFile(m_d3dDevice, "textures/stone-ground-diff.dds", &m_3dTexture);
    EXIT_ON_FAILURE(hr);

    return TRUE;    
}


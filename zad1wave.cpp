#ifndef UNICODE
#define UNICODE
#endif 

#pragma comment(lib, "d2d1")
#include "zad1wave.h"

using D2D1::RenderTargetProperties;
using D2D1::HwndRenderTargetProperties;
using D2D1::SizeU;
using D2D1::Point2F;
using D2D1::StrokeStyleProperties;

ID2D1Factory7* d2d_factory = nullptr;
ID2D1HwndRenderTarget* d2d_render_target = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

point points[51][51];
const double pi = 3.14159265358979323846;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.hCursor = NULL;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
        return 0;
    
    ShowWindow(hwnd, nCmdShow);

    for (int i = 0; i < 51; i++) 
    {
        double x = -1 + 0.04 * i;
        for (int j = 0; j < 51; j++) 
        {
            double y = -1 + 0.04 * j;
            double z = cos(10 * sqrt(pow(x, 2) + pow(y, 2)))/4;
            point p(x, y, z);
            points[i][j] = p;
        }
    }
    
    for (int i = 0; i < 51; i++) 
    {
        for (int j = 0; j < 51; j++) 
        {
            double prev_x = points[i][j].x;
            points[i][j].x = points[i][j].x * cos(pi/4) - points[i][j].y * sin(pi/4);
            points[i][j].y = prev_x * sin(pi/4) + points[i][j].y * cos(pi/4);
        }
    }

    int divider = 256;
    for (int i = 0; i < 51; i++) 
    {
        for (int j = 0; j < 51; j++) 
            points[i][j].y = points[i][j].z * sin(-pi / divider) + points[i][j].y * cos(-pi / divider);
        
    }

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

RECT rc{};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &d2d_factory);
        
        GetClientRect(hwnd, &rc);

        d2d_factory->CreateHwndRenderTarget(
            RenderTargetProperties(),
            HwndRenderTargetProperties(hwnd,
                SizeU(static_cast<UINT32>(rc.right) -
                    static_cast<UINT32>(rc.left),
                    static_cast<UINT32>(rc.bottom) -
                    static_cast<UINT32>(rc.top))),
            &d2d_render_target);

        return 0;
    }
    case WM_DESTROY:
        if (d2d_render_target) d2d_render_target->Release();
        if (d2d_factory) d2d_factory->Release();

        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {   
        PAINTSTRUCT ps;
        
        ID2D1SolidColorBrush* brush = nullptr;
        ID2D1StrokeStyle* brush_style = nullptr;
    
        // Sta³e z kolorami
        D2D1_COLOR_F const clear_color =
        { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
        D2D1_COLOR_F const brush_color_1 =
        { .r = 0.0f, .g = 0.0f, .b = 0.75f, .a = 1.0f };

        // Utworzenie pêdzla i jego stylu
        d2d_render_target->CreateSolidColorBrush(brush_color_1, &brush);
        d2d_factory->CreateStrokeStyle(
            StrokeStyleProperties(
                D2D1_CAP_STYLE_ROUND, D2D1_CAP_STYLE_ROUND,
                D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f,
                D2D1_DASH_STYLE_DOT, 0.0f),
            nullptr, 0, &brush_style);
        
        // Rysowanie
        d2d_render_target->BeginDraw();
        d2d_render_target->Clear(clear_color);

        double for_y = (rc.bottom - rc.top) / 2;
        double for_x = (rc.right - rc.left) / 2;
        int scale = 200;

        int divider = 256;
        for (int i = 0; i < 51; i++) 
        {
            for (int j = 0; j < 51; j++) 
            {
                double prev_z = points[i][j].z; 
                points[i][j].z = points[i][j].z * cos(pi / divider) - points[i][j].y * sin(pi / divider);
                points[i][j].y = prev_z * sin(pi / divider) + points[i][j].y * cos(pi / divider);
            }
        }

        brush->SetColor(brush_color_1);
        for (int i = 0; i < 50; i++) 
        {
            for (int j = 0; j < 50; j++) 
            {
                d2d_render_target->DrawLine(Point2F((points[i][j].x) * scale +for_x, (points[i][j].y) * scale + for_y),
                    Point2F((points[i + 1][j].x) * scale +for_x, (points[i + 1][j].y) * scale + for_y), brush, 1.0f);
                d2d_render_target->DrawLine(Point2F((points[i][j].x) * scale +for_x, (points[i][j].y) * scale + for_y),
                    Point2F((points[i][j + 1].x) * scale +for_x, (points[i][j + 1].y) * scale + for_y), brush, 1.0f);
                if (i == 49) 
                    d2d_render_target->DrawLine(Point2F((points[i + 1][j].x) * scale +for_x, (points[i + 1][j].y) * scale + for_y),
                        Point2F((points[i + 1][j + 1].x) * scale +for_x, (points[i + 1][j + 1].y) * scale + for_y), brush, 1.0f);
                if (j == 49) 
                    d2d_render_target->DrawLine(Point2F((points[i][j + 1].x) * scale +for_x, (points[i][j + 1].y) * scale + for_y),
                        Point2F((points[i + 1][j + 1].x) * scale +for_x, (points[i + 1][j + 1].y) * scale + for_y), brush, 1.0f);
            }
        }

        d2d_render_target->EndDraw();

        if (brush_style) brush_style->Release();
        if (brush) brush->Release();
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#pragma once

#ifndef ZAD1WAVE_H
#define ZAD1WAVE_H

#include <windows.h>
#include <d2d1_3.h>
#include <d2d1.h>
#include <math.h>
#include <stdio.h>

struct point
{
    double x;
    double y;
    double z;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
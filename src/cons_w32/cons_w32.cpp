// Markus Borris, 2011
// This file is part of Toolib library.

//!
/** Taken from Improved Console 4.0 (Rauch, Baeckmann)
*/
//! \file

#include "Toolib/cons_w32/cons_w32.h"
#include <stdexcept>


namespace too
{
namespace con
{
Console::Console()
    : hWnd(GetConsoleWindow())
    , hOutput(GetStdHandle(STD_OUTPUT_HANDLE))
    , hInput(GetStdHandle(STD_INPUT_HANDLE))
    , wndBufMode(false)
    , colFG(FG_WHITE)
    , colBG(BG_BLACK)
    , col(colFG | colBG)
    , dblbuf(0)
    , m_bKeyEvent(false)
    , m_bMouseEvent(false)
{
    HMODULE kernel32 = GetModuleHandle(TEXT("kernel32.dll"));
    if (!kernel32)
        throw std::runtime_error("no handle for kernel32.dll");
    SetConsoleDisplayMode = reinterpret_cast<SETCONSOLEDISPLAYMODE>(GetProcAddress(kernel32, "SetConsoleDisplayMode"));

    hide();
    disableWndBufMode();
    setWndPos(10, 10);
    setWndSize(80, 25); // initializes cWidthHeight, rLTRB, dblbuf
    clear();
}

Console::~Console()
{
    try
    {
        enableWndBufMode();
    }
    catch (...)
    {
    }
    try
    {
        delete[] dblbuf;
    }
    catch (...)
    {
    }
}

Console& Console::getInstance()
{
    static Console instance;
    return instance;
}

void Console::hide()
{
    ShowWindow(hWnd, SW_HIDE);
}

void Console::show()
{
    ShowWindow(hWnd, SW_SHOW);
}

void Console::minimize()
{
    ShowWindow(hWnd, SW_MINIMIZE);
}

void Console::maximize()
{
    ShowWindow(hWnd, SW_MAXIMIZE);
}

void Console::restore()
{
    ShowWindow(hWnd, SW_NORMAL);
}

bool Console::isWndBufMode() const
{
    return wndBufMode;
}

void Console::enableWndBufMode()
{
    SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
    wndBufMode = true;
}

void Console::disableWndBufMode()
{
    SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT);
    setWndBufSize(getWndSizeX() + 1, getWndSizeY() + 1);
    wndBufMode = false;
}

bool Console::isWndFSMode() const
{
    DWORD flags;
    GetConsoleDisplayMode(&flags);

    return (flags & CONSOLE_FULLSCREEN_MODE) != 0;
}

void Console::enableWndFSMode()
{
    COORD newScreenBufferDimensions;
    SetConsoleDisplayMode(hOutput, CONSOLE_FULLSCREEN_MODE, &newScreenBufferDimensions);
}

void Console::disableWndFSMode()
{
    COORD newScreenBufferDimensions;
    SetConsoleDisplayMode(hOutput, CONSOLE_WINDOWED_MODE, &newScreenBufferDimensions);
}

//! In pixels.
int Console::getWndPosX() const
{
    RECT rect;
    GetWindowRect(hWnd, &rect);

    return rect.left;
}

//! In pixels.
int Console::getWndPosY() const
{
    RECT rect;
    GetWindowRect(hWnd, &rect);

    return rect.top;
}

//! In pixels.
void Console::setWndPos(int x, int y)
{
    SetWindowPos(hWnd, 0, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

//! Get max. number of columns.
SHORT Console::getMaxWndSizeX() const
{
    return GetLargestConsoleWindowSize(hOutput).X;
}

//! Get max. number of rows.
SHORT Console::getMaxWndSizeY() const
{
    return GetLargestConsoleWindowSize(hOutput).Y;
}

std::basic_string<TCHAR> Console::getTitle() const
{
    const int MAX_TITLE_LEN = 512;

    TCHAR title[MAX_TITLE_LEN];
    GetConsoleTitle(title, MAX_TITLE_LEN);

    return std::basic_string<TCHAR>(title);
}

void Console::setTitle(const std::basic_string<TCHAR>& title)
{
    SetConsoleTitle(title.c_str());
}

// Helper.
void Console::zeroWndSize()
{
    SMALL_RECT wndSize;
    wndSize.Top    = 1;
    wndSize.Left   = 1;
    wndSize.Right  = 1;
    wndSize.Bottom = 1;
    SetConsoleWindowInfo(hOutput, TRUE, &wndSize);

    COORD bufSize;
    bufSize.X = 1;
    bufSize.Y = 1;
    SetConsoleScreenBufferSize(hOutput, bufSize);

    /*cWidthHeight.X = 0; cWidthHeight.Y = 0;
    rLTRB.Left = 0; rLTRB.Top = 0;
    rLTRB.Right = 0; rLTRB.Bottom = 0;*/
}

const COORD Console::cLeftTop = {0, 0};
} // con
} // too

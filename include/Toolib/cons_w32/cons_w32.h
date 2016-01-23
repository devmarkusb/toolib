// Markus Borris, 2011
// This file is part of Toolib library.

//!
/** Taken from Improved Console 4.0 (Rauch, Baeckmann)

Extensions: write(), double buffering (doesn't work yet), inlining a lot
Removed: "shorties", macros
*/
//! \file

#ifndef CONS_W32_H_INCL_cni3nb8z145z12
#define CONS_W32_H_INCL_cni3nb8z145z12

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <tchar.h>
#include <windows.h>
#include "Toolib/PPDEFS.h"
#include "../../ToolibDEF.h"

#undef min
#undef max


namespace too
{
namespace con
{
typedef WORD CColor;

enum TextColor : WORD
{
    FG_BLACK     = 0,
    FG_DARKRED   = FOREGROUND_RED,
    FG_DARKGREEN = FOREGROUND_GREEN,
    FG_DARKBLUE  = FOREGROUND_BLUE,
    FG_OCHER     = FOREGROUND_RED | FOREGROUND_GREEN,
    FG_VIOLET    = FOREGROUND_RED | FOREGROUND_BLUE,
    FG_TURQUOISE = FOREGROUND_GREEN | FOREGROUND_BLUE,
    FG_GREY      = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

    FG_DARKGREY  = FOREGROUND_INTENSITY | FG_BLACK,
    FG_RED       = FOREGROUND_INTENSITY | FG_DARKRED,
    FG_GREEN     = FOREGROUND_INTENSITY | FG_DARKGREEN,
    FG_BLUE      = FOREGROUND_INTENSITY | FG_DARKBLUE,
    FG_YELLOW    = FOREGROUND_INTENSITY | FG_OCHER,
    FG_PINK      = FOREGROUND_INTENSITY | FG_VIOLET,
    FG_LIGHTBLUE = FOREGROUND_INTENSITY | FG_TURQUOISE,
    FG_WHITE     = FOREGROUND_INTENSITY | FG_GREY
};

enum BgColor : WORD
{
    BG_BLACK     = 0,
    BG_DARKRED   = BACKGROUND_RED,
    BG_DARKGREEN = BACKGROUND_GREEN,
    BG_DARKBLUE  = BACKGROUND_BLUE,
    BG_OCHER     = BACKGROUND_RED | BACKGROUND_GREEN,
    BG_VIOLET    = BACKGROUND_RED | BACKGROUND_BLUE,
    BG_TURQUOISE = BACKGROUND_GREEN | BACKGROUND_BLUE,
    BG_GREY      = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

    BG_DARKGREY  = BACKGROUND_INTENSITY | BG_BLACK,
    BG_RED       = BACKGROUND_INTENSITY | BG_DARKRED,
    BG_GREEN     = BACKGROUND_INTENSITY | BG_DARKGREEN,
    BG_BLUE      = BACKGROUND_INTENSITY | BG_DARKBLUE,
    BG_YELLOW    = BACKGROUND_INTENSITY | BG_OCHER,
    BG_PINK      = BACKGROUND_INTENSITY | BG_VIOLET,
    BG_LIGHTBLUE = BACKGROUND_INTENSITY | BG_TURQUOISE,
    BG_WHITE     = BACKGROUND_INTENSITY | BG_GREY
};

class TOOLIBSHARED_EXPORT Console
{
private:
    // Console window.
    HWND hWnd;
    static const COORD cLeftTop;
    COORD cWidthHeight;
    SMALL_RECT rLTRB; // left, top, right, bottom

    // Standard output device.
    HANDLE hOutput;
    // Standard input device.
    HANDLE hInput;
    // Window buffered mode. Has nothing to do with double buffering.
    // True means buffer and window are the same, no scrolling.
    bool wndBufMode;

    CColor colFG;
    CColor colBG;
    CColor col; // always = colFG|colBG

    CHAR_INFO* dblbuf;

    bool m_bKeyEvent;
    bool m_bMouseEvent;
    KEY_EVENT_RECORD m_tKeyEvent;     // undefined when m_bKeyEvent false
    MOUSE_EVENT_RECORD m_tMouseEvent; // undefined when m_bMouseEvent false

    // Already implemented functions without declaration in <windows.h>
    typedef BOOL(WINAPI* SETCONSOLEDISPLAYMODE)(HANDLE, DWORD, PCOORD);
    SETCONSOLEDISPLAYMODE SetConsoleDisplayMode;

    Console();

public:
    ~Console();

    // Singleton access
    static Console& getInstance();

    // Hide/Show console
    void hide();
    void show();
    // todo shown()

    // todo activ, deactiv, actived

    // Minimize/Maximize/Restore console
    void minimize();
    void maximize();
    void restore();

    //! Clear screen directly.
    void clrscr(char character = ' ')
    {
        DWORD charsWritten;
        FillConsoleOutputCharacterA(hOutput, character, getWndBufSizeX() * getWndBufSizeY(), cLeftTop, &charsWritten);
    }
    //! Clear double buffer.
    void clear(char character = ' ')
    {
        // clearColor(color);
        // clearText(character);
        size_t iSize = getDblBufSize();
        for (size_t i = 0; i < iSize; ++i)
        {
            dblbuf[i].Attributes     = col;
            dblbuf[i].Char.AsciiChar = character;
        }
    }

    // Get/Set: Color
    CColor getColor() const
    {
        return col;
        // return getCSBI().wAttributes;
    }

    void setColor(CColor color)
    {
        col = color;
        SetConsoleTextAttribute(hOutput, color);
    }

    // Get/Set: Text color
    TextColor getTextColor() const
    {
        return static_cast<TextColor>(colFG);
        // return getTextColor(getCSBI().wAttributes);
    }
    void setTextColor(TextColor color)
    {
        colFG = color;
        col = color | colBG;
        SetConsoleTextAttribute(hOutput, color | colBG);
    }

    // Get/Set: Background color
    BgColor getBgColor() const
    {
        return static_cast<BgColor>(colBG);
        // return getBgColor(getCSBI().wAttributes);
    }
    //! If you want this for the full screen, you have to clear it afterwards.
    void setBgColor(BgColor color)
    {
        colBG = color;
        col = colFG | color;
        SetConsoleTextAttribute(hOutput, colFG | color);
    }

    // Get/Set: Cursor position (on screen, not double buffer).
    int getCurPosX() const { return getCSBI().dwCursorPosition.X; }

    int getCurPosY() const { return getCSBI().dwCursorPosition.Y; }

    void setCurPos(SHORT x, SHORT y)
    {
        COORD pos;
        pos.X = x;
        pos.Y = y;
        SetConsoleCursorPosition(hOutput, pos);
    }

    // Get/Set: Cursor size
    int getCurSize() const
    {
        CONSOLE_CURSOR_INFO cci = getCCI();

        if (!cci.bVisible)
            return 0;

        return cci.dwSize;
    }

    void setCurSize(int size)
    {
        CONSOLE_CURSOR_INFO cci;

        if (size > 0)
        {
            cci.bVisible = TRUE;
            cci.dwSize   = size;
        }
        else
        {
            cci.bVisible = FALSE;
            cci.dwSize   = 100;
        }

        SetConsoleCursorInfo(hOutput, &cci);
    }

    // Enable/Disable: Window buffered mode (do not mistake for double buffer).
    bool isWndBufMode() const;
    void enableWndBufMode();
    void disableWndBufMode();

    // Enable/Disable: Window fullscreen mode
    bool isWndFSMode() const;
    void enableWndFSMode();
    void disableWndFSMode();

    // Get/Set: Window position
    int getWndPosX() const;
    int getWndPosY() const;
    void setWndPos(int x, int y);


    //! Columns.
    SHORT getWndSizeX() const
    {
        // return getCSBI().srWindow.Right - getCSBI().srWindow.Left + 1;
        return cWidthHeight.X;
    }

    //! Rows.
    SHORT getWndSizeY() const
    {
        // return getCSBI().srWindow.Bottom - getCSBI().srWindow.Top + 1;
        return cWidthHeight.Y;
    }

    //! Columns, rows.
    void setWndSize(SHORT x, SHORT y)
    {
        COORD bufSize;
        bufSize.X = std::min(x, getMaxWndSizeX());
        bufSize.Y = std::min(y, getMaxWndSizeY());
        if (!wndBufMode)
        {
            zeroWndSize();

            SetConsoleScreenBufferSize(hOutput, bufSize);
        }

        SMALL_RECT wndSize;
        wndSize.Top    = cLeftTop.Y;
        wndSize.Left   = cLeftTop.X;
        wndSize.Right  = bufSize.X - 1;
        wndSize.Bottom = bufSize.Y - 1;
        SetConsoleWindowInfo(hOutput, TRUE, &wndSize);
        cWidthHeight.X = bufSize.X;
        cWidthHeight.Y = bufSize.Y;
        rLTRB.Left     = wndSize.Left;
        rLTRB.Top      = wndSize.Top;
        rLTRB.Right    = wndSize.Right;
        rLTRB.Bottom = wndSize.Bottom;
        if (dblbuf) // first allocation ever by constructor (calling this method)
            delete[] dblbuf;
        dblbuf = new CHAR_INFO[cWidthHeight.X * cWidthHeight.Y];
    }

    //! Get: Maximal window size in columns, rows.
    SHORT getMaxWndSizeX() const;
    SHORT getMaxWndSizeY() const;

    // Get/Set: Title
    std::basic_string<TCHAR> getTitle() const;
    void setTitle(const std::basic_string<TCHAR>& title);

    //! Without double buffer, directly to screen
    void write(const std::string& s)
    {
        DWORD dwIgnore = 0L;
        WriteConsoleA(hOutput, s.c_str(), static_cast<DWORD>(s.length()), &dwIgnore, NULL);
    }
    //! Target double buffer.
    void puts(int x, int y, const std::string& s)
    {
        if (x < cLeftTop.X || y < cLeftTop.Y)
            return;
        unsigned int pos = static_cast<unsigned int>(x + y * cWidthHeight.X);
        size_t len = s.length();
        if (pos + len > getDblBufSize())
            return;
        for (size_t i = 0; i < s.length(); ++i)
        {
            dblbuf[pos + i].Attributes     = col;
            dblbuf[pos + i].Char.AsciiChar = s[i];
        }
    }

    //! \param pause in ms.
    void CopyDblBuffer2Screen(long pause)
    {
        // (const CHAR_INFO *)(&dblbuf)
        BOOL fSuccess = WriteConsoleOutputA(hOutput, dblbuf, cWidthHeight, cLeftTop, &rLTRB);
        if (fSuccess == FALSE)
        {
            std::ostringstream os;
            os << "WriteConsoleOutput failed! " << GetLastError();
            throw std::runtime_error(os.str());
        }
        sleep(pause);
    }

    //! Call first to get events. Then call getFirstKeyEvent and getFirstMouseClickEvent.
    /** Notices only the first key and mouse button in the queue.*/
    bool getEvents()
    {
        bool bRet = false;
        DWORD num = 0, cNumRead = 0;
        cNumRead = 0;
        GetNumberOfConsoleInputEvents(hInput, &num);
        INPUT_RECORD irInBuf[128];
        if (num == 0)
            return false;
        if (!ReadConsoleInput(hInput, irInBuf, 128, &cNumRead))
            return false;
        for (DWORD i = 0; i < cNumRead; i++)
        {
            if (!m_bKeyEvent && irInBuf[i].EventType == KEY_EVENT)
            {
                m_tKeyEvent = irInBuf[i].Event.KeyEvent;
                bRet = m_bKeyEvent = true;
            }
            if (!m_bMouseEvent && irInBuf[i].EventType == MOUSE_EVENT)
            {
                if (irInBuf[i].Event.MouseEvent.dwButtonState != 0)
                {
                    m_tMouseEvent = irInBuf[i].Event.MouseEvent;
                    bRet = m_bMouseEvent = true;
                }
            }
            if (m_bKeyEvent && m_bMouseEvent)
                break;
        }
        return bRet;
    }
    bool getFirstKeyEvent(KEY_EVENT_RECORD& e)
    {
        if (!m_bKeyEvent)
            return false;
        e.wVirtualKeyCode = m_tKeyEvent.wVirtualKeyCode;
        m_bKeyEvent       = false;
        return true;
    }
    bool getFirstMouseClickEvent(MOUSE_EVENT_RECORD& e)
    {
        if (!m_bMouseEvent)
            return false;
        e             = m_tMouseEvent;
        m_bMouseEvent = false;
        return true;
    }

private:
    // Helper
    CONSOLE_CURSOR_INFO getCCI() const
    {
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hOutput, &cci);

        return cci;
    }

    CONSOLE_SCREEN_BUFFER_INFO getCSBI() const
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOutput, &csbi);

        return csbi;
    }

    // Helper: getTextColor()/getBgColor()
    // BgColor getBgColor (CColor color) const { return static_cast<BgColor>(color & 0xF0); }
    // TextColor getTextColor (CColor color) const { return static_cast<TextColor>(color & 0x0F); }

    // Helper for setWndSize()
    void zeroWndSize();

    // Get/Set: Window buffer size
    int getWndBufSizeX() const { return getCSBI().dwSize.X; }

    int getWndBufSizeY() const { return getCSBI().dwSize.Y; }

    void setWndBufSize(SHORT x, SHORT y)
    {
        if (!wndBufMode)
            return;

        COORD size;
        size.X = x;
        size.Y = y;
        SetConsoleScreenBufferSize(hOutput, size);
    }
    void sleep(long time) { Sleep(time); }
    size_t getDblBufSize() { return cWidthHeight.X * cWidthHeight.Y; }

    // Forbidden
    Console(const Console&);
    Console& operator=(const Console&);
};
}
}

//! This is almost always useful in connection with the general console stuff.
#include "catch_exit.h"


#endif

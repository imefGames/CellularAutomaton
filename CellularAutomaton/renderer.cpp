#include "renderer.h"

#include <Windows.h>

namespace CellularAutomaton
{
    Renderer::Renderer()
    {
        HANDLE stdInHandle{ GetStdHandle(STD_INPUT_HANDLE) };
        HANDLE stdOutHandle{ GetStdHandle(STD_OUTPUT_HANDLE) };

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(stdOutHandle, &csbi);
        m_BufferWidth = csbi.srWindow.Right - csbi.srWindow.Left;
        m_BufferHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;

        SetConsoleMode(stdInHandle, ENABLE_EXTENDED_FLAGS);
        SetConsoleMode(stdInHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(stdOutHandle, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(stdOutHandle, &cursorInfo);

        m_Buffer.resize(m_BufferWidth * m_BufferHeight);
    }

    void Renderer::DrawCharacter(unsigned int x, unsigned int y, char c)
    {
        GetCharacterInfoAtPosition(x, y).m_Character = c;
    }

    void Renderer::DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            for (unsigned int i = 0; i < width; ++i)
            {
                char c{ ' ' };
                bool isOnBorderX{ i == 0 || i == width - 1 };
                bool isOnBorderY{ j == 0 || j == height - 1 };
                if (isOnBorderX && isOnBorderY)
                {
                    c = '+';
                }
                else if (isOnBorderX)
                {
                    c = '|';
                }
                else if (isOnBorderY)
                {
                    c = '-';
                }
                DrawCharacter(x + i, y + j, c);
            }
        }
    }

    void Renderer::RenderFrame()
    {
        CHAR_INFO* chiBuffer{ new CHAR_INFO[m_BufferWidth * m_BufferHeight]{} };
        for (unsigned int i = 0; i < m_Buffer.size(); ++i)
        {
            const CharacterInfo& currentCharInfo{ m_Buffer[i] };
            chiBuffer[i].Char.AsciiChar = currentCharInfo.m_Character;
            chiBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }

        COORD coordBufCoord{ 0, 0 };
        COORD coordBufSize{ m_BufferWidth, m_BufferHeight };
        SMALL_RECT srctWriteRect{ 0, 0, m_BufferWidth - 1, m_BufferHeight - 1 };

        WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), chiBuffer, coordBufSize, coordBufCoord, &srctWriteRect);
        delete[] chiBuffer;
    }

    Renderer::CharacterInfo& Renderer::GetCharacterInfoAtPosition(unsigned int x, unsigned int y)
    {
        return m_Buffer[x + y * m_BufferWidth];
    }
}
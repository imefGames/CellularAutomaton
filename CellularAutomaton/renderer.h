#pragma once

#include <vector>

namespace CellularAutomaton
{
    enum class EColor : unsigned char
    {
        Black,
        Red,
        Green,
        Blue,
        Yellow,
        Pink,
        Cyan,
        White
    };

    class Renderer
    {
    public:
        Renderer();

        void DrawCharacter(unsigned int x, unsigned int y, char c, EColor color = EColor::White);
        void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, EColor color = EColor::White);

        void RenderFrame();

    private:
        struct CharacterInfo
        {
            char m_Character = ' ';
            EColor m_Color = EColor::White;
        };

        CharacterInfo& GetCharacterInfoAtPosition(unsigned int x, unsigned int y);

        std::vector<CharacterInfo> m_Buffer;
        unsigned int m_BufferWidth;
        unsigned int m_BufferHeight;
    };
}
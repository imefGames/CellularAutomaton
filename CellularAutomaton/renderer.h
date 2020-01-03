#pragma once

#include <vector>

namespace CellularAutomaton
{
    class Renderer
    {
    public:
        Renderer();

        void DrawCharacter(unsigned int x, unsigned int y, char c);
        void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

        void RenderFrame();

    private:
        struct CharacterInfo
        {
            char m_Character = ' ';
        };

        CharacterInfo& GetCharacterInfoAtPosition(unsigned int x, unsigned int y);

        std::vector<CharacterInfo> m_Buffer;
        unsigned int m_BufferWidth;
        unsigned int m_BufferHeight;
    };
}
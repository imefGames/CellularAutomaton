#pragma once

#include <functional>
#include <string>

namespace CellularAutomaton
{
    class InputHandler;
    class Renderer;

    class Button
    {
    public:
        Button(unsigned int x, unsigned int y, const std::string& text, char shortcutInput, const std::function<void()>& callback);

        void RegisterButton(InputHandler& inputHandler);

        void Render(Renderer& renderer);
        void OnClick(unsigned int x, unsigned int y);

    private:
        std::string m_Text;
        std::function<void()> m_Command;
        unsigned int m_X;
        unsigned int m_Y;
        char m_ShortcutInput;
    };
}
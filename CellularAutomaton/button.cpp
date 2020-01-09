#include "button.h"

#include "inputhandler.h"
#include "renderer.h"

namespace CellularAutomaton
{
    Button::Button(unsigned int x, unsigned int y, const std::string& text, char shortcutInput, const std::function<void()>& command)
        : m_X{ x }
        , m_Y{ y }
        , m_ShortcutInput{ shortcutInput }
        , m_Text{ text }
        , m_Command{ command }
    {
    }

    void Button::RegisterButton(InputHandler& inputHandler)
    {
        inputHandler.RegisterButtonPressCommand(m_ShortcutInput, m_Command);
        inputHandler.RegisterMouseClickCommand([this](unsigned int clickX, unsigned int clickY) { OnClick(clickX, clickY); });
    }

    void Button::Render(Renderer& renderer)
    {
        unsigned int x{ m_X };
        unsigned int y{ m_Y };

        renderer.DrawCharacter(x++, y, '"');
        renderer.DrawCharacter(x++, y, m_ShortcutInput);
        renderer.DrawCharacter(x++, y, '"');
        x++;
        for (char c : m_Text)
        {
            renderer.DrawCharacter(x++, y, c);
        }
    }

    void Button::OnClick(unsigned int x, unsigned int y)
    {
        if (y == m_Y && x >= m_X && x < m_X + m_Text.size() + 4)
        {
            m_Command();
        }
    }
}
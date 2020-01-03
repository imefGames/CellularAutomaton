#pragma once

#include <functional>
#include <vector>

namespace CellularAutomaton
{
    class InputHandler
    {
    public:
        using ButtonPressCommandFunction = std::function<void()>;
        using MouseClickCommandFunction = std::function<void(unsigned int, unsigned int)>;

        void RegisterButtonPressCommand(char requiredKey, const ButtonPressCommandFunction& command);
        void RegisterMouseClickCommand(const MouseClickCommandFunction& command);

        void HandleEvents();

    private:
        class ButtonPressCommand
        {
        public:
            ButtonPressCommand();
            ButtonPressCommand(char requiredKey, const ButtonPressCommandFunction& command);

            char GetRequiredKey() const { return m_RequiredKey; }
            void Execute() { m_Command(); }

        private:
            char m_RequiredKey;
            ButtonPressCommandFunction m_Command;
        };

        std::vector<ButtonPressCommand> m_ButtonPressCommands;
        std::vector<MouseClickCommandFunction> m_MouseClickCommands;

    };
}
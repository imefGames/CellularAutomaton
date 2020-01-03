#include "inputhandler.h"

#include <Windows.h>

namespace CellularAutomaton
{
    static constexpr unsigned int K_INPUT_RECORD_BUFFER_SIZE{ 128 };

    void InputHandler::RegisterButtonPressCommand(char requiredKey, const ButtonPressCommandFunction& command)
    {
        m_ButtonPressCommands.emplace_back(requiredKey, command);
    }

    void InputHandler::RegisterMouseClickCommand(const MouseClickCommandFunction& command)
    {
        m_MouseClickCommands.push_back(command);
    }

    void InputHandler::HandleEvents()
    {
        unsigned long readEvents;
        INPUT_RECORD irInBuf[K_INPUT_RECORD_BUFFER_SIZE];
        HANDLE inHandle{ GetStdHandle(STD_INPUT_HANDLE) };
        PeekConsoleInput(inHandle, irInBuf, K_INPUT_RECORD_BUFFER_SIZE, &readEvents);
        FlushConsoleInputBuffer(inHandle);
        for (unsigned long i = 0; i < readEvents; i++)
        {
            switch (irInBuf[i].EventType)
            {
                case KEY_EVENT:
                {
                    const KEY_EVENT_RECORD& keyEvent{ irInBuf[i].Event.KeyEvent };
                    if (keyEvent.bKeyDown)
                    {
                        for (ButtonPressCommand& command : m_ButtonPressCommands)
                        {
                            if (command.GetRequiredKey() == keyEvent.uChar.AsciiChar)
                            {
                                command.Execute();
                            }
                        }
                    }
                    break;
                }

                case MOUSE_EVENT:
                {
                    MOUSE_EVENT_RECORD& mouseEvent{ irInBuf[i].Event.MouseEvent };
                    if (mouseEvent.dwEventFlags == 0 && mouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        unsigned int clickX{ static_cast<unsigned int>(mouseEvent.dwMousePosition.X) };
                        unsigned int clickY{ static_cast<unsigned int>(mouseEvent.dwMousePosition.Y) };

                        for (const MouseClickCommandFunction& command : m_MouseClickCommands)
                        {
                            command(clickX, clickY);
                        }
                    }
                    break;
                }
            }
        }
    }

    InputHandler::ButtonPressCommand::ButtonPressCommand()
        : m_RequiredKey{}
        , m_Command{}
    {
    }

    InputHandler::ButtonPressCommand::ButtonPressCommand(char requiredKey, const ButtonPressCommandFunction& command)
        : m_RequiredKey{ requiredKey }
        , m_Command{ command }
    {
    }
}
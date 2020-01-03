#include "grid.h"
#include "renderer.h"

#include <chrono>
#include <thread>
#include <Windows.h>

int main()
{
    using namespace std::chrono_literals;

    bool keepRunning{ true };
    bool isAutomatonRunning{ false };
    CellularAutomaton::Grid cellularAutomatonGrid{ 1, 2, 50, 20 };
    CellularAutomaton::Renderer renderer;

    cellularAutomatonGrid.RandomizeGrid();

    while (keepRunning)
    {
        unsigned long readEvents;
        INPUT_RECORD irInBuf[128];
        HANDLE inHandle{ GetStdHandle(STD_INPUT_HANDLE) };
        PeekConsoleInput(inHandle, irInBuf, 128, &readEvents);
        FlushConsoleInputBuffer(inHandle);
        for (unsigned long i = 0; i < readEvents; i++)
        {
            switch (irInBuf[i].EventType)
            {
                case KEY_EVENT:
                {
                    if (irInBuf[i].Event.KeyEvent.bKeyDown)
                    {
                        if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar == 'q')
                        {
                            keepRunning = false;
                        }
                        else if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar == 'r')
                        {
                            cellularAutomatonGrid.RandomizeGrid();
                        }
                        else if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar == 'c')
                        {
                            cellularAutomatonGrid.ClearGrid();
                        }
                        else if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar == 's')
                        {
                            if (!isAutomatonRunning)
                            {
                                cellularAutomatonGrid.ComputeNextGrid();
                            }
                        }
                        else if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar == ' ')
                        {
                            isAutomatonRunning = !isAutomatonRunning;
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
                        cellularAutomatonGrid.OnMouseClick(clickX, clickY);
                    }
                    break;
                }
            }
        }

        if (isAutomatonRunning)
        {
            cellularAutomatonGrid.ComputeNextGrid();
        }

        cellularAutomatonGrid.Render(renderer);
        renderer.RenderFrame();
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
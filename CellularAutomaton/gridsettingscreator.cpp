#include "gridsettingscreator.h"

#include "gridsettings.h"
#include "renderer.h"

#include <fstream>
#include <istream>
#include <map>

namespace CellularAutomaton
{
    namespace GridSettingsCreator
    {
        EColor FindColor(const std::string& colorText)
        {
            if (colorText == "Black") { return EColor::Black; }
            if (colorText == "Red") { return EColor::Red; }
            if (colorText == "Green") { return EColor::Green; }
            if (colorText == "Blue") { return EColor::Blue; }
            if (colorText == "Yellow") { return EColor::Yellow; }
            if (colorText == "Pink") { return EColor::Pink; }
            if (colorText == "Cyan") { return EColor::Cyan; }
            if (colorText == "White") { return EColor::White; }
            return EColor::Black;
        }

        GridSettings CreateGridSettings(const std::string& filePath)
        {
            GridSettings createdSettings;

            std::map<std::string, CellID> cells;
            std::ifstream fileStream{ filePath };

            while (!fileStream.eof())
            {
                std::string word{};
                fileStream >> word;

                if (word == "GridPos")
                {
                    unsigned int x{ 0 };
                    unsigned int y{ 0 };
                    fileStream >> x >> y;

                    createdSettings.SetGridX(x);
                    createdSettings.SetGridY(y);
                }
                else if (word == "GridSize")
                {
                    unsigned int w{ 0 };
                    unsigned int h{ 0 };
                    fileStream >> w >> h;

                    createdSettings.SetGridWidth(w);
                    createdSettings.SetGridHeight(h);
                }
                else if (word == "Cell")
                {
                    std::string cellName{};
                    int cellChar{};
                    std::string cellColorText{};
                    fileStream >> cellName >> cellChar >> cellColorText;

                    std::vector<CellDescriptor>& cellDescriptors{ createdSettings.GetCellDescriptors() };
                    CellID cellID{ static_cast<CellID>(cellDescriptors.size()) };
                    EColor cellColor{ FindColor(cellColorText) };

                    cells[cellName] = cellID;
                    cellDescriptors.emplace_back(cellID, static_cast<char>(cellChar), cellColor);
                }
                else if (word == "DefaultTransition")
                {
                    std::string cellName{};
                    std::string destinationCellName{};
                    fileStream >> cellName >> destinationCellName;

                    CellID cellID{ cells[cellName] };
                    CellID destinationCellID{ cells[destinationCellName] };
                    createdSettings.GetCellDescriptors()[cellID].SetDefaultTransitionCellID(destinationCellID);
                }
                else if (word == "Transition")
                {
                    std::string cellName{};
                    std::string destinationCellName{};
                    std::string searchedCellName{};
                    unsigned int searchedCellCount;
                    fileStream >> cellName >> destinationCellName >> searchedCellCount >> searchedCellName;

                    CellID cellID{ cells[cellName] };
                    CellID destinationCellID{ cells[destinationCellName] };
                    CellID searchedCellID{ cells[searchedCellName] };
                    createdSettings.GetCellDescriptors()[cellID].GetTransitions().emplace_back(searchedCellID, searchedCellCount, destinationCellID);
                }
            }
            
            fileStream.close();

            return createdSettings;
        }
    }
}
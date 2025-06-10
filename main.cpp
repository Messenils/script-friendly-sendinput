
#include <iostream>
#include "pch.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>
//char buffer[256];

std::string GetExecutableFolder() {
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    std::string exePath(path);

    // Remove the executable name to get the folder
    size_t lastSlash = exePath.find_last_of("\\/");
    return exePath.substr(0, lastSlash);
}

void SendMouseClick(int x, int y, int z) {
    // Convert screen coordinates to absolute values
    double screenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
    double screenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx = x * (65535.0f / screenWidth);
    double fy = y * (65535.0f / screenHeight);

    INPUT input[3] = {};

    // Move the mouse to the specified position
    input[0].type = INPUT_MOUSE;
    input[0].mi.dx = static_cast<LONG>(fx);
    input[0].mi.dy = static_cast<LONG>(fy);
    input[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    if (z == 1) {
        // Simulate mouse left button down
        input[1].type = INPUT_MOUSE;
        input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        // Simulate mouse left button up
        input[2].type = INPUT_MOUSE; 
        input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }
    if (z == 2) {
        // Simulate mouse left button down
        input[1].type = INPUT_MOUSE;
        input[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

        // Simulate mouse left button up
        input[2].type = INPUT_MOUSE;
        input[2].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }

    SendInput(3, input, sizeof(INPUT));
}

void sendKey(char key) {
    INPUT ip = {};
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = VkKeyScan(key); // Converts character to virtual key code
    ip.ki.dwFlags = 0; // Key down
    SendInput(1, &ip, sizeof(INPUT));

    // Key up event
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

void sendString(const std::string& text) {
    for (char ch : text) {
        sendKey(ch);
        Sleep(50); // Add a small delay to mimic human typing
    }
}
    
std::string getIniString(const std::string& section, const std::string& key, const std::string& defaultValue, const std::string& iniPath) {
    char buffer[256]; // Buffer to hold the retrieved string
    GetPrivateProfileString(section.c_str(), key.c_str(), defaultValue.c_str(), buffer, sizeof(buffer), iniPath.c_str());
    return std::string(buffer);
}

void main()
{
    //init
    ShowWindow(GetConsoleWindow(), SW_HIDE); // Hides the console window
    int delay = 0;
    int xpos = 0;
    int ypos = 0;
    int Mouseclick = 0;
    bool innstilt = false;
    bool timetoquit = false;
    std::string iniPath = GetExecutableFolder() + "\\click.ini";
    int steparray = 0;
    std::string iniSettings = "Settings" + std::to_string(steparray);
    std::string textinput;

    while (timetoquit == false)
    {
        if (innstilt == false)
        {
            delay = GetPrivateProfileInt(iniSettings.c_str(), "Delay", 0, iniPath.c_str());
            xpos = GetPrivateProfileInt(iniSettings.c_str(), "XPOS", 0, iniPath.c_str());
            ypos = GetPrivateProfileInt(iniSettings.c_str(), "YPOS", 0, iniPath.c_str());
            Mouseclick = GetPrivateProfileInt(iniSettings.c_str(), "Mouseclick", 0, iniPath.c_str());
            textinput = getIniString(iniSettings.c_str(), "Textinput", "NOP", iniPath);

            if (Mouseclick == 1 || textinput != "NOP")
            { 
                innstilt = true;
                
            }

        }
        if (Mouseclick == 1 || textinput != "NOP")
        {
            delay = delay - 1;
        }


        if (delay < 0)
        {
            if (Mouseclick == 1) {
                SendMouseClick(xpos, ypos, Mouseclick);
                Mouseclick = 0;
            }
            Sleep(250);
            if (textinput != "NOP") {
                sendString(textinput);
                textinput = "NOP";
            }
            steparray++;
            iniSettings = "Settings" + std::to_string(steparray);
            Mouseclick = GetPrivateProfileInt(iniSettings.c_str(), "Mouseclick", 0, iniPath.c_str());
            textinput = getIniString(iniSettings.c_str(), "Textinput", "NOP", iniPath);
            if (Mouseclick == 1 || textinput != "NOP")
            {
                innstilt = false;

            }
            else { //end of input settings
                timetoquit = true;

            }
                
        }

        if (timetoquit == true)
        { 
            exit(0);
            break;
        }
        Sleep(1000);  // Small delay to prevent excessive CPU usage
    }


}



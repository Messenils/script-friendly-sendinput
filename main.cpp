
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



void SendMouseClick(int x, int y) {
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

    // Simulate mouse left button down
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // Simulate mouse left button up
    input[2].type = INPUT_MOUSE; 
    input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(3, input, sizeof(INPUT));
}

bool main()
{
    //init
    int delay, xpos, ypos, repeat, allset, timesrepeated;
    bool innstilt = false;
    bool timetoquit = false;
    std::string iniPath = GetExecutableFolder() + "\\click.ini";

    



    while (timetoquit == false)
    {
        if (innstilt == false)
        {
            delay = GetPrivateProfileInt("Settings", "Delay", 0, iniPath.c_str());
            xpos = GetPrivateProfileInt("Settings", "XPOS", 0, iniPath.c_str());
            ypos = GetPrivateProfileInt("Settings", "YPOS", 0, iniPath.c_str());
            repeat = GetPrivateProfileInt("Settings", "TimesToRepeat", 0, iniPath.c_str());
            allset = GetPrivateProfileInt("Settings", "AllSet", 0, iniPath.c_str());

            int number = GetPrivateProfileInt("Settings", "AllSet", 0, iniPath.c_str());
            std::string message = "The number is: " + std::to_string(number);
            

            if (allset == 1)
            { 
                innstilt = true;
                
            }
        }
        if (allset == 1)
        {
            delay = delay - 1;
            //MessageBox(NULL, "Hello, World!", "Message Box", MB_OK);
        }


        if (delay < 0)
        {
            timesrepeated++;
            SendMouseClick(xpos, ypos); 
            if (timesrepeated <= repeat)
            {
                innstilt = false;
            }
            else
                timetoquit = true;
        }


        if (timetoquit == true)
        { 
            exit(0);
            break;
        }
        Sleep(1000);  // Small delay to prevent excessive CPU usage
    }

    return false;
}



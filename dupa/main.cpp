#define _WIN32_WINNT 0x0500
#include <iostream>
#include <Windows.h>
#include "stdarg.h"
#include <cwchar> 
using namespace std;

int width = 60; //pixeli 
int height = 30; //pixeli

/* TO SET COLOR: 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i );
*/

// system("CLS") to clear screen

void ShowConsoleCursor(bool showFlag);
void gotoxy(int x, int y);
void drawPlayer();
void checkForInput();

int BLACK = 0;
int WHITE = 255;
int RED = 192;

int pixels [1800];

string pixel = "  ";

int x = 30; int y = 15;
int playerPosX = 15; int playerPosY = 15;
int state;

COORD cord = {120, 31};

int main() {

/////////////// SETUP ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////|
    HWND consoleWindow = GetConsoleWindow();                                                                                                                                              
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); 
    ShowConsoleCursor(false);         
                                                                                                                                                                  
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), cord);       


    // Font setup
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 10;                   // Width of each character in the font
    cfi.dwFontSize.Y = 20;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);                                                                                                                                              
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////|

    for (int i =0;i < 1800;i ++) {
        pixels[i] = BLACK;
    }

    //pixels[y * width + x] = RED;

    int temp = 20;

    while (true) {
         for (int i =0;i < 1800;i ++) {
        pixels[i] = BLACK;
        }
        
        checkForInput();
        drawPlayer();



        for(int i =0; i < height * width; i ++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), pixels[i]);
        cout << pixel;
        }



        gotoxy(0,0);
    }



    while (0==0){}
    return 0;
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD c = { x, y };  
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}

void drawPlayer() {
    pixels[playerPosY * width + playerPosX + 1] = WHITE;
    pixels[(playerPosY + 1) * width + playerPosX] = WHITE;
    pixels[(playerPosY + 1) * width + playerPosX + 1] = WHITE;
    pixels[(playerPosY + 1) * width + playerPosX + 2] = WHITE;
}

void checkForInput() {
     if(GetAsyncKeyState(VK_LEFT)) {
        playerPosX--;
    }

    if(GetAsyncKeyState(VK_RIGHT)) {
        playerPosX++;
    }

    if(GetAsyncKeyState(VK_UP)) {
        playerPosY--;
    }

    if(GetAsyncKeyState(VK_DOWN)) {
        playerPosY++;
    }
}

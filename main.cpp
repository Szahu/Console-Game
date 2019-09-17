#define _WIN32_WINNT 0x0500
#include <iostream>
#include <Windows.h>
#include "stdarg.h"
#include <cwchar> 
#include <chrono>
#include <thread>
#include <vector>
using std::cout;
using  std::cin;
using std::endl;
using std::string;
using std::vector;

bool shot = false;

int width = 60; //pixeli 
int height = 30; //pixeli

/* TO SET COLOR: 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i );
*/

void ShowConsoleCursor(bool showFlag);
void gotoxy(int x, int y);
void drawPlayer();
void clearPixel(int x, int y);
void checkForInput();
void log();
void drawPixel(int x, int y, int col);
void shooting();
void setColor(int col);
void clearBullets();
void shooting();


int BLACK = 0;
int WHITE = 255;
int RED = 192;
int YELLOW = 225;

std::chrono::steady_clock::time_point start;
std::chrono::steady_clock::time_point end;
int elapsed_time_int;
float elapsed_time_f;

std::chrono::steady_clock::time_point actual_end;
int actual_elapsed_time_int;
float actual_elapsed_time_f;

int pixels [1800];

string pixel = "  ";
string bullet = "[]";

int bulletToErase = 0;

int playerPosX = 10; int playerPosY = 10;
int state;

COORD cord = {120, 30};

int verticalLimiter = 0;
int verticalLimiterCheck = 2;

bool space_pressed = false;

struct Enemy {
    int posX; int posY;
    int verticalEnmLimiter = 0;
    int verticalEnmLimiterCheck = 75; // aka movement Speed
    
    void draw() {
        drawPixel(posX - 2, posY, RED);
        drawPixel(posX , posY, RED);
        drawPixel(posX + 2, posY, RED);
        drawPixel(posX - 2 , posY + 1, RED);
        drawPixel(posX + 2 , posY + 1, RED);

        if (verticalEnmLimiter == verticalEnmLimiterCheck) {
            posY++;
            verticalEnmLimiter = 0;
            clearPixel(posX, posY - 1);
            clearPixel(posX - 2, posY - 1);
            clearPixel(posX + 2, posY - 1);
        }

        else {
            verticalEnmLimiter++;
        }
    }

};

struct Bullet {
    int posX; int posY;
    int verticalBulLimiter = 0;
    int verticalBulLimiterCheck = 35; // aka movement Speed

    void draw() {
        gotoxy(posX,posY);
        setColor(14);
        cout << bullet << endl;

        //if (posY - 1 == 0) {
        //   bulletToErase = 1;
        //}

        if (verticalBulLimiter == verticalBulLimiterCheck) {
            posY--;
            clearPixel(posX, posY + 1);
            verticalBulLimiter = 0;
        }
        else {
            verticalBulLimiter++;
        }

    }
};

std::vector<Bullet> bullets;

Bullet test_bullet;

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
////////////// SETUP END/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////|

    for (int i =0;i < 1800;i ++) {
        pixels[i] = BLACK;
        cout << pixel;
    }

    Enemy test_enemy; test_enemy.posX = 10; test_enemy.posY = 10;

/// MAIN LOOP ///
    while (true) {
        start = std::chrono::steady_clock::now();

        /// LOGGING HERE
        log();
        cout << "Bullet to erase: " << bulletToErase <<  "  FPS: " << 1000000 / actual_elapsed_time_f << " Space pressed: " << space_pressed << endl;
        /// LOGGING HERE

///////////////////////////// MAIN LOOP ZONE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        test_enemy.draw();

        checkForInput();
        shooting();
        drawPlayer();
        if (shot) {test_bullet.draw(); }
        //for (Bullet x : bullets) {
        //   x.draw();
        //}

        clearBullets();
//////////////////////////MAIN LOOP ZONE END //////////////////////////////////////////////////////////////////////////////////////////////////////////////   

        end = std::chrono::steady_clock::now();
        elapsed_time_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        elapsed_time_f = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
        std::this_thread::sleep_for(std::chrono::microseconds(10000 - elapsed_time_int));

        actual_end = std::chrono::steady_clock::now();
        actual_elapsed_time_int = std::chrono::duration_cast<std::chrono::microseconds>(actual_end - start).count();
        actual_elapsed_time_f = std::chrono::duration_cast<std::chrono::microseconds>(actual_end - start).count();
    }
/// MAIN LOOP END ///

    return 0;
}

void setColor(int col) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col );
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD c = { x , y };  
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}

void drawPlayer() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);

    gotoxy(playerPosX + 2, playerPosY);
    cout << pixel << endl;

    gotoxy(playerPosX, playerPosY + 1);
    cout << pixel << endl;

    gotoxy(playerPosX + 2, playerPosY + 1);
    cout << pixel << endl;

    gotoxy(playerPosX + 4, playerPosY + 1);
    cout << pixel << endl;
}

void clearPixel(int x, int y) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK);
    gotoxy(x  ,y);
    cout << pixel;
}

void checkForInput() {
    
     if(GetAsyncKeyState(VK_LEFT) && playerPosX - 2 > 0) {
        playerPosX--;
        clearPixel(playerPosX + 4, playerPosY);
        clearPixel(playerPosX + 6, playerPosY +1);
    }

    if(GetAsyncKeyState(VK_RIGHT) && playerPosX + 8 < width  * 2) {
        playerPosX++;
        clearPixel(playerPosX , playerPosY);
        clearPixel(playerPosX - 2, playerPosY +1);
    }

    if(GetAsyncKeyState(VK_UP) && playerPosY > 0) {
        verticalLimiter++;

        if (verticalLimiter == verticalLimiterCheck) {
            playerPosY--;
            clearPixel(playerPosX , playerPosY + 2);
            clearPixel(playerPosX + 2 , playerPosY + 2);
            clearPixel(playerPosX + 4 , playerPosY + 2);
            verticalLimiter = 0;
        }
    }

    if(GetAsyncKeyState(VK_DOWN) && playerPosY + 3 < height) {
        verticalLimiter++;
        
        if (verticalLimiter == verticalLimiterCheck) {
            playerPosY++;
            clearPixel(playerPosX + 2 , playerPosY - 1);
            clearPixel(playerPosX , playerPosY);
            clearPixel(playerPosX + 4 , playerPosY);
            verticalLimiter = 0;
        }     
    }
}

void log() {
    gotoxy(0, 0);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void drawPixel(int x, int y, int col) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
    gotoxy(x  ,y);
    cout << pixel;
}

void shooting() {
    if(GetAsyncKeyState(VK_SPACE) && !shot) {
        space_pressed = true;
        Bullet new_bullet;
        new_bullet.posX = playerPosX + 2;
        new_bullet.posY = playerPosY - 1;
        //bullets.push_back(new_bullet);
        test_bullet = new_bullet;
        shot = true;
    }
    else {
        space_pressed = false;
    }
}

void clearBullets() {
    if (bulletToErase == 1) {
        bullets.pop_back();
    }
}
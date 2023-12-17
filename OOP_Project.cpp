#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

class Car {
private:
    char car[4][4] = { ' ','±','±',' ',
                       '±','±','±','±',
                       ' ','±','±',' ',
                       '±','±','±','±' };
    int position;

public:
    Car() {
        position = WIN_WIDTH / 2;
    }

    void setPosition(int pos) {
        position = pos;
    }

    int getPosition() {
        return position;
    }

    void draw() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                gotoxy(j + position, i + 22);
                cout << car[i][j];
            }
        }
    }

    void erase() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                gotoxy(j + position, i + 22);
                cout << " ";
            }
        }
    }
};

class Enemy {
private:
    int x;
    int y;
    bool flag;

public:
    Enemy() {
        x = 1;
        y = 1;
        flag = false;
    }

    void setX(int xPos) {
        x = xPos;
    }

    void setY(int yPos) {
        y = yPos;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    void setFlag(bool value) {
        flag = value;
    }

    bool getFlag() {
        return flag;
    }

    void generate(int ind) {
        setX(3 + rand() % (WIN_WIDTH - 10));
    }

    void draw() {
        if (flag) {
            gotoxy(x, y);
            cout << "****";
            gotoxy(x, y + 1);
            cout << " ** ";
            gotoxy(x, y + 2);
            cout << "****";
            gotoxy(x, y + 3);
            cout << " ** ";
        }
    }

    void erase() {
        if (flag) {
            gotoxy(x, y);
            cout << "    ";
            gotoxy(x, y + 1);
            cout << "    ";
            gotoxy(x, y + 2);
            cout << "    ";
            gotoxy(x, y + 3);
            cout << "    ";
        }
    }

    void reset() {
        erase();
        y = 4;
        generate(0);
    }
};

class Game:public Enemy {
private:
    Car car;
    Enemy enemies[2];
    int score;
    clock_t startTime;

public:
    Game() {
        score = 0;
        startTime = clock();
    }

    void setcursor(bool visible, DWORD size) {
        if (size == 0)
            size = 20;

        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible = visible;
        lpCursor.dwSize = size;
        SetConsoleCursorInfo(console, &lpCursor);
    }

    void drawBoundary() {
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            gotoxy(i, SCREEN_HEIGHT + 1);
            cout << "±";
        }

        for (int i = -2; i < SCREEN_HEIGHT; i++) {
            gotoxy(0, i + 2);
            cout << "±";
            gotoxy(SCREEN_WIDTH, i + 2);
            cout << "±";
        }

        for (int i = -1; i < SCREEN_HEIGHT; i++) {
            gotoxy(WIN_WIDTH, i + 1);
            cout << "±";
        }
    }

    int collision() {
        int carPos = car.getPosition();

        if (enemies[0].getY() + 4 >= 23 && (enemies[0].getX() + 4 - carPos >= 0 && enemies[0].getX() + 4 - carPos < 9)) {
            return 1;
        }
        if (enemies[1].getY() + 4 >= 23 && (enemies[1].getX() + 4 - carPos >= 0 && enemies[1].getX() + 4 - carPos < 9)) {
            return 1;
        }
        

        return 0;
    }

    void gameOver() {
        system("cls");
        drawBoundary();
        gotoxy(22, 11);
        cout << "--------------------------" << endl;
        gotoxy(22, 12);
        cout << "-------- Game Over -------" << endl;
        gotoxy(22, 13);
        cout << "--------------------------" << endl << endl;
        gotoxy(19, 15);
        cout << "Press Esc or Enter key to go back to the menu.";

        for (int i = 0; i <= SCREEN_HEIGHT; i++) {
            for (int j = 0; j < 17; j++) {
                gotoxy(j + 1, i);
                cout << "*";
                gotoxy(WIN_WIDTH - 1 - j, i);
                cout << "*";
            }
        }

        while (!(GetAsyncKeyState(VK_ESCAPE) & 0x8000) && !(GetAsyncKeyState(VK_RETURN) & 0x8000));
    }

    void drawTimeElapsed() {
        clock_t endTime = clock();
        double elapsedSecs = double(endTime - startTime) / CLOCKS_PER_SEC;

        gotoxy(WIN_WIDTH + 4, 9);
        cout << "Time Elapsed";
        gotoxy(WIN_WIDTH + 3, 11);
        cout << elapsedSecs << " seconds";
    }

    void updateScore() {
        gotoxy(WIN_WIDTH + 6, 6);
        cout << "Score: " << score << endl;
    }

    void instructions() {
        system("cls");
        drawBoundary();
        gotoxy(24, 4);
        cout << "----------------";
        gotoxy(26, 5);
        cout << "Instructions";
        gotoxy(24, 6);
        cout << "----------------";
        gotoxy(18, 9);
        cout << "Avoid Cars by moving left or right. ";
        gotoxy(18, 10);
        cout << "Press Left Arrow to move left";
        gotoxy(18, 11);
        cout << "Press Right Arrow to move right";
        gotoxy(18, 12);
        cout << "Press 'escape' to exit";
        gotoxy(18, 13);
        cout << "Press any key to go back to the menu";
        _getch();
    }

    void play() {
        car.setPosition(-1 + WIN_WIDTH / 2);
        score = 0;
        enemies[0].setFlag(true);
        enemies[1].setFlag(false);
        enemies[0].setY(1);
        enemies[1].setY(1);

        system("cls");
        drawBoundary();
        updateScore();
         generate(0);
         generate(1);
        gotoxy(WIN_WIDTH + 5, 1);
        cout << "----------";
        gotoxy(WIN_WIDTH + 6, 2);
        cout << "Car Game";
        gotoxy(WIN_WIDTH + 5, 3);
        cout << "----------";
        gotoxy(WIN_WIDTH + 5, 5);
        cout << "----------";
        //score
        gotoxy(WIN_WIDTH + 5, 7);
        cout << "----------";
        gotoxy(WIN_WIDTH + 6, 15);
        cout << "Control ";
        gotoxy(WIN_WIDTH + 5, 16);
        cout << "---------";
        gotoxy(WIN_WIDTH + 1, 18);
        cout << " Left Arrow - Left";
        gotoxy(WIN_WIDTH + 1, 20);
        cout << "Right Arrow - Right";

        gotoxy(18, 5);
        cout << "Press any key to start";
        _getch();
        gotoxy(18, 5);
        cout << "                      ";
        startTime = clock();

        while (1) {
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // Left arrow
                int carPos = car.getPosition();
                if (carPos > 2)
                    car.setPosition(carPos - 4);
            }

            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // Right arrow
                int carPos = car.getPosition();
                if (carPos < WIN_WIDTH - 7)
                    car.setPosition(carPos + 4);
            }

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // Escape key
                break;
            }

            drawTimeElapsed();
            car.draw();
            enemies[0].draw();
            enemies[1].draw();

            if (collision() == 1) {
                gameOver();
                return;
            }

            Sleep(50);
            car.erase();
            enemies[0].erase();
            enemies[1].erase();
            

            if (enemies[0].getY() == 10)
                if (enemies[1].getFlag() == 0)
                    enemies[1].setFlag(true);

            if (enemies[0].getFlag() == 1)
                enemies[0].setY(enemies[0].getY() + 1);

            if (enemies[1].getFlag() == 1)
                enemies[1].setY(enemies[1].getY() + 1);

            if (enemies[0].getY() > SCREEN_HEIGHT - 4) {
                enemies[0].reset();
                score++;
                updateScore();
            }

            if (enemies[1].getY() > SCREEN_HEIGHT - 4) {
                enemies[1].reset();
                score++;
                updateScore();
            }
        }
    }
};

int main() {
    Game game;
    game.setcursor(0, 0);
    srand((unsigned)time(NULL));
    Car myCar;
    do {
        system("cls");
        game.drawBoundary();

        gotoxy(20, 5);
        cout << " -------------------------- ";
        gotoxy(20, 6);
        cout << " |        Car Game        | ";
        gotoxy(20, 7);
        cout << " --------------------------";
        gotoxy(17, 10);
        cout << "1. Play Game";
        gotoxy(17, 11);
        cout << "2. Instructions";
        gotoxy(17, 12);
        cout << "3. Quit";
        gotoxy(17, 14);
        cout << "Select option: ";

        char op = _getche();
        if (op == '1') {
            game.play();
        }
        else if (op == '2') {
            game.instructions();
        }
        else if (op == '3') {
            break;
        }
    } while (1);

    return 0;
}

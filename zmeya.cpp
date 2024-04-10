#include <iostream>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
using namespace std;

void generateField(int width, int height) {
    cout << string(width, '_') << endl;

    for (int i = 0; i < height - 2; ++i) {
        cout << "|" << string(width - 2, ' ') << "|" << endl;
    }

    cout << string(width, '_') << endl;
}

void moveSnake(vector<int>& snakeX, vector<int>& snakeY, int dx, int dy) {
    for (int i = snakeX.size() - 1; i > 0; --i) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] += dx;
    snakeY[0] += dy;
}

void changeDirection(int& dx, int& dy, int direction) {
    switch (direction) {
    case 72:
        dx = 0;
        dy = -1;
        break;
    case 80:
        dx = 0;
        dy = 1;
        break;
    case 75:
        dx = -1;
        dy = 0;
        break;
    case 77:
        dx = 1;
        dy = 0;
        break;
    }
}

bool checkCollision(int x, int y, const vector<int>& snakeX, const vector<int>& snakeY) {
    if (x <= 0 || x >= 49 || y <= 0 || y >= 19)
        return true;
    for (size_t i = 1; i < snakeX.size(); ++i) {
        if (x == snakeX[i] && y == snakeY[i])
            return true;
    }

    return false;
}

int main() {
    int dx = 1, dy = 0;
    const int w = 50, h = 20;
    vector<int> snakeX = { w / 2 };
    vector<int> snakeY = { h / 2 };
    char field[w][h];
    const char body = 'o';
    const char head = 'U';
    const char apple = 'O';
    bool end = false;
    srand(time(0));

    int appleX, appleY;
    do {
        appleX = rand() % (w - 2) + 1;
        appleY = rand() % (h - 2) + 1;
    } while (field[appleX][appleY] == body);

    generateField(w, h);

    while (!end) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {
                end = true;
            }
            else {
                changeDirection(dx, dy, key);
            }
        }

        COORD prevCoords = { snakeX.back(), snakeY.back() };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), prevCoords);
        cout << ' ';

        moveSnake(snakeX, snakeY, dx, dy);

        if (checkCollision(snakeX[0], snakeY[0], snakeX, snakeY)) {
            end = true;
            break;
        }

        COORD headCoords = { snakeX[0], snakeY[0] };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), headCoords);
        cout << head;

        for (size_t i = 1; i < snakeX.size(); ++i) {
            COORD bodyCoords = { snakeX[i], snakeY[i] };
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bodyCoords);
            cout << body;
        }

        if (snakeX[0] == appleX && snakeY[0] == appleY) {
            snakeX.push_back(snakeX.back());
            snakeY.push_back(snakeY.back());
            do {
                appleX = rand() % (w - 2) + 1;
                appleY = rand() % (h - 2) + 1;
            } while (field[appleX][appleY] == body || (appleX == snakeX[0] && appleY == snakeY[0]));
        }

        COORD appleCoords = { appleX, appleY };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), appleCoords);
        cout << apple;

        Sleep(250);
    }

    return 0;
}

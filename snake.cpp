#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

vector<pair<int, int>> snake;
pair<int, int> food;
int direction = KEY_RIGHT;
bool gameOver = false;
int width = 60, height = 30;

void initGame() {
    snake.clear();
    snake.push_back(make_pair(height / 2, width / 2));
    srand(time(0));
    food.first = rand() % height;
    food.second = rand() % width;
}

void draw() {
    clear();
    // 边界
    for (int i = 0; i <= width; ++i) {
        mvprintw(0, i, "#");
        mvprintw(height, i, "#");
    }
    for (int i = 0; i <= height; ++i) {
        mvprintw(i, 0, "#");
        mvprintw(i, width, "#");
    }

    // 食物
    mvprintw(food.first, food.second, "@");

    // 蛇
    for (size_t i = 0; i < snake.size(); ++i) {
        mvprintw(snake[i].first, snake[i].second, "*");
    }

    refresh();
}

void input() {
    timeout(0);
    int ch = getch();
    if (ch != ERR) {
        if (ch == KEY_UP && direction != KEY_DOWN) direction = ch;
        else if (ch == KEY_DOWN && direction != KEY_UP) direction = ch;
        else if (ch == KEY_LEFT && direction != KEY_RIGHT) direction = ch;
        else if (ch == KEY_RIGHT && direction != KEY_LEFT) direction = ch;
        else if (ch == 'q') gameOver = true;
    }
}

void logic() {
    pair<int, int> head = snake[0];

    switch (direction) {
        case KEY_UP: head.first--; break;
        case KEY_DOWN: head.first++; break;
        case KEY_LEFT: head.second--; break;
        case KEY_RIGHT: head.second++; break;
    }

    // 撞墙
    if (head.first <= 0 || head.first >= height ||
        head.second <= 0 || head.second >= width) {
        gameOver = true;
        return;
    }

    // 撞自己
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i] == head) {
            gameOver = true;
            return;
        }
    }

    snake.insert(snake.begin(), head); // 加头

    // 吃食物
    if (head == food) {
        food.first = rand() % (height - 1) + 1;
        food.second = rand() % (width - 1) + 1;
    } else {
        snake.pop_back(); // 不吃就去尾
    }
}

int main() {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    noecho();

    initGame();

    while (!gameOver) {
        draw();
        input();
        logic();
        napms(200);  // 每次循环停200毫秒，控制速度
    }

    endwin();
    return 0;
}

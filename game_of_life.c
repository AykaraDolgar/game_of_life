#include <ncurses.h>
#include <stdio.h>

#define y_size 25
#define x_size 80

void drawField(const int x_max, const int y_max, char matr[][x_size]);
void printMes(int code);
int init(char matr_init[][x_size], int *count);
void nextStep(char matr[][x_size], char matr_next[][x_size], int *count);
int compareMatrix(char matr1[][x_size], char matr2[][x_size]);
void copyMatrix(char matr1[][x_size], char matr2[][x_size]);
void check(char ch, int *time, int *flag_quit, int delta);
int countNeighbours(char matr[][x_size], int i, int j);
void drawChar(const int x_max, const int y_max, int x, int y, char matr[][x_size]);

int main() {
    const int x_max = x_size + 2;
    const int y_max = y_size + 2;
    char matr[y_size][x_size];
    int count = 0, time = 150, flag_quit = 0, flag_error = 0, result = 0;
    initscr();
    noecho();
    cbreak();
    result = init(matr, &count);
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        flag_error = 1;
    }
    if (result == 1) {
        printMes(1);
        flag_error = 1;
    }
    if (flag_error == 0) {
        char matr_next[y_size][x_size];
        int delta = 10;
        timeout(time);
        drawField(x_max, y_max, matr);
        check(getch(), &time, &flag_quit, delta);
        nextStep(matr, matr_next, &count);
        while (count > 0 && compareMatrix(matr, matr_next) == 0 && flag_quit == 0) {
            drawField(x_max, y_max, matr_next);
            check(getch(), &time, &flag_quit, delta);
            copyMatrix(matr, matr_next);
            nextStep(matr, matr_next, &count);
        }
        drawField(x_max, y_max, matr_next);
        printMes(0);
    }
    endwin();
    return 0;
}

void check(char ch, int *time, int *flag_quit, int delta) {
    if (ch == 'q') {
        (*flag_quit) = 1;
    } else if ((ch == '+') && (*time > 10)) {
        (*time) -= delta;
        timeout(*time);
    } else if (ch == '-') {
        (*time) += delta;
        timeout(*time);
    }
}

void drawField(const int x_max, const int y_max, char matr[][x_size]) {
    clear();
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            drawChar(x_max, y_max, x, y, matr);
        }
        printw("\n");
    }
    refresh();
}

void drawChar(const int x_max, const int y_max, int x, int y, char matr[][x_size]) {
    if (y == 0 || y == y_max - 1) {
        printw("-");
    } else if (x == 0 || x == x_max - 1) {
        printw("|");
    } else if (matr[y - 1][x - 1] == '1') {
        printw("o");
    } else {
        printw(" ");
    }
}

void printMes(int code) {
    for (int i = 0; i < 12; i++) printw("\n");
    for (int i = 0; i < 35; i++) printw(" ");
    if (code == 1) {
        printw("INCORRECT INPUT");
    } else {
        printw("THE END");
    }
    for (int i = 0; i < 12; i++) printw("\n");
    refresh();
    timeout(100000);
    getch();
}

int init(char matr_init[][x_size], int *count) {
    int flag_quit = 0;
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            matr_init[i][j] = getch();
            if (matr_init[i][j] != '0' && matr_init[i][j] != '1') {
                flag_quit = 1;
            }
            if (matr_init[i][j] == '1') {
                (*count) += 1;
            }
        }
        char ch = getch();
        if ((ch != '\n') && (ch != EOF)) {
            flag_quit = 1;
        }
    }
    if (*count == 0) {
        flag_quit = 1;
    }
    return flag_quit;
}

void nextStep(char matr[][x_size], char matr_next[][x_size], int *count) {
    (*count) = 0;  // считает количество единиц в полученной матрице matr_next
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            int neighbours = countNeighbours(matr, i, j);
            if ((matr[i][j] == '1' && neighbours >= 2 && neighbours <= 3) ||
                (matr[i][j] == '0' && neighbours == 3)) {
                matr_next[i][j] = '1';
                ++(*count);
            } else {
                matr_next[i][j] = '0';
            }
        }
    }
}

int countNeighbours(char matr[][x_size], int i, int j) {
    int neighbours = 0;
    for (int k = i - 1; k <= i + 1; k++) {
        for (int l = j - 1; l <= j + 1; l++) {
            int row = (k + y_size) % y_size;
            int col = (l + x_size) % x_size;
            if ((k != i || l != j) && matr[row][col] == '1') {
                ++neighbours;
            }
        }
    }
    return neighbours;
}

int compareMatrix(char matr1[][x_size], char matr2[][x_size]) {
    int flag_quit = 1;
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            if (matr1[i][j] != matr2[i][j]) {
                flag_quit = 0;
            }
        }
    }
    return flag_quit;
}

void copyMatrix(char matr1[][x_size], char matr2[][x_size]) {
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            matr1[i][j] = matr2[i][j];
        }
    }
}
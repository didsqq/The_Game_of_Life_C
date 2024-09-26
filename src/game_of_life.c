#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define rows 25
#define cols 80

int game(int **mas);

void algorithm(int **old);

void output(int **mas);

void scan(int **mas);

int sumend(int **mas);

int control(char command, int *speed);

void swap(int **mas1, int **mas2);

int sumneib(int i, int j, int **old);

void clearmas(int **mas);

int endgame(int history[500][rows][cols], int **mas);

int main() {
    int **mas = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        mas[i] = malloc(cols * sizeof(int));
    }

    scan(mas);

    if (freopen("/dev/tty", "r", stdin)) initscr();
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_CYAN);
    init_pair(2, COLOR_MAGENTA, COLOR_CYAN);
    nodelay(stdscr, TRUE);

    if (game(mas) == 1) {
        clear();
        endwin();
        printf(
            " ___   __   _  _  ____     __   _  _  ____  ____\n"
            "/ __) / _\\ ( \\/ )(  __)   /  \\ / )( \\(  __)(  _ \\\n"
            "( (_ \\/    \\/ \\/ \\ ) _)   (  O )\\ \\/ / ) _)  )   /\n"
            "\\___/\\_/\\_/\\_)(_/(____)   \\__/  \\__/ (____)(__\\_)\n");
        return 0;
    }
}

int game(int **mas) {
    int speed = 700000;
    while (1) {
        char command;
        command = getch();
        int history[500][rows][cols];
        if (control(command, &speed)) {
            clearmas(mas);
            return 1;
        }
        algorithm(mas);
        if (endgame(history, mas) == 0) {
            clearmas(mas);
            return 1;
        }
        clear();
        output(mas);
        if (sumend(mas) == 0) {
            clearmas(mas);
            return 0;
        }
        usleep(speed);
    }
}

void algorithm(int **old) {
    int **new = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        new[i] = malloc(cols * sizeof(int));
    }
    swap(old, new);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = sumneib(i, j, old);

            if ((sum == 2 || sum == 3) && old[i][j] == 1) {
                new[i][j] = 1;
            } else if (sum == 3 && old[i][j] == 0) {
                new[i][j] = 1;
            } else {
                new[i][j] = 0;
            }
        }
    }

    swap(new, old);
    clearmas(new);
}

int sumneib(int i, int j, int **old) {
    int sprava = j + 1, sleva = j - 1, sverhu = i - 1, snizu = i + 1;
    if (sprava == 80) {
        sprava = 0;
    }
    if (sleva == -1) {
        sleva = 79;
    }
    if (snizu == 25) {
        snizu = 0;
    }
    if (sverhu == -1) {
        sverhu = 24;
    }
    int sum = old[sverhu][sleva] + old[i][sleva] + old[snizu][sleva] + old[sverhu][j] + old[snizu][j] +
              old[sverhu][sprava] + old[i][sprava] + old[snizu][sprava];
    return sum;
}

void swap(int **mas1, int **mas2) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mas2[i][j] = mas1[i][j];
        }
    }
}

void output(int **mas) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mas[i][j] == 1) {
                attron(COLOR_PAIR(1));
                printw("*");
            }
            if (mas[i][j] == 0) {
                attron(COLOR_PAIR(2));
                printw("-");
            }
        }
        if (i != 24) printw("\n");
    }
}
void scan(int **mas) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &mas[i][j]);
        }
    }
}
int sumend(int **mas) {
    int suma = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            suma += mas[i][j];
        }
    }
    return suma;
}
int control(char command, int *speed) {
    if (command == '1') {
        *speed = 1200000;
    }
    if (command == '2') {
        *speed = 700000;
    }
    if (command == '3') {
        *speed = 200000;
    }
    if (command == '+') {
        *speed /= 2;
    }
    if (command == '-') {
        *speed *= 2;
    }
    if (command == 'q' || command == 'Q') {
        return 1;
    }
    return 0;
}

int endgame(int history[500][rows][cols], int **mas) {
    int flag = 1;
    for (int k = 0; k < 499; k++) {
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (history[k][i][j] != mas[i][j]) {
                    count++;
                }
            }
        }
        if (count == 0) {
            flag = 0;
            break;
        }
    }
    if (flag == 1) {
        for (int k = 0; k < 499; k++) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    history[k][i][j] = history[k + 1][i][j];
                }
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                history[499][i][j] = mas[i][j];
            }
        }
    }

    return flag;
}

void clearmas(int **mas) {
    for (int i = 0; i < rows; i++) {
        free(mas[i]);
    }
    free(mas);
}

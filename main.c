#include <stdlib.h>
#include "curses.h"
#include <time.h>
#include <unistd.h>

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define FIELD_WIDTH 20
#define FIELD_HEIGHT 20
#define BOARDS_BEGIN 2
#define HUD_WIDTH 12
#define START_FIGURE_X 10
#define START_FIGURE_Y 0

typedef struct// структура фигуры, пока это #
{
    char figure;
    int x;
    int y;
    int block;
} Figure_t;

typedef struct { // общая сруктура игры
    int **field;
    //int **next;
    //int score;
    // int high_score;
    // int level;
    //int speed;
    //int pause;
} GameInfo_t;


void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) // создать бокс для игрового поля
{
    MVADDCH(top_y, left_x, ACS_ULCORNER);

    int i = left_x + 1;

    for (; i < right_x; i++)
        MVADDCH(top_y, i, ACS_HLINE);
    MVADDCH(top_y, i, ACS_URCORNER);

    for (int i = top_y + 1; i < bottom_y; i++) {
        MVADDCH(i, left_x, ACS_VLINE);
        MVADDCH(i, right_x, ACS_VLINE);
    }
    MVADDCH(bottom_y, left_x, ACS_LLCORNER);
    i = left_x + 1;
    for (; i < right_x; i++)
        MVADDCH(bottom_y, i, ACS_HLINE);
    MVADDCH(bottom_y, i, ACS_LRCORNER);

}

int **create_Field_t() // выделение памяти под поле
{
    int **field = (int **) malloc(FIELD_HEIGHT * sizeof(int *));
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        field[i] = (int *) malloc(FIELD_WIDTH * sizeof(int));
    }
    return field;
}

Figure_t create_new_figure_t(int x, int y) // фигура с определенными координатами и формой
{
    Figure_t figure;
    figure.figure = '#';
    figure.x = x;
    figure.y = y;
    return figure;
}

GameInfo_t *create_Game_t() // созданеи структуры игры
{
    GameInfo_t *tetg = (GameInfo_t *) malloc(sizeof(GameInfo_t));
    tetg->field = create_Field_t();
    return tetg;
}

void draw_box() // отрисовка бокса для поля
{
    print_rectangle(0, FIELD_HEIGHT + 1, 0, FIELD_WIDTH + 1);
    print_rectangle(0, FIELD_HEIGHT + 1, FIELD_WIDTH + 2, FIELD_WIDTH + HUD_WIDTH + 3);
    print_rectangle(1, 5, FIELD_WIDTH + 3, FIELD_WIDTH + HUD_WIDTH + 2);
    print_rectangle(6, 10, FIELD_WIDTH + 3, FIELD_WIDTH + HUD_WIDTH + 2);
    print_rectangle(11, 16, FIELD_WIDTH + 3, FIELD_WIDTH + HUD_WIDTH + 2);
    MVPRINTW(2, FIELD_WIDTH + 6, "LEVEL");
    MVPRINTW(7, FIELD_WIDTH + 6, "SCORE");
    MVPRINTW(12, FIELD_WIDTH + 7, "NEXT");
}

void move_down_figure(Figure_t *figure) {
    figure->x++;
}

void move_left_figure(Figure_t *figure) {
    figure->y--;
}

void move_right_figure(Figure_t *figure) {
    figure->y++;
}

int blocks[FIELD_WIDTH + 1][FIELD_HEIGHT + 1]; // массив для храниеия поля с упавшими блоками

void put_figure(Figure_t *figure/*, GameInfo_t *tetg*/) // уложить фигуру вниз
{

    for (int i = 1; i < FIELD_HEIGHT + 1; i++) {
        for (int j = 1; j < FIELD_WIDTH + 1; j++) {
            if (figure->x == i && figure->y == j) {
                blocks[i][j] = 1;
            }
        }
    }
}

void draw_field(Figure_t figure) // отрисовка происходящей текущей ситуации на поле
{
    for (int i = 1; i < FIELD_HEIGHT + 1; i++) {
        for (int j = 1; j < FIELD_WIDTH + 1; j++) {
            if ((figure.x == i && figure.y == j) || blocks[i][j] == 1)
                MVADDCH(figure.x, figure.y, figure.figure);
            else
                MVADDCH(i, j, ' ');
        }
    }
}

int collision(Figure_t figure) { // столкновение с краями или лежащими фигурами
    int collision = 0;
    if (figure.x == 20 || blocks[figure.x + 1][figure.y]) collision = 1;
    else if (figure.y == 1) collision = 2;
    else if (figure.y == 20) collision = 3;
    return collision;
}

int check_line() { // проверка линий на заполненность
    int full = 0;
    for (int i = 1; i < FIELD_HEIGHT + 1; i++) {
        int counter = 0;
        for (int j = 1; j < FIELD_WIDTH + 1; j++) {
            if (counter < FIELD_WIDTH) {
                if (!blocks[i][j]) continue;
                else {
                    counter++;
                }
            }
        }
        if (counter == FIELD_WIDTH) full = i;
    }
    return full;
}

void delete_line(int full) { // удаление заполненных линий
    for (int i = 1; i < FIELD_HEIGHT + 1; i++) {
        for (int j = 1; j < FIELD_WIDTH + 1; j++) {
            blocks[full][j] = 0;
        }
    }
}

void move_line(int full) { // должна быть функция перемещения линий  ан одну вниз после удаления полной
    for (int i = full; i < 0; i--) {
        for (int j = FIELD_WIDTH; j < 0; j--) {
            if (blocks[i - 1][j]) {
                blocks[i][j] = 1;
                blocks[i - 1][j] = 0;
            }
        }
    }
}

Figure_t start_new_game(GameInfo_t *tetg) // функция начала игры
{
    Figure_t figure = create_new_figure_t(0, 10);
    draw_box();
    draw_field(figure);
    return figure;
}

void main_loop(Figure_t figure, int k/*, GameInfo_t *tetg*/) // главная петля игры
{
    while ((k = getch()) != 27) {
        if (collision(figure) == 1) {
            put_figure(&figure/*, tetg*/);
            draw_field(figure);
            int full = check_line();
            if (full) {
                delete_line(full);
                move_line(full);
            }
            draw_field(figure);
            figure = create_new_figure_t(START_FIGURE_Y, START_FIGURE_X);
            refresh();
        } else {
            if (k == KEY_LEFT) {
                if (collision(figure) == 2) continue;
                else move_left_figure(&figure);
            } else if (k == KEY_RIGHT) {
                if (collision(figure) == 3) continue;
                else move_right_figure(&figure);
            } else {
                halfdelay(10);
                move_down_figure(&figure);
            }
            draw_field(figure);
            refresh();
        }
    }
}

int main() {
    initscr();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    int k;
    GameInfo_t *tetg = create_Game_t();
    Figure_t figure = start_new_game(tetg);
    main_loop(figure, k/*, tetg*/);
    endwin();
    return 0;
}



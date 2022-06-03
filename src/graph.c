// Copyright 2022 basiledd, scourgei, semiotic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stack_d.h"
#include "stack_c.h"

int operation_priority(char c);
int is_digit(char c);
char* get_string_number(char* expr, int* pos);
char* to_postfix(char* infix_expr);
char* input();
double execute(char op, double first, double second);
double dijkstra(char* postfix_expr, double x);
void draw(const int* array);
void fill(int* screen, char* postfix_expr);
int check(char* expression);

int main() {
    // Хранит инфиксное выражение
    char* expression = NULL;
    expression = input();

    if (expression != NULL && !check(expression)) {
        // Хранит постфиксное выражение
        char* postfix_expr = to_postfix(expression);

        // Экран
        int* screen = NULL;

        // Заполняем и рисуем
        fill(screen, postfix_expr);

        free(postfix_expr);
    } else {
        printf("n/a");
    }

    free(expression);

    return 0;
}

// Функция отрисовки экрана
void draw(const int* array) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 80; j++)
            array[j] == i ? printf("*") :  printf(".");
        printf("\n");
    }
}

// Функция заполнения экрана
void fill(int* screen, char* postfix_expr) {
    screen = malloc(sizeof(int) * 80);
    if (screen != NULL) {
        for (int i = 0; i < 80; i++)
            screen[i] = round(12 * dijkstra(postfix_expr, (4 * M_PI / 79) * i) + 12);
        draw(screen);
    }
    free(screen);
}

// Приоритет операторов
int operation_priority(char c) {
    int result = -1;

    if (c == '(')
        result = 0;

    if (c == '+' || c == '-')
        result = 1;

    if (c == '*' || c == '/')
        result = 2;

    if (c == '^' || c == 's' || c == 'c' || c == 't' || c == 'g' || c == 'q' || c == 'l')
        result = 3;

    if (c == '~')
        result = 4;

    return result;
}

// Функция проверки символа на цифру или x
int is_digit(char c) {
    return ('0' <= c && c <= '9') || (c == 'x');
}

// Функция парсинга целочисленных значений возвращает число в виде строки
char* get_string_number(char* expr, int* pos) {
    // Число в виде строки
    char* str_number = NULL;
    // Текущий символ в строке
    int size = 0;

    for (; *pos < (int) strlen(expr); (*pos)++) {
        char num = expr[*pos];
        // Если символ - цифра
        if (is_digit(num) || num == '.') {
            size++;

            // Выделяем память для
            char* temp = (char*) realloc(str_number, sizeof(char) * size);
            if (temp != NULL)
                str_number = temp;

            str_number[size - 1] = num;
        } else {
            (*pos)--;
            break;
        }
    }

    char* temp = (char*) realloc(str_number, sizeof(char) * ++size);

    if (temp != NULL)
        str_number = temp;
    if (str_number)
        str_number[size - 1] = '\0';

    return str_number;
}

// Функция конвертирования в обратную польскую нотацию
char* to_postfix(char* infix_expr) {
    // Строка содержащая постфиксную запись
    char* postfix_expr = malloc(strlen(infix_expr) * sizeof(char) * 2);
    // Текущий символ в постфиксной записи
    int n = 0;
    // Стек, содержащий операторы в виде символов
    stack_c s = new_stack_c();

    for (int i = 0; i < (int) strlen(infix_expr); i++) {
        char c = infix_expr[i];

        // Если символ - цифра
        if (is_digit(c)) {
            // Достаем из строки число
            char* temp = get_string_number(infix_expr, &i);

            // Записываем его в постфиксную запись
            for (int j = 0; j < (int) strlen(temp); j++) {
                postfix_expr[n] = temp[j];
                n++;
            }

            // Добавляем пробел
            postfix_expr[n] = ' ';
            n++;
            free(temp);
        } else if (c == '(') {
            // Если открывыающая скобка - добавляем её в стек
            add_stack_node_c(s, '(');
        } else if (c == ')') {
            // Если закрывающая скобка - записываем в постфиксную запись из стека
            // Всё вплоть до открывающей скобки
            while (!is_empty_c(s) && s->top->c != '(') {
                postfix_expr[n] = extract_stack_node_char(s).c;
                n++;
                postfix_expr[n] = ' ';
                n++;
            }
            // Удаляем открывающуюся скобку из стека
            extract_stack_node_char(s);
        } else if (operation_priority(c) != -1) {  // Если символ - операция
            char op = c;

            // Если унарный минус
            if (op == '-' && (i == 0 || (i > 1 && operation_priority(infix_expr[i - 1]) != -1)))
                // Преобразуем его в тильду
                op = '~';

            // Записываем в постфиксную запись все операторы из стека
            // Имеющие более высокий приоритет
            while (!is_empty_c(s) && operation_priority(s->top->c) >= operation_priority(op)) {
                postfix_expr[n] = extract_stack_node_char(s).c;
                n++;
                postfix_expr[n] = ' ';
                n++;
            }

            if (op == 'c' && infix_expr[i + 1] == 'o')
                // Если косинус, заносим в стек символ 'c'
                add_stack_node_c(s, 'c');
            else if (op == 'c' && infix_expr[i + 1] == 't')
                // Если котангенс, заносим в стек символ 'g'
                add_stack_node_c(s, 'g');
            else if (op == 's' && infix_expr[i + 1] == 'q')
                // Если корень, заносим в стек символ 'q'
                add_stack_node_c(s, 'q');
            else if (op == 'l')
                // Если натуральный логарифм, заносим в стек символ 'l'
                add_stack_node_c(s, 'l');
            else
                // Иначе, заносим в стек оператор
                add_stack_node_c(s, op);

            if ((op == 's' && infix_expr[i + 1] == 'i') || op == 'c' || op == 't' || op == 'g')
                // Пропускаем два символа если это sin cos tan ctg
                i += 2;
            else if (op == 's' && infix_expr[i + 1] == 'q')
                // Пропускаем три символа если это sqrt
                i += 3;
            else if (op == 'l')
                // Пропускаем один символ если это ln
                i += 1;
        }
    }

    // Заносим в постфиксную запись все операторы из стека
    while (!is_empty_c(s)) {
        postfix_expr[n] = extract_stack_node_char(s).c;
        n++;
    }

    // Чистим стек
    destroy_stack_c(s);

    return postfix_expr;
}

// Функция ввода строки с клавиатуры
char* input() {
    char* buffer = NULL;  // Расширяемый буфер для символов
    size_t capacity = 0;  // Вместимость буфера
    size_t size = 0;  // Текущее количество символов в буфере

    int c;  // Считыватель символов или EOF
    int flag = 1;  // Флаг который отвечает за корректность ввода

    // Итеративно получаем символы из стандартного ввода
    while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF) {
        // Увеличиваем буфер, если это необходимо
        if (size + 1 > capacity) {
            // Увеличиваем вместимость буфера, если это возможно
            if (capacity < SIZE_MAX) {
                capacity++;
            } else {
                free(buffer);
                flag = 0;
            }
            // Увеличиваем буфер
            if (flag) {
                char* temp = realloc(buffer, capacity);
                if (temp == NULL) {
                    free(buffer);
                    flag = 0;
                }
                buffer = temp;
            }
        }

        if (!flag)
            break;

        // Добавляем текущий символ в буфер
        buffer[size++] = c;
    }

    // Проверяем, не предоставил ли пользователь никаких входных данных
    if (size == 0 && c == EOF && flag == 1) {
        flag = 0;
    }

    // Проверяем, не предоставил ли пользователь слишком много входных данных
    if (size == SIZE_MAX && flag == 1) {
        free(buffer);
        flag = 0;
    }

    // Если последним прочитанным символом был CR, попробуем также прочитать LF
    if (c == '\r' && (c = fgetc(stdin)) != '\n') {
        // Возвращаем NULL, если символ не может быть вставлен обратно в stdin
        if (c != EOF && ungetc(c, stdin) == EOF) {
            free(buffer);
            flag = 0;
        }
    }

    // Минимизируем буфер
    char* s = NULL;
    if (flag) {
        s = realloc(buffer, size + 1);
        if (s == NULL) {
            free(buffer);
            flag = 0;
        } else {
            // Завершаем строку
            s[size] = '\0';
        }
    }

    // Обрабатываем некорректный ввод
    if (!flag)
        s = NULL;

    return s;
}

int check(char* expression) {
    int flag = 0;
    // Алфавит допустимых символов
    char* alpha = "sincotargqlx()+-*/^.0123456789 ";
    for (int i = 0; i < (int) strlen(expression); i++) {
        if (strchr(alpha, expression[i]) == NULL) {
            // Поднимаем флаг если нашли недопустимый символ
            flag = 1;
            break;
        }
    }
    // Возвращаем значение флага
    return flag;
}

// Функция расчета значений, получает на вход символ и выполняет указанную операцию
double execute(char op, double first, double second) {
    double result = 0;

    if (op == '+') {
        result = first + second;
    } else if (op == '-') {
        result = first - second;
    } else if (op == '*') {
        result = first * second;
    } else if (op == '/') {
        result = first / second;
    } else if (op == '^') {
        result = pow(first, second);
    } else if (op == 's') {  // sin
        result = sin(second);
    } else if (op == 'c') {  // cos
        result = cos(second);
    } else if (op == 't') {  // tan
        result = tan(second);
    } else if (op == 'g') {  // ctg
        result = cos(second) / sin(second);
    } else if (op == 'q') {  // sqrt
        result = pow(second, 0.5);
    } else if (op == 'l') {  // ln
        result = log(second);
    }

    return result;
}

// Алгоритм дейкстры
double dijkstra(char* postfix_expr, double x) {
    // Стек, содержащий операторы в виде действительных чисел
    stack_d d = new_stack_d();
    // Счетчик действий
    int counter = 0;

    for (int i = 0; i < (int) strlen(postfix_expr); i++) {
        char c = postfix_expr[i];

        // Если символ - число
        if (is_digit(c)) {
            if (c != 'x') {
                // Достаем из строки число в виде действительного числа
                char* str_number = get_string_number(postfix_expr, &i);
                char *ptr;
                double num = strtod(str_number, &ptr);

                // Записываем его в стек
                add_stack_node_d(d, num);
                free(str_number);
            } else {  // Если x, обрабатываем его как число
                add_stack_node_d(d, x);
            }
        } else if (operation_priority(c) != -1) {  // Если оператор
            // Прибавляем значение счётчику
            counter += 1;
            // Если оператор - унарный минус
            if (c == '~') {
                double last;
                if (is_empty_d(d))
                    // Если стек пуст, задаем нулевое значение
                    last = 0;
                else
                    // Иначе достаем число из стека
                    last = extract_stack_node_double(d).num;

                // Заносим результат в стек
                add_stack_node_d(d, execute('-', 0, last));

                continue;
            }

            // Достаем значения из стека в обратном порядке
            double second;
            if (is_empty_d(d))
                second = 0;
            else
                second = extract_stack_node_double(d).num;

            double first;
            if (is_empty_d(d))
                first = 0;
            else if (c != 's' && c != 'c' && c != 't' && c != 'g' && c != 'l')
                first = extract_stack_node_double(d).num;
            else
                first = 0;

            // Получаем результат операции и заносим в стек
            add_stack_node_d(d, execute(c, first, second));
        }
    }

    // Запоминаем результат из стека
    double result = extract_stack_node_double(d).num;

    // Чистим стек
    destroy_stack_d(d);

    // Возвращаем результат
    return result;
}

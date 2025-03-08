#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! Скобки у отриц не обязательны для удобства и униерсальности

int sn = 0;
string s = "a-(21.1/b+c)*(-1)";
bool bad = false;
bool in_comp = false;

void expr_comp();
void expr();
void add();
void fact();
void number();
void sign_number();

void error(string s) {
    if (!bad) cout << "Обнаружена ошибка в выражении: " << s << "\nПрограмма прекращает работу\n";
}

// Обработка перехода
void next() {
    if (sn == -1 || sn >= s.size() - 1) sn = -1;
    else {
        sn++;
    }
}

// Проверка цифр 1-9 (по грамматике 0 отдельно)
bool is_digit1_9(char c) {
    return c >= '1' && c <= '9';
}

// <Number> ::= <Digit> {<Digit> | '0'}
void number() {
    if (sn == -1 || !is_digit1_9(s[sn])) {
        error("Число должно начинаться с 1-9");
        bad = true;
        return;
    }
    next();

    while (sn != -1 && (isdigit(s[sn]) || s[sn] == '0')) {
        next();
    }
}

// <Double_Number> ::= (<Digit> | '0') '.' {(<Digit> | '0')}
void double_number() {
    bad = false;
    next();

    if (sn == -1 || !isdigit(s[sn])) {
        error("Ожидалась цифра после '.' в дробном числе");
        bad = true;
        return;
    }

    while (sn != -1 && (isdigit(s[sn]) || s[sn] == '0')) {
        next();
    }
}

// <Sign_Number> ::= '-' (<Number> | <Double_Number>)
void sign_number() {
    next();

    if (sn == -1) {
        error("Нет числа или переменной после '-'");
        bad = true;
        return;
    }

    if (isdigit(s[sn]) || s[sn] == '0') {
        if (s[sn] == '0') {
            next();
            if (sn != -1 && s[sn] == '.') double_number();
        }
        else {
            number();
            if (sn != -1 && s[sn] == '.') double_number();
        }
    }
    else if (isalpha(s[sn])) next();
    else {
        error("Неверный знаковый номер");
        bad = true;
    }
}

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void fact() {
    if (sn == -1) {
        error("Конец строки в множителе");
        bad = true;
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        expr();
        if (sn == -1 || s[sn] != ')') {
            error("Отсутствует закрывающая скобка в множителе");
            bad = true;
            return;
        }
        next();
    }
    else if (s[sn] == '-') {
        sign_number();
    }
    else if (isalpha(s[sn])) {
        next();
    }
    else if (isdigit(s[sn]) || s[sn] == '0') {
        if (s[sn] == '0') {
            next();
            if (sn != -1 && s[sn] == '.') double_number();
            else if (isdigit(s[sn])){
                error("Отсутствует точка после 0 в числе");
                bad = true;
                return;
            }
        }
        else {
            number();
            if (sn != -1 && s[sn] == '.') double_number();
        }
    }
    else {
        error("Неверный множитель");
        bad = true;
    }
}

// <Added> ::= <Factor> {('*' | '/') <Factor>}
void add() {
    fact();
    if (bad || sn == -1) return;

    while (sn != -1 && (s[sn] == '*' || s[sn] == '/')) {
        next();
        fact();
    }
}

// <Expression> ::= <Added> {('+' | '-') <Added>}
void expr() {
    add();
    if (bad || sn == -1) return;

    while (sn != -1 && (s[sn] == '+' || s[sn] == '-')) {
        next();
        add();
    }
}

// <Expression_comp> ::= '('Expression')' ('>' | '<' | '=') '('Expression')'
void expr_comp() {
    if (sn == -1 || !(s[sn] == '>' || s[sn] == '<' || s[sn] == '=')) {
        error("Неверный оператор сравнения");
        bad = true;
        return;
    }
    next();

    if (sn == -1 || s[sn] != '(') {
        error("Ожидалась '(' после оператора сравнения");
        bad = true;
        return;
    }
    next();
    expr();
    if (sn == -1 || s[sn] != ')') {
        error("Отсутствует закрывающая скобка после первого выражения");
        bad = true;
        return;
    }
    next();
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << s << '\n';

    if (s[sn] == 'н') {
        next();
        if (sn != -1 && s[sn] == 'е') { // Исправлено || на &&
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                expr();
                expr_comp();
                if (sn != -1 && s[sn] == ')') next();
                else {
                    error("Отсутствует закрывающая скобка после 'не'");
                    bad = true;
                }
            }
            else {
                error("Ожидалась '(' после 'не'");
                bad = true;
            }
        }
        else {
            error("Ожидалась 'е' после 'н' для 'не'");
            bad = true;
        }
    }
        
    // Проверка "и" & "или"
    if (sn != -1 && s[sn] == 'и') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                expr();
                if (sn != -1 && s[sn] == ')') {
                    next();
                    expr_comp();
                    if (sn != -1 && s[sn] == ',') {
                        next();
                        expr();
                        expr_comp();
                        if (sn != -1 && s[sn] == ')') next();
                        else {
                            error("Отсутствует закрывающая скобка после второго выражения в 'и'");
                            bad = true;
                        }
                    }
                    else {
                        error("Ожидалась ',' в выражении 'и'");
                        bad = true;
                    }
                }
                else {
                    error("Отсутствует закрывающая скобка после первого выражения в 'и'");
                    bad = true;
                }
            }
            else {
                error("Отсутствует открывающая скобка первого выражения в 'и'");
                bad = true;
            }
        }
        else if (sn != -1 && s[sn] == 'л') {
            next();
            if (sn != -1 && s[sn] == 'и') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    expr();
                    expr_comp();
                    if (sn != -1 && s[sn] == ',') {
                        next();
                        expr();
                        expr_comp();
                        if (sn != -1 && s[sn] == ')') next();
                        else {
                            error("Отсутствует закрывающая скобка после второго выражения в 'или'");
                            bad = true;
                        }
                    }
                    else {
                        error("Ожидалась ',' в выражении 'или'");
                        bad = true;
                    }
                }
                else {
                    error("Ожидалась '(' после 'или'");
                    bad = true;
                }
            }
            else {
                error("Ожидалась 'и' после 'л' для 'или'");
                bad = true;
            }
        }
        else {
            error("Ожидалась 'л' после 'и' для 'или'");
            bad = true;
        }
    }

    // Первичный вызов
    if (sn!=-1)expr();
    if (in_comp && sn != -1) expr_comp();

    // Проверка на оставшиеся символы
    if (sn != -1) {
        error("Остались лишние символы в конце выражения");
        bad = true;
    }

    if (!bad) cout << "Строка корректна\n";

    return 0;
    
}
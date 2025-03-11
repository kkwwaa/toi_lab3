#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! Скобки у отриц не обязательны для удобства и униерсальности. Отрицательные выр-ия не пред-ны, домножайте на -1

int sn = 0;
string s = "(9-2)=(q/2)";
bool bad = false;
bool in_comp = false;

void PartOfComparativeExpression();
void Expression();
void Addend();
void Factor();
void Number();
void SignedNumber();

#pragma region MyRegion
void Error(string s) {
    if (!bad) cout << "Обнаружена ошибка в выражении: " << s << "\nПрограмма прекращает работу\n";
}

// Обработка перехода
void next() {
    if (sn == -1 || sn >= s.size() - 1) sn = -1;//проверка ввыхода за границы
    else {
        sn++;
    }
}

// Проверка цифр 1-9 (по грамматике 0 отдельно)
bool is_digit1_9(char c) {
    return c >= '1' && c <= '9';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// <Number> ::= <Digit> {<Digit> | '0'}
void Number() {
    if (!is_digit1_9(s[sn])) {
        Error("Число должно начинаться с 1-9");
        bad = true;
        return;
    }
    next();

    while (sn != -1 && is_digit(s[sn])) {
        next();
    }
}

// <Double_Number> ::= (<Digit> | '0') '.' {(<Digit> | '0')}
void PartOfDoubleNumber() {
    next();

    if (sn == -1 || !is_digit(s[sn])) {
        Error("Ожидалась цифра после '.' в дробном числе");
        bad = true;
        return;
    }

    while (sn != -1 && is_digit(s[sn])) {
        next();
    }
}

// <Sign_Number> ::= '-' (<Number> | <Double_Number>)
void SignedNumber() {
    next();

    if (sn == -1) {
        Error("Нет числа или переменной после '-'");
        bad = true;
        return;
    }

    if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber();
            else {
                Error("Ожидалась '.' в знаковом дробном числе");
                bad = true;
                return;
            }
        }
        else {
            Number();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber();
        }
    }
    else if (is_alpha(s[sn])) next();
    else {
        Error("Неверное знаковое число");
        bad = true;
    }
}

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void Factor() {
    if (sn == -1) {
        Error("Конец строки в множителе");
        bad = true;
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        Expression();
        if (sn == -1 || s[sn] != ')') {
            Error("Отсутствует закрывающая скобка в множителе");
            bad = true;
            return;
        }
        next();
    }
    else if (s[sn] == '-') {
        SignedNumber();
    }
    else if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber();
            else if (is_digit(s[sn])){
                Error("Отсутствует точка после 0 в числе");
                bad = true;
                return;
            }
        }
        else {
            Number();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber();
        }
    }
    else if (is_alpha(s[sn])) {
        next();
    }
    else {
        Error("Неверный множитель");
        bad = true;
    }
}

// <Addend> ::= <Factor> {('*' | '/') <Factor>}
void Addend() {
    Factor();
    if (bad || sn == -1) return;

    while (sn != -1 && (s[sn] == '*' || s[sn] == '/')) {
        next();
        Factor();
    }
}

// <Expression> ::= <Addend> {('+' | '-') <Addend>}
void Expression() {
    Addend();
    if (bad || sn == -1) return;

    while (sn != -1 && (s[sn] == '+' || s[sn] == '-')) {
        next();
        Addend();
    }
}

// <PartOfComparativeExpression> ::= ('>' | '<' | '=') '('Expression')'
void PartOfComparativeExpression() {
    if (sn == -1 || !(s[sn] == '>' || s[sn] == '<' || s[sn] == '=')) {
        Error("Неверный оператор сравнения");
        bad = true;
        return;
    }
    next();

    if (sn == -1 || s[sn] != '(') {
        Error("Ожидалась '(' после оператора сравнения");
        bad = true;
        return;
    }
    next();
    Expression();
    if (sn == -1 || s[sn] != ')') {
        Error("Отсутствует закрывающая скобка после первого выражения");
        bad = true;
        return;
    }
    next();
}

void BooleanExpressionNot() {
    if (s[sn] == 'н') {
        next();
        if (sn != -1 && s[sn] == 'е') {
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                Expression();
                PartOfComparativeExpression();
                if (sn != -1 && s[sn] == ')') next();
                else {
                    Error("Отсутствует закрывающая скобка после 'не'");
                    bad = true;
                }
            }
            else {
                Error("Ожидалась '(' после 'не'");
                bad = true;
            }
        }
        else {
            Error("Ожидалась 'е' после 'н' для 'не'");
            bad = true;
        }
    }
}

void BooleanExpression() {
    if (sn != -1 && s[sn] == '(') {
        next();
        Expression();
        if (sn != -1 && s[sn] == ')') {
            next();
            PartOfComparativeExpression();
            if (sn != -1 && s[sn] == ',') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    Expression();
                    if (sn != -1 && s[sn] == ')') {
                        next();
                        PartOfComparativeExpression();
                        if (sn != -1 && s[sn] == ')') next();
                        else {
                            Error("Отсутствует закрывающая скобка после второго выражения в 'и'");
                            bad = true;
                        }
                    }
                    else {
                        Error("Ожидалась ')' в выражении 'и'");
                        bad = true;
                    }
                }
                else {
                    Error("Ожидалась ',' в выражении 'и'");
                    bad = true;
                }
            }
            else {
                Error("Отсутствует закрывающая скобка после первого выражения в 'и'");
                bad = true;
            }
        }
        else {
            Error("Отсутствует открывающая скобка первого выражения в 'и'");
            bad = true;
        }
    }
}
#pragma endregion

void BooleanExpressionAndOr() {
    // Проверка "и" & "или"
    if (sn != -1 && s[sn] == 'и') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            BooleanExpression();
        }
        else if (sn != -1 && s[sn] == 'л') {
            next();
            if (sn != -1 && s[sn] == 'и') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    BooleanExpression();
                }
                else {
                    Error("Ожидалась '(' после 'или'");
                    bad = true;
                }
            }
            else {
                Error("Ожидалась 'и' после 'л' для 'или'");
                bad = true;
            }
        }
        else {
            Error("Ожидалась 'л' после 'и' для 'или'");
            bad = true;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << s << '\n';

    BooleanExpressionNot();
    BooleanExpressionAndOr();

    // Первичный вызов
    if (sn!=-1)Expression();
    //Если найдено сравнение, то переквалифицируем в сравнительное выражение
    if (in_comp && sn != -1) PartOfComparativeExpression();

    // Проверка на оставшиеся символы
    if (sn != -1) {
        Error("Остались лишние символы в конце выражения");
        bad = true;
    }

    if (!bad) cout << "Строка корректна\n";

    return 0;
    
}
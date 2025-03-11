#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! Скобки у отриц не обязательны для удобства и униерсальности. Отрицательные выр-ия не пред-ны, домножайте на -1

int sn = 0;
string s = "(4)=(9)";
//9-2)=(q/2)";
bool bad = false;
bool in_comp = false, sign = false;

#pragma region Additional
struct TreeNode {
    string inf = "";

    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};
TreeNode* Root = nullptr;
TreeNode* RootComp = nullptr;

void Insert(TreeNode*& R) {
    R = new TreeNode;
    R->inf = s[sn];
}

void Delete(TreeNode*& R) {
    if (R != nullptr) {
        Delete(R->left);
        Delete(R->right);
        delete R;
    }
}

void Write(TreeNode*& R) {
    if (R != nullptr) {
        Write(R->left);
        Write(R->right);
        cout << R->inf << ' ';
    }
}

void Error(string s) {
    if (!bad) cout << "Обнаружена ошибка в выражении: " << s << "\nПрограмма прекращает работу\n";
    Delete(Root);
    bad = true;
} //добавить ошибку с пробелами

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
#pragma endregion

#pragma region Voids
void PartOfComparativeExpression(TreeNode*& R);
void Expression(TreeNode*& R);
void Addend(TreeNode*& R);
void Factor(TreeNode*& R);
void Number(TreeNode*& R);
void SignedNumber(TreeNode*& R);
#pragma endregion

#pragma region Numbers
// <Number> ::= <Digit> {<Digit> | '0'}
void Number(TreeNode*& R) {
    Insert(R);
    if (sign) R->inf = '-' + R->inf;
    next();

    while (sn != -1 && is_digit(s[sn])) {
        R->inf += s[sn];
        next();
    }
}

// <Double_Number> ::= (<Digit> | '0') '.' {(<Digit> | '0')}
void PartOfDoubleNumber(TreeNode*& R) {
    Insert(R);
    next();

    if (sn == -1 || !is_digit(s[sn])) {
        Error("Ожидалась цифра после '.' в дробном числе");
        return;
    }

    while (sn != -1 && is_digit(s[sn])) {
        R->inf += s[sn];
        next();
    }
}

// <Sign_Number> ::= '-' (<Number> | <Double_Number>)
void SignedNumber(TreeNode*& R) {
    sign = true;
    next();

    if (sn == -1) {
        Error("Нет числа или переменной после '-'");
        return;
    }

    if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            R->inf += s[sn];
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber(R);
            else {
                Error("Ожидалась '.' в знаковом дробном числе");
                return;
            }
        }
        else {
            Number(R);
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber(R);
        }
    }
    else if (is_alpha(s[sn])) {
        Insert(R);
        next();
    }
    else {
        Error("Неверное знаковое число");
    }
    sign = false;
}
#pragma endregion

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void Factor(TreeNode*& R) {
    if (sn == -1) {
        Error("Конец строки в множителе");
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        Expression(R);
        if (sn == -1 || s[sn] != ')') {
            Error("Отсутствует закрывающая скобка в множителе");
            return;
        }
        next();
    }
    else if (s[sn] == '-') {
        SignedNumber(R);
    }
    else if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            Insert(R);
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber(R);
            else if (is_digit(s[sn])){
                Error("Отсутствует точка после 0 в числе");
                return;
            }
        }
        else {
            Number(R);
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber(R);
        }
    }
    else if (is_alpha(s[sn])) {
        Insert(R);
        next();
    }
    else {
        Error("Неверный множитель");
    }
}

// <Addend> ::= <Factor> {('*' | '/') <Factor>}
void Addend(TreeNode*& R) {
    Factor(R);
    if (bad || sn == -1) return;

    while (sn != -1 && (s[sn] == '*' || s[sn] == '/')) {
        TreeNode* O = new TreeNode;
        Insert(O);
        O->left = R;
        next();
        Factor(O->right);
        R = O;
    }
}

// <Expression> ::= <Addend> {('+' | '-') <Addend>}
void Expression(TreeNode*& R) {
    Addend(R);
    TreeNode* O = R;

    while (sn != -1 && (s[sn] == '+' || s[sn] == '-')) {
        TreeNode* O = new TreeNode;
        Insert(O);
        O->left = R;
        next();
        Addend(O->right);
        R = O;
    }
}

#pragma region Expressions
// <PartOfComparativeExpression> ::= ('>' | '<' | '=') '('Expression')'
void PartOfComparativeExpression(TreeNode*& R){
    if (sn == -1 || !(s[sn] == '>' || s[sn] == '<' || s[sn] == '=')) {
        Error("Неверный оператор сравнения");
        return;
    }
    TreeNode* O = new TreeNode;
    Insert(O);
    O->left = R;
    next();
    
    if (sn == -1 || s[sn] != '(') {
        Error("Ожидалась '(' после оператора сравнения");
        return;
    }
    next();
    Expression(O->right);
    if (sn == -1 || s[sn] != ')') {
        Error("Отсутствует закрывающая скобка после первого выражения");
        return;
    }
    R = O;
    next();
}

void BooleanExpressionNot(TreeNode*& R) {
    if (s[sn] == 'н') {
        next();
        if (sn != -1 && s[sn] == 'е') {
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                Expression(R);
                PartOfComparativeExpression(R);
                if (sn != -1 && s[sn] == ')') next();
                else {
                    Error("Отсутствует закрывающая скобка после 'не'");
                }
            }
            else {
                Error("Ожидалась '(' после 'не'");
            }
        }
        else {
            Error("Ожидалась 'е' после 'н' для 'не'");
        }
    }
}

void BooleanExpression(TreeNode*& R) {
    if (sn != -1 && s[sn] == '(') {
        next();
        Expression(R);
        if (sn != -1 && s[sn] == ')') {
            next();
            PartOfComparativeExpression(R);
            if (sn != -1 && s[sn] == ',') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    Expression(R);
                    if (sn != -1 && s[sn] == ')') {
                        next();
                        PartOfComparativeExpression(R);
                        if (sn != -1 && s[sn] == ')') next();
                        else {
                            Error("Отсутствует закрывающая скобка после второго выражения в 'и'");
                        }
                    }
                    else {
                        Error("Ожидалась ')' в выражении 'и'");
                    }
                }
                else {
                    Error("Ожидалась ',' в выражении 'и'");
                }
            }
            else {
                Error("Отсутствует закрывающая скобка после первого выражения в 'и'");
            }
        }
        else {
            Error("Отсутствует открывающая скобка первого выражения в 'и'");
        }
    }
}


void BooleanExpressionAndOr(TreeNode*& R) {
    // Проверка "и" & "или"
    if (sn != -1 && s[sn] == 'и') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            BooleanExpression(R);
        }
        else if (sn != -1 && s[sn] == 'л') {
            next();
            if (sn != -1 && s[sn] == 'и') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    BooleanExpression(R);
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
#pragma endregion



int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << s << '\n';

    BooleanExpressionNot(Root);
    BooleanExpressionAndOr(Root);

    // Первичный вызов
    if (sn!=-1)Expression(Root);
    //Если найдено сравнение, то переквалифицируем в сравнительное выражение
    if (in_comp && sn != -1) PartOfComparativeExpression(Root);

    // Проверка на оставшиеся символы
    if (sn != -1) {
        Error("Остались лишние символы в конце выражения");
        bad = true;
    }

    if (!bad) cout << "Строка корректна\n";
    if (Root) Write(Root);

    return 0;
    
}
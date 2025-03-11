#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! ������ � ����� �� ����������� ��� �������� � ��������������. ������������� ���-�� �� ����-��, ���������� �� -1

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
    if (!bad) cout << "���������� ������ � ���������: " << s << "\n��������� ���������� ������\n";
}

// ��������� ��������
void next() {
    if (sn == -1 || sn >= s.size() - 1) sn = -1;//�������� ������� �� �������
    else {
        sn++;
    }
}

// �������� ���� 1-9 (�� ���������� 0 ��������)
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
        Error("����� ������ ���������� � 1-9");
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
        Error("��������� ����� ����� '.' � ������� �����");
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
        Error("��� ����� ��� ���������� ����� '-'");
        bad = true;
        return;
    }

    if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber();
            else {
                Error("��������� '.' � �������� ������� �����");
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
        Error("�������� �������� �����");
        bad = true;
    }
}

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void Factor() {
    if (sn == -1) {
        Error("����� ������ � ���������");
        bad = true;
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        Expression();
        if (sn == -1 || s[sn] != ')') {
            Error("����������� ����������� ������ � ���������");
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
                Error("����������� ����� ����� 0 � �����");
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
        Error("�������� ���������");
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
        Error("�������� �������� ���������");
        bad = true;
        return;
    }
    next();

    if (sn == -1 || s[sn] != '(') {
        Error("��������� '(' ����� ��������� ���������");
        bad = true;
        return;
    }
    next();
    Expression();
    if (sn == -1 || s[sn] != ')') {
        Error("����������� ����������� ������ ����� ������� ���������");
        bad = true;
        return;
    }
    next();
}

void BooleanExpressionNot() {
    if (s[sn] == '�') {
        next();
        if (sn != -1 && s[sn] == '�') {
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                Expression();
                PartOfComparativeExpression();
                if (sn != -1 && s[sn] == ')') next();
                else {
                    Error("����������� ����������� ������ ����� '��'");
                    bad = true;
                }
            }
            else {
                Error("��������� '(' ����� '��'");
                bad = true;
            }
        }
        else {
            Error("��������� '�' ����� '�' ��� '��'");
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
                            Error("����������� ����������� ������ ����� ������� ��������� � '�'");
                            bad = true;
                        }
                    }
                    else {
                        Error("��������� ')' � ��������� '�'");
                        bad = true;
                    }
                }
                else {
                    Error("��������� ',' � ��������� '�'");
                    bad = true;
                }
            }
            else {
                Error("����������� ����������� ������ ����� ������� ��������� � '�'");
                bad = true;
            }
        }
        else {
            Error("����������� ����������� ������ ������� ��������� � '�'");
            bad = true;
        }
    }
}
#pragma endregion

void BooleanExpressionAndOr() {
    // �������� "�" & "���"
    if (sn != -1 && s[sn] == '�') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            BooleanExpression();
        }
        else if (sn != -1 && s[sn] == '�') {
            next();
            if (sn != -1 && s[sn] == '�') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    BooleanExpression();
                }
                else {
                    Error("��������� '(' ����� '���'");
                    bad = true;
                }
            }
            else {
                Error("��������� '�' ����� '�' ��� '���'");
                bad = true;
            }
        }
        else {
            Error("��������� '�' ����� '�' ��� '���'");
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

    // ��������� �����
    if (sn!=-1)Expression();
    //���� ������� ���������, �� ����������������� � ������������� ���������
    if (in_comp && sn != -1) PartOfComparativeExpression();

    // �������� �� ���������� �������
    if (sn != -1) {
        Error("�������� ������ ������� � ����� ���������");
        bad = true;
    }

    if (!bad) cout << "������ ���������\n";

    return 0;
    
}
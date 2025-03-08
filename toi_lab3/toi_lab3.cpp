#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! ������ � ����� �� ����������� ��� �������� � ��������������

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
    if (!bad) cout << "���������� ������ � ���������: " << s << "\n��������� ���������� ������\n";
}

// ��������� ��������
void next() {
    if (sn == -1 || sn >= s.size() - 1) sn = -1;
    else {
        sn++;
    }
}

// �������� ���� 1-9 (�� ���������� 0 ��������)
bool is_digit1_9(char c) {
    return c >= '1' && c <= '9';
}

// <Number> ::= <Digit> {<Digit> | '0'}
void number() {
    if (sn == -1 || !is_digit1_9(s[sn])) {
        error("����� ������ ���������� � 1-9");
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
        error("��������� ����� ����� '.' � ������� �����");
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
        error("��� ����� ��� ���������� ����� '-'");
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
        error("�������� �������� �����");
        bad = true;
    }
}

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void fact() {
    if (sn == -1) {
        error("����� ������ � ���������");
        bad = true;
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        expr();
        if (sn == -1 || s[sn] != ')') {
            error("����������� ����������� ������ � ���������");
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
                error("����������� ����� ����� 0 � �����");
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
        error("�������� ���������");
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
        error("�������� �������� ���������");
        bad = true;
        return;
    }
    next();

    if (sn == -1 || s[sn] != '(') {
        error("��������� '(' ����� ��������� ���������");
        bad = true;
        return;
    }
    next();
    expr();
    if (sn == -1 || s[sn] != ')') {
        error("����������� ����������� ������ ����� ������� ���������");
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

    if (s[sn] == '�') {
        next();
        if (sn != -1 && s[sn] == '�') { // ���������� || �� &&
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                expr();
                expr_comp();
                if (sn != -1 && s[sn] == ')') next();
                else {
                    error("����������� ����������� ������ ����� '��'");
                    bad = true;
                }
            }
            else {
                error("��������� '(' ����� '��'");
                bad = true;
            }
        }
        else {
            error("��������� '�' ����� '�' ��� '��'");
            bad = true;
        }
    }
        
    // �������� "�" & "���"
    if (sn != -1 && s[sn] == '�') {
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
                            error("����������� ����������� ������ ����� ������� ��������� � '�'");
                            bad = true;
                        }
                    }
                    else {
                        error("��������� ',' � ��������� '�'");
                        bad = true;
                    }
                }
                else {
                    error("����������� ����������� ������ ����� ������� ��������� � '�'");
                    bad = true;
                }
            }
            else {
                error("����������� ����������� ������ ������� ��������� � '�'");
                bad = true;
            }
        }
        else if (sn != -1 && s[sn] == '�') {
            next();
            if (sn != -1 && s[sn] == '�') {
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
                            error("����������� ����������� ������ ����� ������� ��������� � '���'");
                            bad = true;
                        }
                    }
                    else {
                        error("��������� ',' � ��������� '���'");
                        bad = true;
                    }
                }
                else {
                    error("��������� '(' ����� '���'");
                    bad = true;
                }
            }
            else {
                error("��������� '�' ����� '�' ��� '���'");
                bad = true;
            }
        }
        else {
            error("��������� '�' ����� '�' ��� '���'");
            bad = true;
        }
    }

    // ��������� �����
    if (sn!=-1)expr();
    if (in_comp && sn != -1) expr_comp();

    // �������� �� ���������� �������
    if (sn != -1) {
        error("�������� ������ ������� � ����� ���������");
        bad = true;
    }

    if (!bad) cout << "������ ���������\n";

    return 0;
    
}
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;//!!! ������ � ����� �� ����������� ��� �������� � ��������������. ������������� ���-�� �� ����-��, ���������� �� -1

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
    if (!bad) cout << "���������� ������ � ���������: " << s << "\n��������� ���������� ������\n";
    Delete(Root);
    bad = true;
} //�������� ������ � ���������

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
        Error("��������� ����� ����� '.' � ������� �����");
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
        Error("��� ����� ��� ���������� ����� '-'");
        return;
    }

    if (is_digit(s[sn])) {
        if (s[sn] == '0') {
            R->inf += s[sn];
            next();
            if (sn != -1 && s[sn] == '.') PartOfDoubleNumber(R);
            else {
                Error("��������� '.' � �������� ������� �����");
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
        Error("�������� �������� �����");
    }
    sign = false;
}
#pragma endregion

// <Factor> ::= <Letter> | <Number> | '(' <Expression> ')' | <Sign_Number>
void Factor(TreeNode*& R) {
    if (sn == -1) {
        Error("����� ������ � ���������");
        return;
    }

    if (s[sn] == '(') {
        if (sn == 0) in_comp = true;
        next();
        Expression(R);
        if (sn == -1 || s[sn] != ')') {
            Error("����������� ����������� ������ � ���������");
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
                Error("����������� ����� ����� 0 � �����");
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
        Error("�������� ���������");
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
        Error("�������� �������� ���������");
        return;
    }
    TreeNode* O = new TreeNode;
    Insert(O);
    O->left = R;
    next();
    
    if (sn == -1 || s[sn] != '(') {
        Error("��������� '(' ����� ��������� ���������");
        return;
    }
    next();
    Expression(O->right);
    if (sn == -1 || s[sn] != ')') {
        Error("����������� ����������� ������ ����� ������� ���������");
        return;
    }
    R = O;
    next();
}

void BooleanExpressionNot(TreeNode*& R) {
    if (s[sn] == '�') {
        next();
        if (sn != -1 && s[sn] == '�') {
            next();
            if (sn != -1 && s[sn] == '(') {
                next();
                Expression(R);
                PartOfComparativeExpression(R);
                if (sn != -1 && s[sn] == ')') next();
                else {
                    Error("����������� ����������� ������ ����� '��'");
                }
            }
            else {
                Error("��������� '(' ����� '��'");
            }
        }
        else {
            Error("��������� '�' ����� '�' ��� '��'");
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
                            Error("����������� ����������� ������ ����� ������� ��������� � '�'");
                        }
                    }
                    else {
                        Error("��������� ')' � ��������� '�'");
                    }
                }
                else {
                    Error("��������� ',' � ��������� '�'");
                }
            }
            else {
                Error("����������� ����������� ������ ����� ������� ��������� � '�'");
            }
        }
        else {
            Error("����������� ����������� ������ ������� ��������� � '�'");
        }
    }
}


void BooleanExpressionAndOr(TreeNode*& R) {
    // �������� "�" & "���"
    if (sn != -1 && s[sn] == '�') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            BooleanExpression(R);
        }
        else if (sn != -1 && s[sn] == '�') {
            next();
            if (sn != -1 && s[sn] == '�') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    BooleanExpression(R);
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
#pragma endregion



int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << s << '\n';

    BooleanExpressionNot(Root);
    BooleanExpressionAndOr(Root);

    // ��������� �����
    if (sn!=-1)Expression(Root);
    //���� ������� ���������, �� ����������������� � ������������� ���������
    if (in_comp && sn != -1) PartOfComparativeExpression(Root);

    // �������� �� ���������� �������
    if (sn != -1) {
        Error("�������� ������ ������� � ����� ���������");
        bad = true;
    }

    if (!bad) cout << "������ ���������\n";
    if (Root) Write(Root);

    return 0;
    
}
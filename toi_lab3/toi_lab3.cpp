#include <iostream>

using namespace std;

int sn = 0;
string s = "(c+-d)*((1+d))";
bool bad = false;

void expr();
void add();
void fact();

void next() {
    if (sn==-1 || sn >= s.size()-1) sn = -1;
    else sn++;
}

void fact() {
    if (s[sn] == '(') {
        next();
        expr();
        if (sn == -1 || s[sn] != ')') bad = true;
    }
    else if (isdigit(s[sn]) || isalpha(s[sn])) {}
    else bad = true;
    next();
}

void add() {
    fact();
    if (bad || sn==-1) return;
    
    if (s[sn] == '*' || s[sn] == '/') {
        next();
        fact();
    }
}

void expr() {
    add();
    if (bad || sn==-1) return;

    else if (s[sn] == '+' || s[sn] == '-') {
        next();
        add();
    }
}

int main()
{
    expr();

    if (bad || sn!=-1) cout << "Error\n";
    else cout << "OK\n";
}


#include <iostream>

using namespace std;

int sn = 0;
string s = "200.892*(-8.6)";//200.892*(-8.6), ((a+b)*(-8.6)), (-c)*(-(1+d)), (c)*(-(1+d))", (c* (-(1 + d)) ? c * (1 + d) - ? ÏÐÎÁÅËÛ, ÂÑ¨ ÎÒÐÈÖ Â ÑÊÎÁÊÀÕ ÄÁ    , a8, 10,2+(-d), (9+a>b-1)`,(9+a)>(b-1)
bool bad = false;

void expr();
void add();
void fact();

void next() {
    if (sn==-1 || sn >= s.size()-1) sn = -1;
    else sn++;
}

void fact() {
    if (sn == -1) {
        bad = true;
        return;
    }
    if (s[sn] == '(') {
        next();
        if (s[sn] == '-') {
            next();
            fact();
        }
        else expr();
        if (sn == -1 || s[sn] != ')') bad = true;
    }
    else if (isalpha(s[sn])) {}
    else if (isdigit(s[sn])) {
        while (sn != -1 && (isdigit(s[sn]) || s[sn] == '.')) next();
        return;
    }
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

    else if (s[sn] == '+' || s[sn] == '-' || s[sn] == '>' || s[sn] == '<' || s[sn] == '==') {
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


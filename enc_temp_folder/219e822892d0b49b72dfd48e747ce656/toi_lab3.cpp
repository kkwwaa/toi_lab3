#include <iostream>

using namespace std;

int sn = 0;
string s = "i+1=b-1";//200.892*(-8.6), ((a+b)*(-8.6)), (-c)*(-(1+d)), (c)*(-(1+d))", (c* (-(1 + d)) ? c * (1 + d) - ? ПРОБЕЛЫ, ВСЁ ОТРИЦ В СКОБКАХ ДБ   12. , a8, 10.2+(-d), (9+a>b-1)`,(9+a)>(b-1), 200.892*(-8.6)*(a/2+34-12.0)
bool bad = false;
bool bol = false;

void expr();
void add();
void fact();

void next() {
	if (sn == -1 || sn >= s.size() - 1) sn = -1;
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
	if (bad || sn == -1) return;

	while (sn != -1 && (s[sn] == '*' || s[sn] == '/')) {
		next();
		fact();
	}
}

void expr() {
    if (bad || sn == -1) return;
    if (s[sn] == 'н') {
        next();
        if (sn != -1 && s[sn] == 'е') { // Исправлено || на &&
            next();
            if (sn != -1 && s[sn] == '(') {
                bol = true;
                next();
                expr(); // Разбираем выражение внутри
                if (sn == -1 || s[sn] != ')') bad = true;
                next();
            }
            else bad = true;
        }
        else bad = true;
        return; // Завершаем обработку "не"
    }

    // Проверка "и" & "или"
    if (s[sn] == 'и') {
        next();
        if (sn != -1 && s[sn] == '(') {
            next();
            expr();
            if (sn != -1 && s[sn] == ',') {
                bol = true;
                next();
                expr();
                if (sn != -1 && s[sn] == ')') {
                    next();
                }
                else bad = true;
            }
            else bad = true;
        }
        else if (sn != -1 && s[sn] == 'л') {
            next();
            if (sn != -1 && s[sn] == 'и') {
                next();
                if (sn != -1 && s[sn] == '(') {
                    next();
                    expr();
                    if (sn != -1 && s[sn] == ',') {
                        bol = true;
                        next();
                        expr();
                        if (sn != -1 && s[sn] == ')') {
                            next();
                        }
                        else bad = true;
                    }
                    else bad = true;
                }
                else  bad = true;
            }
            else bad = true;
        }
        else bad = true;
        return; // Завершаем обработку "и"
    }
	add();
	

	while (sn != -1 && (s[sn] == '+' || s[sn] == '-' || s[sn] == '>' || s[sn] == '<' || s[sn] == '=')) {
		next();
		add();
	}
}

int main()
{
	expr();

	if (bad || sn != -1) cout << "Error\n";
	else cout << "OK\n";
}


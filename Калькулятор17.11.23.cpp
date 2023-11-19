#include <iostream>
#include <sstream>
#include <cmath>
#include <stack>
using namespace std;

const double Pi = acos(-1); //Объявляем значение числа Пи
const double Ex = exp(1);


struct Typ //Структура, описывающая любое число или операцию
{
    char type; // 0 для чисел, "+" для операции сложения и т.д.
    double value; //Значение (только для чисел). У операций значение всегда "0"
};

string Minus(string Line) { //функция, добавляющая 0 перед каждым унарным минусом (зв исключением того, который может стоять в самом начале), тем самым превращая этот минус в бинарный
    int a;
    string Zero = "0";
    string Bracket_minus = "(-";
    int s;
    s = Line.length() - 1;
    for (int i = 0; i < s; ++i) {
        a = i + 1;
        if (Line[i] == Bracket_minus[0] && Line[a] == Bracket_minus[1]) {
            Line.insert(a, Zero);
        }
    }
    return Line;
}

bool Calc(stack <Typ>& Stack_n, stack <Typ>& Stack_o, Typ& item) { //Математическая функция, которая производит расчеты
    //Функция имеет тип bool, чтобы при возникновении какой-либо ошибки возвращать "false"
    double a, b, c;
    a = Stack_n.top().value; //Берется верхнее число из стека с числами
    Stack_n.pop(); //Удаляется верхнее число из стека с числами
    switch (Stack_o.top().type) {  //Проверяется тип верхней операции из стека с операциями
    case '+': //Если тип верхней операции из стека с операциями сложение
        b = Stack_n.top().value;
        Stack_n.pop();
        c = a + b;
        item.type = '0';
        item.value = c;
        Stack_n.push(item); //Результат операции кладется обратно в стек с числами
        Stack_o.pop();
        break;

    case '-':
        b = Stack_n.top().value;
        Stack_n.pop();
        c = b - a;
        item.type = '0';
        item.value = c;
        Stack_n.push(item);
        Stack_o.pop();
        break;

    case '^':
        b = Stack_n.top().value;
        Stack_n.pop();
        c = pow(b, a);
        item.type = '0';
        item.value = c;
        Stack_n.push(item);
        Stack_o.pop();
        break;

    case '*':
        b = Stack_n.top().value;
        Stack_n.pop();
        c = a * b;
        item.type = '0';
        item.value = c;
        Stack_n.push(item);
        Stack_o.pop();
        break;

    case '/':
        b = Stack_n.top().value;
        if (a == 0) {
            cerr << "\nНа 0 делить нельзя!\n";
            return false;
        }
        else {
            Stack_n.pop();
            c = (b / a);
            item.type = '0';
            item.value = c;
            Stack_n.push(item);
            Stack_o.pop();
            break;
        }
    }
    return true;
}

int Prioritet_op(char Ch) { //Функция возвращает приоритет операции: "1" для сложения и вычитания, "2" для умножения и деления и т.д.
    if (Ch == '^')return 3;
    if (Ch == '+' || Ch == '-')return 1;
    if (Ch == '*' || Ch == '/')return 2;
    else return 0;
}

int main()
{
    setlocale(LC_ALL, "rus");
    while (true) {
        system("cls");
        cout << "Введите выражение: ";
        std::string str;
        getline(cin, str);
        str = Minus(str);

        stringstream sstr{ str };

        char ch; //Переменная, в которую будет записываться текущий обрабатываемый символ
        double value;
        bool flag = true; //Нужен для того, чтобы программа смогла отличить унарный минус (-5) от вычитания (2-5)
        stack<Typ> Stack_n; //Стек с числами
        stack<Typ> Stack_o; //Стек с операциями
        Typ item; //Объект типа Typ
        while (true) {
            ch = sstr.peek(); //Смотрим на первый символ
            if (ch == '\377')break; //Если достигнут конец строки, выходим из цикла
            if (ch == ' ') { //Игнорирование пробелов
                sstr.ignore();
                continue;
            }
            if (ch == 'p') { //Если прочитано число Пи
                item.type = '0';
                item.value = Pi;
                Stack_n.push(item); //Число кладется в стек с числами
                flag = 0;
                sstr.ignore();
                continue;
            }
            if (ch == 'e') {
                item.type = '0';
                item.value = Ex;
                Stack_n.push(item);
                flag = 0;
                sstr.ignore();
                continue;
            }
            if (ch >= '0' && ch <= '9' || ch == '-' && flag == 1) { //Если прочитано число
                sstr >> value;
                item.type = '0';
                item.value = value;
                Stack_n.push(item); //Число кладется в стек с числами
                flag = 0;
                continue;
            }
            if (ch == '+' || ch == '-' && flag == 0 || ch == '*' || ch == '/' || ch == '^') { //Если прочитана операция
                if (Stack_o.size() == 0) { //Если стек с операциями пуст
                    item.type = ch;
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    sstr.ignore();
                    continue;
                }
                if (Stack_o.size() != 0 && Prioritet_op(ch) >
                    Prioritet_op(Stack_o.top().type)) { //Если стек с операциями НЕ пуст, но приоритет текущей операции выше верхней в стеке с операциями
                    item.type = ch;
                    item.value = 0;
                    Stack_o.push(item); //Операция кладется в стек с операциями
                    sstr.ignore();
                    continue;
                }
                if (Stack_o.size() != 0 && Prioritet_op(ch) <=
                    Prioritet_op(Stack_o.top().type)) {//Если стек с операциями НЕ пуст, но приоритет текущей операции ниже либо равен верхней в стеке с операциями
                    if (Calc(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                        system("pause");
                        return 0;
                    }
                    continue;
                }
            }
            if (ch == '(') { //Если прочитана открывающаяся скобка
                item.type = ch;
                item.value = 0;
                Stack_o.push(item); //Операция кладется в стек с операциями
                sstr.ignore();
                continue;
            }
            if (ch == ')') { //Если прочитана закрывающаяся скобка
                while (Stack_o.top().type != '(') {
                    if (Calc(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                        system("pause");
                        return 0;
                    }
                    else continue; //Если все хорошо
                }
                Stack_o.pop();
                sstr.ignore();
                continue;
            }
            else { //Если прочитан какой-то странный символ
                cout << "\nНеверно введено выражение!\n";
                system("pause");
                return 0;
            }
        }
        while (Stack_o.size() !=
            0) { //Вызываем матем. функцию до тех пор, пока в стеке с операциями не будет 0 элементов
            if (Calc(Stack_n, Stack_o, item) == false) { //Если функция вернет "false", то прекращаем работу
                system("pause");
                return 0;
            }
            else continue; //Если все хорошо
        }
        cout << "Ответ: " << Stack_n.top().value << endl; //Выводим ответ
        system("pause");
    }
    return 0;
}
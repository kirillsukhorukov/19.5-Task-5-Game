#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Функция вывода игрового поля
void field_print(bool(&field)[13], int& sector)
{
    cout << endl << "- Playing field -" << endl;
    cout << endl << "Q - question letter on the table" << endl;
    cout << "E - sector is empty\n" << endl;
    for (int i = 0; i < 13; i++) ((i + 1) == sector) ? cout << '!' << "\t" : cout << "\t";
    cout << endl;
    for (int i = 0; i < 13; i++) cout << i + 1 << "\t";
    cout << endl;
    for (int i = 0; i < 13; i++) (field[i]) ? cout << 'E' << "\t" : cout << 'Q' << "\t";
    cout << endl << endl;
}

//Функция ввода и проверки целого числа
int int_input()
{
    string str;
    bool error = false;
    do
    {
        cout << "Input the spinning top offset: ";
        getline(cin, str);
        for (int i = 0; i < str.length(); i++)
        {
            if (!isdigit(str[i]) || str.length() > 9)
            {
                cerr << "Error! Repeat input." << endl;
                error = true;
                i = str.length() - 1;
            }
            else error = false;
        }

    } while (error);

    //Возврат целого числа
    return stoi(str);
}

//Функция вращения волчка
void spinning(bool(&field)[13], int& sector)
{
    cout << "- We rotate the spinning top -" << endl;

    //Ввод смещения
    int offset = int_input();

    //Определяем сектор после вращения
    sector = ((sector + offset) % 13 == 0) ? 13 : (sector + offset) % 13;

    //Смещаем сектор до момента пока не попадем на не отыгранный сектор
    while (field[sector - 1])
    {
        sector++;
        if (sector == 14) sector = 1;
    }

    //Вывод текущего игрового поля
    field_print(field, sector);

    //Помечаем сектор на поле как отыгранный
    field[sector - 1] = true;
    return;
}

//Функция печати вопроса 
void question_print(string (&questions)[13], const int& sector, bool &error)
{
    ifstream question;
    //Открываем файл с вопросом
    question.open(questions[sector - 1]);
    //Если файл открылся - печатаем содержимое
    if (question.is_open())
    {
        cout << "----- Question " << sector << " sector -----" << endl;
        while (!question.eof())
        {
            std::string str;
            std::getline(question, str);
            std::cout << str << '\n';
        }
        cout << endl;
        question.close();
    }
    
    //Если не открылся выводим сообщение об ошибке и возвращаем влаг конца игры
    else {
        cerr << "File not found!" << endl;
        error = true;
    }
}

//Функция проверки ответа знатоков
bool checking_answers(string(&answers)[13], const string &answerCon, const int& sector, bool& error)
{
    ifstream answer;
    //Открываем файл с ответом
    answer.open(answers[sector - 1]);
    //Если файл открылся - считываем содержимое и сравниваем ответы
    if (answer.is_open())
    {
        string ans;
        getline(answer, ans);
        answer.close();
        return (answerCon == ans) ? true : false;
    }

    //Если не открылся выводим сообщение об ошибке и возвращаем влаг конца игры
    else {
        cerr << "File not found!" << endl;
        error = true;
        return false;
    }
}

//функция подведения результата раунда
void round_result(const int& pointCon, const int& pointV, bool &endGame)
{
    cout << "------- Game score -------" << endl;
    cout << "Connoisseur - " << pointCon << " : Viewer - " << pointV << endl << endl;

    if (pointCon == 6)
    {
        endGame = true;
        cout << "----- Game over. Connoisseur WIN!!! -----" << endl;
    }
    if (pointV == 6)
    {
        endGame = true;
        cout << "----- Game over. Viewer WIN!!! -----" << endl;
    }
}

int main()
{
    cout << "----- What? Where? When? -----\n" << endl;

    //Массив игрового поля true - сектор сыграл, false - еще нет
    bool field[13] = { };

    //Массив путей к файлам с вопросами
    string questions[] =   {"..\\questions\\Q01.txt", "..\\questions\\Q02.txt", "..\\questions\\Q03.txt",
                            "..\\questions\\Q04.txt", "..\\questions\\Q05.txt", "..\\questions\\Q06.txt",
                            "..\\questions\\Q07.txt", "..\\questions\\Q08.txt", "..\\questions\\Q09.txt",
                            "..\\questions\\Q10.txt", "..\\questions\\Q11.txt", "..\\questions\\Q12.txt",
                            "..\\questions\\Q13.txt"};

    //Массив путей к файлам с ответами
    string answers[] =     {"..\\answers\\A01.txt", "..\\answers\\A02.txt", "..\\answers\\A03.txt",
                            "..\\answers\\A04.txt", "..\\answers\\A05.txt", "..\\answers\\A06.txt",
                            "..\\answers\\A07.txt", "..\\answers\\A08.txt", "..\\answers\\A09.txt",
                            "..\\answers\\A10.txt", "..\\answers\\A11.txt", "..\\answers\\A12.txt",
                            "..\\answers\\A13.txt" };
    
    //Переменная текущего сектора
    int curSector = 1;

    //Флаг окончания игры
    bool endGame = false;

    //Переменные очков знатоков и телезрителей
    int pointCon = 0;
    int pointV = 0;

    while (!endGame)
    {
        spinning(field, curSector);
        cout << "Playing sector: " << curSector << endl << endl;

        //Печать вопроса выпавшего сектора
        question_print(questions, curSector, endGame);

        //Запрос ответа у знатоков
        string answerCon;
        cout << "The answer of connoisseurs: ";
        getline (cin, answerCon);

        //Если знатоки устали то по кодовому слову нужно выйти из игры с победой )))
        if (answerCon == "Victory") pointCon = 6;
        
        //Проверка ответа на вопрос
        else if (checking_answers(answers, answerCon, curSector, endGame))
        {
            cout << endl << "The answer is correct! Point goes to connoisseurs!\n" << endl;
            pointCon++;
        }
        else
        {
            cout << endl << "The answer is not correct! The score goes to the viewers!\n" << endl;
            pointV++;
        }

        round_result(pointCon, pointV, endGame);
    }
}
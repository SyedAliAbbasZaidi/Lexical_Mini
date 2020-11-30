#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <ctype.h>

using namespace std;

const int rows = 20;
const int cols = 25;

int Table[rows][cols];

void display()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << Table[i][j] << " ";
        }
        cout << endl;
    }
}

//----------------------------  is_alpha( )  ----------------------------//

bool is_alpha(char ch)
{

    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    {
        return true;
    }
    return false;
}

//----------------------------  is_digit()  ----------------------------//

bool is_digit(char ch)
{
    char digit[] = {"0123456789"};

    for (int i = 0; i < strlen(digit); i++)
    {
        if (digit[i] == ch)
        {
            return true;
        }
    }
    return false;
}

bool is_operator(char ch)
{
    char op[] = {"+-=<>,/*&%#!:;|"};
    for (int i = 0; op[i] != '\0'; i++)
    {
        if (ch == op[i])
            return true;
    }
    return false;
}

void get_TransitionTable()
{
    ifstream fin("Table.txt");

    if (!fin.is_open())
    {
        cout << "File Not Found." << endl;
        exit(0);
    }

    char input[100];

    for (int i = 0; i < rows; i++)
    {
        fin.getline(input, 100);

        char *temp_str = NULL;

        temp_str = strtok(input, " ");

        Table[i][0] = atoi(temp_str);

        for (int j = 1; j < cols; j++)
        {
            temp_str = strtok(NULL, " ");

            Table[i][j] = atoi(temp_str);
        }
    }

    fin.close();
}

//----------------------------  isKeyword( )  ----------------------------//

bool isKeyword(char *token)
{
    char keywords[32][10] = {"main", "else", "new", "bool", "endl", "true", "break",
                             "case", "false", "char", "float", "for", "return",
                             "const","short", "unsigned", "signed", "using", "if", "default", "void", "delete", "int",
                             "do", "long", "double", "switch", "while", "namespace", "std", "cin", "cout"};

    for (int i = 0; i < 32; i++)
    {
        if (strcmp(keywords[i], token) == 0)
            return true;
    }

    return false;
}

int get_NextState(int state, char ch)
{
    if (is_alpha(ch))
        return Table[state][0];

    else if (is_digit(ch))
        return Table[state][1];

    else if (ch == '+')
        return Table[state][2];

    else if (ch == '-')
        return Table[state][3];

    else if (ch == '*')
        return Table[state][4];

    else if (ch == '/')
        return Table[state][5];

    else if (ch == '{')
        return Table[state][6];

    else if (ch == '}')
        return Table[state][7];

    else if (ch == '(')
        return Table[state][8];

    else if (ch == ')')
        return Table[state][9];

    else if (ch == '[')
        return Table[state][10];

    else if (ch == ']')
        return Table[state][11];

    else if (ch == '_')
        return Table[state][12];

    else if (ch == '.')
        return Table[state][13];

    else if (ch == '=')
        return Table[state][14];

    else if (ch == '>')
        return Table[state][15];

    else if (ch == '<')
        return Table[state][16];

    else if (ch == '&')
        return Table[state][17];

    else if (ch == '!')
        return Table[state][18];

    else if (ch == '|')
        return Table[state][19];

    else if (ch == ':')
        return Table[state][20];

    else if (ch == '%')
        return Table[state][21];

    else if (ch == ';')
        return Table[state][22];

    else if (ch == '#')
        return Table[state][23];

    else if (ch == ',')
        return Table[state][24];

    return 0;
}

//-------------------------------  main( )  -----------------------------//

int main()
{
    get_TransitionTable();
    //display();
    ifstream fin("input.txt");
    ofstream fout("Token.txt");
    ofstream error("Error.txt");

    if (!fin.is_open())
    {
        cout << "File Not Found.\n"
             << endl;
        exit(0);
    }
    
    char token[255];
    int token_index = 0;

    char ch;
    int state = 1, flag = 0;

    do
    {
        if (fin.eof())
            return 0;

        if (flag == 0)
            fin >> ch;
        else
            flag = 0;

        state = get_NextState(state, ch);

        if (state == 2 || state == 4 || state == 5 || state == 6 || state == 8 || state == 9 || state == 10 || state == 11 || state == 12 || state == 13 || state == 14 || state == 15)
        {
            token[token_index] = ch;
            token_index++;
           
            token[token_index] = '\0';
            if (isKeyword(token))
            {
                fout << token << " is a <Keyword>" << endl;

                state = 1;
                token_index = 0;
            }
        }

        else if (state == 3)
        {

            token[token_index] = '\0';
            if (isKeyword(token))
                fout << token << " is a <Keyword>" << endl;

            else
                fout << token << " is a <Identifier>" << endl;

            state = 1;
            token_index = 0;
            flag = 1;
        }

        else if (state == 19)
        {
            token[token_index] = '\0';
            fout << token << " is a <Number>" << endl;
            state = 1;
            token_index = 0;
            flag = 1;
        }

        else if (state == 18)
        {
            token[token_index] = '\0';
            fout << token << " is a <Operator>" << endl;

            state = 1;
            token_index = 0;
            flag = 1;
        }

        else if (state == 17)
        {
            token[token_index] = ch;
            token[token_index + 1] = '\0';
            fout << token << " is a <Operator>" << endl;

            state = 1;
            token_index = 0;
        }
        else if (state == 16)
        {
            token[token_index] = ch;
            token[token_index + 1] = '\0';
            fout << token << " is a <Bracket>" << endl;

            state = 1;
            token_index = 0;
        }

        else
        {
            token[token_index] = ch;
            token[token_index + 1] = '\0';
            error << token << " is a <Invalid token>" << endl;
            state = 1;
            token_index = 0;
        }
    } while (!fin.eof());
    fin.close();
    fout.close();
    error.close();
    return 0;
}
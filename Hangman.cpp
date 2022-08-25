// this is a simple hangman game
// the special of this program -> it is resizable at any point
// whenever the terminal size is  changed, the game will always display in the center of the terminal
// for example when the terminal is changed to fullscreen or the terminal width is increased or decreased

// make sure you have downloaded words.txt and save it in the same file of this program
// from Hazrul Haiqal

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iomanip>
#include <thread>
#include <vector>
using namespace std;

namespace centerString // center a string in a given size
{
    string center(string str, int size, string a) // get 3 parameters - center(string,size,symbol)
    {
        while (str.length() != size)
        {
            str.insert(str.length(), a, 0, 1);

            if (str.length() == size)
                break;

            str.insert(0, a, 0, 1);
        }
        return str;
    }

    string center(string str, int size) // get 2 parameters - center(string,size)
    {
        while (str.length() != size)
        {
            str.insert(str.length(), " ", 0, 1);

            if (str.length() == size)
                break;

            str.insert(0, " ", 0, 1);
        }
        return str;
    }
}

// global variable
int X, Y;              // for coordinate
int terminalWidth = 0; // terminal width (Changing when the terminal screen width changed)
int twidth = 0;        // terminal width (fixed, to detect the changes of terminal screen width)
HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;
using namespace centerString;

void get_terminal_width() // get terminal width (multhithreading)
{
    while (1)
    {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        terminalWidth = (int)(csbi.srWindow.Right - csbi.srWindow.Left);
        Sleep(300);
    }
}

class Coordinate
{
public:
    void gotoxy(int x, int y) // move cursor to coordiante(x,y);
    {
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(hc, coord);
    }

    void getxy(int &x, int &y) // get cursor coordinate
    {
        GetConsoleScreenBufferInfo(hc, &csbi);
        x = csbi.dwCursorPosition.X;
        y = csbi.dwCursorPosition.Y;
    }

private:
    COORD coord;
} cursor;

void drawHangman() // draw hangman
{
    int twidth = terminalWidth;
    const char *hangman =
        R""""(
  +----+
  |    |
  O    |
 /|\   |
 / \   |
       |
==========
)"""";
    vector<string> tokens;
    string text = hangman;
    stringstream check(text);
    string draw;
    while (getline(check, draw, '\n'))
    {
        tokens.push_back(draw);
    }
    cursor.getxy(X, Y);

    for (int i = 0; i < tokens.size(); i++)
    {
        cursor.gotoxy(0, Y + i);
        cout << " " << center(tokens[i], twidth - 1, " ") << '\n';
    }
}

void displayScreen(int &choice)
{
Resize:
    SetConsoleTextAttribute(hc, 10);
    twidth = terminalWidth;
    cout << " +" << center("", twidth - 3, "-") << "+" << endl;
    cout << " |" << center(" ", twidth - 3) << "|" << endl;
    cout << " |" << center("WELCOME TO THE MOST AWESOME HANGMAN GAME :>", twidth - 3) << "|" << endl;
    cout << " |" << center("", twidth - 3) << "|" << endl;
    cout << " |" << center("CREATED BY : HAZRUL HAIQAL", twidth - 3) << "|" << endl;
    cout << " |" << center("", twidth - 3) << "|" << endl;
    cout << " +" << center("", twidth - 3, "-") << "+" << endl
         << endl
         << endl
         << endl
         << endl;

    drawHangman();

    cout << endl;

    string option1[] = {"Start", "How To Play", "Quit Game"};

    cout << " " << center("Use arrow Up and Down key to change selection", twidth - 1) << endl;
    cursor.getxy(X, Y);
    int flag = 0;
    int numchoice = 3;

    while (1)
    {
        cursor.gotoxy(X, Y);
        int input = 0;
        for (int j = 0; j < numchoice; j++)
        {
            if (choice == j)
            {
                SetConsoleTextAttribute(hc, 15);
                cout << " " << center("> " + option1[j], twidth - 1) << endl;
                continue;
            }
            SetConsoleTextAttribute(hc, 10);
            cout << " " << center(option1[j], twidth - 1) << endl;
        }
        SetConsoleTextAttribute(hc, 10);
    GetInput:
        int TIMEOUT = 1;
        clock_t tstart = clock();

        while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) // check if the terminal width is changed when getting input
        {
            if (kbhit())
            {
                input = getch();
                break;
            }
            else
            {
                if (twidth != terminalWidth)
                {
                    system("cls");
                    goto Resize;
                }
                else
                {
                    goto GetInput;
                }
            }
        }

        // check if the key was pressed (key_down)
        if ((GetAsyncKeyState(VK_UP) & 0x8000) && (flag == 0)) // arrow up
        {
            flag = 1;
            if (choice == 0)
            {
                choice = (numchoice - 1);
            }
            else
                choice--;
        }
        else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && (flag == 0)) // arrow down
        {
            flag = 1;
            if (choice == (numchoice - 1))
            {
                choice = 0;
            }
            else
                choice++;
        }
        else if ((GetAsyncKeyState(VK_RETURN) & 0x8000) && (flag == 0)) // return/enter key
        {
            return;
        }
        else if (GetAsyncKeyState(VK_UP) == 0 || GetAsyncKeyState(VK_DOWN) == 0 || GetAsyncKeyState(VK_RETURN) == 0) // check if the key was released (key up)
            flag = 0;                                                                                                // reset the flag
    }
}

void HowToPlay()
{
Resize:
    int twidth = terminalWidth;
    int input = 0;
    system("cls");
    cursor.gotoxy(0, 8);
    cout << center(" What...You don't  know how to play hangman?", twidth) << endl
         << endl;
    cout << center(" Here's how to play:", twidth - 1, " ") << endl;
    cout << center(" -Try to solve the riddle before your chances run out.", twidth - 1) << endl;
    cout << center(" -Type in the letters you wish to guess. Careful! Wrong letters lose chances.", twidth - 1) << endl
         << endl;
    cout << setw((twidth - 32) / 2) << " "
         << " Press any key to continue . . . ";

GetInput:
    int TIMEOUT = 1;
    clock_t tstart = clock();
    while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) // check if the terminal width is changed when getting input
    {
        if (kbhit())
        {
            input = getch();
            return;
        }
        else
        {
            if (twidth != terminalWidth)
            {
                goto Resize;
            }
            else
            {
                goto GetInput;
            }
        }
    }
}

void loading()
{
    int j = 0;
Resize:
    twidth = terminalWidth;
    int input = 0;
    system("cls");
    cout << "\n\n\n\n\n\n\n\n"
         << center(" This game have the same concept like word guessing.", terminalWidth) << endl;
    cout << center(" A random word will be choosen. Your task is to guess each letter of the word.", terminalWidth) << endl;
    cout << center(" You are only given 6 chances to guess the word right.", terminalWidth) << endl
         << "\n\n\n";
    cursor.getxy(X, Y);
    char x = 219;
    char y = 177;
    int percent;
    float k;
    cout << setw((twidth - 48) / 2) << " "
         << "Loading ";
    int x1, y1;
    cursor.getxy(x1, y1);
    while (j < 35) // laoding bar
    {
        if (twidth != terminalWidth) // if terminal width was changed during loading
        {
            goto Resize;
        }
        cursor.gotoxy(x1, y1);

        k = j;
        percent = k / 34 * 100;
        for (int m = 0; m < j; m++)
        {
            cout << x;
        }
        for (int i = 34 - j; i > 0; i--)
        {
            cout << y;
        }
        cout << " " << percent << "%";
        if (j < 10)
            Sleep(300);
        if (j >= 10 && j < 20)
            Sleep(150);
        if (j >= 20 && j < 34)
            Sleep(25);
        if (j == 34)
            Sleep(300);
        j++;
    }
    cout << "\r" << setw((twidth - 32) / 2) << " "
         << " Press any key to continue . . .       ";
    cout << "\b\b\b\b\b\b";
Pause:
    int TIMEOUT = 1;
    clock_t tstart = clock();
    while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) // check if the terminal width is changed when getting input
    {
        if (kbhit())
        {
            input = getch();
            break;
        }
        else
        {
            if (twidth != terminalWidth)
            {
                goto Resize;
            }
            else
            {
                goto Pause;
            }
        }
    }
    system("cls");
}

void game()
{
    int trial = 6;
    char input;
    bool CorrectGuess = false;
    char PreviousGuess[6];
    bool win = false;
    for (int i = 0; i < 6; i++)
    {
        PreviousGuess[i] = '-';
    }
    string PGuess = PreviousGuess;
    string word;
    string wordlist[45];
    const char *hanger =
        R""""(
  +----+
  |    |
       |
       |
       |
       |
==========
)"""";

    vector<string> tokens2;
    string text2 = hanger;
    stringstream check2(text2);
    string draw2;
    while (getline(check2, draw2, '\n'))
    {
        tokens2.push_back(draw2);
    }

    srand(time(NULL));
    ifstream RandomWord;
    RandomWord.open("words.txt");

    if (RandomWord.fail())
    {
        cout << center(" Cannot find file words.txt. Please make sure you have downloaded it and save it in the same folder.", twidth, " ") << endl;
        exit(1);
    }

    for (int i = 0; i < 45; i++)
    {
        RandomWord >> wordlist[i];
    }
    int Random = rand() % 45;
    word = wordlist[Random];
    RandomWord.close();

    string MysteryWord(word.length(), '*');

Resize:
    twidth = terminalWidth;
    system("cls");
    while (trial >= 0) // insert a character into hanger
    {
        switch (trial)
        {
        case 5:
        {
            tokens2[3].erase(2, 1);
            tokens2[3].insert(2, "O");
            break;
        }
        case 4:
        {
            tokens2[4].erase(2, 1);
            tokens2[4].insert(2, "|");
            break;
        }
        case 3:
        {
            tokens2[4].erase(1, 1);
            tokens2[4].insert(1, "/");
            break;
        }
        case 2:
        {
            tokens2[4].erase(3, 1);
            tokens2[4].insert(3, "\\");
            break;
        }
        case 1:
        {
            tokens2[5].erase(1, 1);
            tokens2[5].insert(1, "/");
            break;
        }
        case 0:
        {
            tokens2[5].erase(3, 1);
            tokens2[5].insert(3, "\\");
            break;
        }
        }

        CorrectGuess = false;
        bool sameletter = false;

        if (trial >= 0)
        {
            cursor.gotoxy(0, 1);
            cout << center(" Quit (ESC)", twidth);

            if (word == MysteryWord) // if all letters is true
            {
                cursor.gotoxy(0, 1);
                SetConsoleTextAttribute(hc, 15);
                cout << center(" Congratulations, you've guess the correct word which is " + MysteryWord, twidth) << endl;
                cout << center(" Don't be proud. It's an easy word even my grandmother can guess it.", twidth) << endl;
                win = true;
            }

            if (trial == 0) // if there's no more trials
            {
                cursor.gotoxy(0, 1);
                SetConsoleTextAttribute(hc, 4);
                cout << center(" Game Over! I know you're a noob. But don't worry.", twidth) << endl;
                cout << center(" You can try your luck again next time (-_^)", twidth) << endl;
                cout << center(" The correct answer is " + word + ".", twidth);
            }

            cursor.gotoxy(0, 5);

            for (int i = 0; i < tokens2.size(); i++) // draw hanger
            {
                cursor.gotoxy(0, 5 + i);
                cout << " " << center(tokens2[i], twidth, " ") << '\n';
            }

            cursor.getxy(X, Y);

            cursor.gotoxy(X, Y + 2);

            cout << center(" The animal you have to guess is :", twidth) << endl
                 << endl;
            cout << center(MysteryWord, twidth, " ") << endl;
            cout << center(" You have " + to_string(trial) + " guesses left", twidth) << endl;
            if (trial != 6)
            {
                PGuess = PreviousGuess;
                cout << center(" You have guessed : " + PGuess, twidth) << endl;
            }
            else
                cout << endl;

            cout << endl;

            int X1, Y1;
            cursor.getxy(X1, Y1);
        Reenter:
            if (trial > 0 && !win)
            {
                cursor.gotoxy(X1, Y1);
                cout << setw((twidth - 18) / 2) << " "
                     << " Guess a letter :         ";
                cout << "\b\b\b\b\b\b\b\b";
            }
            else if (trial == 0 || win)
            {
                cursor.gotoxy(X1, Y1);
                cout << setw((twidth - 32) / 2) << " "
                     << " Press any key to continue . . . ";
            }

        GetInput:
            int TIMEOUT = 1;
            clock_t tstart = clock();
            while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) // check if the terminal width is changed when getting input
            {
                if (kbhit())
                {
                    input = getch();
                    break;
                }
                else
                {
                    if (twidth != terminalWidth)
                    {
                        goto Resize;
                    }
                    else
                    {
                        goto GetInput;
                    }
                }
            }

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) // if quit (ESC key pressed)
            {
                return;
            }

            if (trial > 0 && !win)
            {
                if (isalpha(input)) // if input is an alphabet
                {
                    input = toupper(input);
                }
                else if (isdigit(input)) // if input is a digit
                {
                    cout << "\r" << center(" Please enter an alphabet!", twidth);
                    cursor.gotoxy(X1, Y1);
                    Sleep(1000);
                    goto Reenter;
                }
                else // if input is a symbol or an arrow key
                {
                    cout << "\r" << center(" Please enter an alphabet!", twidth);
                    Sleep(1000);
                    cursor.gotoxy(X1, Y1);
                    if (input == 0 || GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN))
                    { // to fix getting two value when pressed arrow key
                        getch();
                    }
                    goto Reenter;
                }

                string guess(1, input);

                for (int i = 0; i < MysteryWord.length(); i++)
                {
                    if (input == MysteryWord[i])
                    {
                        sameletter = true;
                        break;
                    }
                    else if (word[i] == input)
                    {
                        MysteryWord[i] = input;
                        CorrectGuess = true;
                    }
                }

                if (CorrectGuess == false)
                {
                    for (int i = 0; i < 6 - trial; i++)
                    {
                        if (input == PreviousGuess[i])
                        {
                            sameletter = true;
                            break;
                        }
                    }
                    cursor.gotoxy(X1, Y1);

                    if (!sameletter)
                    {
                        cout << center(" Sorry, " + guess + " is not part of the word", twidth);
                        PreviousGuess[6 - trial] = input;
                        PGuess = PreviousGuess;
                        trial--;
                        Sleep(1200);
                    }
                }
                else
                {
                    if (!sameletter)
                    {
                        cursor.gotoxy(X1, Y1);
                        cout << center(" Good! " + guess + " is one of the letters!", twidth);
                        Sleep(1200);
                    }
                }

                if (sameletter)
                {
                    cursor.gotoxy(X1, Y1);
                    cout << center(guess + " is already guessed", twidth);
                    Sleep(1200);
                }
            }
            else
            {
                break;
            }
        }
    }
}

void quit()
{
    system("title Quit");
    system("cls");
Resize:
    twidth = terminalWidth;
    int input = 0;
    cursor.gotoxy(0, 9);
    cout << " " << center("Thanks for using my program <3", terminalWidth - 1) << endl;
    cout << setw((terminalWidth - 24) / 2) << " "
         << "See you next time (^-^)";
GetInput:
    int TIMEOUT = 1;
    clock_t tstart = clock();
    while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) // check if the terminal width is changed when getting input
    {
        if (kbhit())
        {
            input = getch();
            break;
        }
        else
        {
            if (twidth != terminalWidth)
            {
                goto Resize;
            }
            else
            {
                goto GetInput;
            }
        }
    }

    cursor.gotoxy(0, 18);
}

int main()
{
    SetConsoleTextAttribute(hc, 10);
    int choice = 0;
    thread t(get_terminal_width);

    while (1)
    {
        if (terminalWidth != 0)
        {
            system("title Hangman Game");
            SetConsoleTextAttribute(hc, 10);
            displayScreen(choice);

            if (choice == 0) // start
            {
                loading();
                game();
            }
            else if (choice == 1) // how to play
            {
                system("title How to Play");
                HowToPlay();
            }
            else if (choice == 2) // quit
            {
                quit();
                exit(0); // end program
            }
            system("cls");
        }
    }
    t.join();

    return 0;
}
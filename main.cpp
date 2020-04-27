#include <iostream>
#include <cstdlib>
#include <string>
#include "classes.h"
#include "parsing.h"
#include "CTransform.h"

using namespace std;


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Not enough args...\nYou must enter something like this 'lanceur 0 afd1.txt mots1.txt sortie1.txt'\n";
        return -1;
    }
    string s_automate(argv[1]);
    string s_words(argv[2]);
    string s_output(argv[3]);
    CParse Parser(s_automate, s_words, s_output);
    CAutomate* a = Parser.AutomateParser();
    CTransform t;
    if (a)
        t.Set_Automate(a);
    if(!t.IsDeterminist())
        t.SolveUndeterminized(Parser.WordParser(), s_output);
    //std::vector<string> words = Parser.WordParser();
    return 0;//a->SolveWords(words, s_output);

}
#include "parsing.h"
#include <fstream>


using namespace std;
CParse::CParse()
{
    m_afd_path = "undefined";
    m_word_path = "undefined";
    m_out_path = "undefined";
    m_nbState = -1;
}

CParse::CParse(std::string p_afdpath, std::string p_wordpath, std::string p_outpath)
{
    m_afd_path = p_afdpath;
    m_word_path = p_wordpath;
    m_out_path = p_outpath;
    m_nbState = -1;
    m_FILE = NULL;
}

CAutomate* CParse::AutomateParser()
{
    int i, j;
    CAutomate* tmp_Automate = new CAutomate();
    CState* tmp_State = NULL;
    CTransition* tmp_Transition = NULL;
    std::ifstream readFile(m_afd_path.c_str(), ios_base::in);
    if (!readFile.is_open())
        return NULL;
    int nbtransition, nb_state=0, transition_num;
    string dictionnary, tmp_string;
    bool init, finish;
    char transition_label;   //To access the desired characters
    readFile >> dictionnary;  //Adding dictionnary
    tmp_Automate->Set_Dictionnary(dictionnary);
    readFile >> nb_state;
    for (i = 0; i < nb_state; i++) //Boucle des états de l'automate
    {
        tmp_State = new CState();
        readFile >> init >> finish;  //State
        //tmp_char = _strdup(state_SE.c_str());   //To access both states on the same line
        tmp_State->Set_Accepting(finish);
        tmp_State->Set_Initial(init);
        tmp_State->Set_State(i);
        readFile >> nbtransition;
        for (j = 0; j < nbtransition; j++)//Boucle des transition par état
        {
            tmp_Transition = new CTransition();
            readFile >> transition_num >> transition_label; //Transitions
            tmp_Transition->Set_Label(transition_label);
            tmp_Transition->Set_Next(transition_num);
            tmp_State->AddTransition(tmp_Transition);
        }
        tmp_Automate->AddNode(tmp_State);
    }
    for(i = 0; i < nb_state; i++)   //Pour chaque état
        for (j = 0; j < tmp_Automate->Get_Node(i)->Get_nbTransition(); j++) //Pour chaque transition de chaque état
        {
            int tmp_next_state = tmp_Automate->Get_Node(i)->Get_Transition(j)->Get_nb_next();
            tmp_Automate->Get_Node(i)->Get_Transition(j)->Set_Transition(tmp_Automate->Get_Node(tmp_next_state));   //On fais pointer la transition sur l'état
        }
    readFile.close();
    return tmp_Automate;
}

vector<string> CParse::WordParser()
{
    string tmp;
    vector<string> wordlist;
    std::ifstream readFile(m_word_path.c_str(), ios_base::in);
    while (getline(readFile, tmp))
        wordlist.push_back(tmp);
    return wordlist;
}
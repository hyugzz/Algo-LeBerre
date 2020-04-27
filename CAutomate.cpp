#include "classes.h"
#include "parsing.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#define POINTER_ERROR 3

CAutomate::CAutomate()  //Constructeur par défaut
{
}

CAutomate::CAutomate(const char* p_alphabet, CState** p_nodes)
{
    int i = 0;
    m_alphabet = p_alphabet;
    while (p_nodes[i])
    {
        m_nodes.push_back(p_nodes[i]);
        i++;
    }
}

CState* CAutomate::Get_Node(int p_node) //Retourne un pointeur vers le noeud désiré
{
    if (p_node < Get_nbNodes())
        return m_nodes[p_node];
    else
    {
        std::cout << "Vector dépassement" << std::endl;
        return NULL;
    }
}

void CAutomate::AddNode(CState* p_node)
{
    m_nodes.push_back(p_node);
    return;
}

int CAutomate::SolveWords(std::vector<std::string> p_words, std::string path)
{
    unsigned int j, i = 0;
    bool IsValid = false;
    CState** currentState = new CState*(StartingState());
    if (currentState == NULL)
        return POINTER_ERROR;
    std::ofstream outfile(path.c_str());
    if (!outfile.is_open())
    {
        return FILE_ERROR;
    }
    for (i = 0; i < p_words.size(); i++) //Pour chaque mot
    {
        (*currentState) = StartingState();
        for (j = 0; j < p_words[i].length(); j++)  //Pour chaque lettre du mot
        {
            if((*currentState)->IsCharInTransitions(p_words[i][j], currentState))
            {
                if ((j == p_words[i].length()-1) && ((*currentState)->Get_Accepting() == 1))
                {
                    IsValid = true;
                }
                else
                    IsValid = false;
            }
            else
            {
                IsValid = false;
                j = p_words[i].length();
            }
        }
        if (IsValid == true)
           std::cout << "1\n";
        if(IsValid == false)
           std::cout << "0\n";
    }
    std::cout << "Automation done!\n";
    return 0;
}

CState* CAutomate::StartingState()
{
    for (unsigned int i = 0; i < m_nodes.size(); i++)
    {
        if (m_nodes[i]->Get_Initial() == true)
            return m_nodes[i];
    }
    return NULL;
}

bool CAutomate::IsWordInDictionnary(std::string p_word)
{
    return true;
}

void CAutomate::Set_Dictionnary(std::string p_dictionnary)
{
    m_alphabet = p_dictionnary;
}

unsigned int CAutomate::Get_nbNodes()
{
    return m_nodes.size();
}

std::string CAutomate::Get_Dictionnnary()
{
    return m_alphabet;
}

#include <iostream>
#include <fstream>
#include <algorithm>
#include "CTransform.h"

#define FILE_ERROR 1
#define ALLOCATE_ERROR 2

#define POINTER_ERROR 3

CTransform::CTransform()
{
	m_automate = NULL;
}

CTransform::CTransform(CAutomate* p_automate)
{
	m_automate = p_automate;
}

void CTransform::Set_Automate(CAutomate* p_automate)
{
	m_automate = p_automate;
}

CAutomate* CTransform::Get_Automate()
{
	return m_automate;
}

bool CTransform::IsDeterminist()
{
	int* tmp_count = (int*)malloc(m_automate->Get_Dictionnnary().size() * sizeof(int));
	int nbinit=0, i, j, k=0;
	for (i = 0; i < m_automate->Get_nbNodes(); i++)	//Pour chaque noeud
	{

		if (m_automate->Get_Node(i)->Get_Initial())
		{
			nbinit++;
			if (nbinit > 1)
				return false;
		}
		for (j = 0; j < m_automate->Get_Node(i)->Get_nbTransition(); j++)	//Pour chaque transition de chaque noeud
		{
			for (k = 0; k < m_automate->Get_Dictionnnary().size(); k++)	//Pour chaque lettre du dictionnaire
				if (m_automate->Get_Node(i)->Get_Transition(j)->Get_Label() == m_automate->Get_Dictionnnary().at(k))	//Si transition[i].label = dico[k]
				{
					tmp_count[k]++;		//On ajoute l'occurence
				}
			k = 0;	//Réintialisation de l'indice
		}
		for(k=0; k < m_automate->Get_Dictionnnary().size(); k++)
		{
			if (tmp_count[k] > 1)	//Si plusieurs transitions ont le même label
				return false;
			tmp_count[k] = 0;	// Réinitialisation du tableau
		}
		k = 0;	//Réintialisation de l'indice
	}
	return true;
}

int CTransform::SolveUndeterminized(std::vector<std::string> p_words, std::string path)
{
	unsigned int j, k, i = 0, nb_currentstate;
	bool IsValid = false;
	std::vector<CState**> currentState; //new CState*(m_automate->StartingState());
	currentState.push_back(new CState * (m_automate->StartingState()));
	if (currentState.size() == 0)
	{
		return POINTER_ERROR;
	}

	std::ofstream outfile(path.c_str());
	if (!outfile.is_open())
	{
		return FILE_ERROR;
	}
	for (i = 0; i < p_words.size(); i++) //Pour chaque mot
	{
		currentState.clear();
		currentState.push_back(new CState*(m_automate->StartingState()));
		for (j = 0; j < p_words[i].length(); j++)  //Pour chaque lettre du mot
		{
			nb_currentstate = currentState.size();
			for (k = 0; k < nb_currentstate; k++)
			{
				/*if (currentState.empty())
				{
					IsValid = false;
					k = nb_currentstate;
					j = p_words[i].length();
				}*/
				if ((*currentState[k])->IsCharInTransitions(p_words[i][j], currentState, k))
				{
					if ((j == p_words[i].length() - 1) && ((*currentState[k])->Get_Accepting() == 1))	//Si on termine sur un état acceptant
					{
						IsValid = true;
						k = nb_currentstate;//Si une case valide c'est bon
					}
					else
						IsValid = false;
				}
				else
				{
					IsValid = false;
				}
			}
			currentState.erase(currentState.begin(), currentState.begin() + k);
		}
		/*****************************************/
		std::cout << i + 1 << " : ";
		if (IsValid == true)
			std::cout << "1\n";
		if (IsValid == false)
			std::cout << "0\n";
	}
	std::cout << "Automation done!\n";
	return 0;
}

CAutomate* CTransform::Determinize()
{
	return NULL;
}

CAutomate* CTransform::Reduce()
{
	CAutomate* minimal = new CAutomate();
	unsigned int i, k, j;
	int ***class_tab = TabAlloc(m_automate->Get_Dictionnnary().size(),  m_automate->Get_nbNodes());	//Le tableau récapitulatif des transitions en fonction du numéro de label

	int* tmp_new_class[2];	//Les classes de chaque noeud
	for (i = 0; i < 2; i++)
		tmp_new_class[i] = (int*)calloc(m_automate->Get_nbNodes(), sizeof(int));
	for (i = 0; i < 2; i++)
		for (j = 0; j < m_automate->Get_nbNodes(); j++)
			tmp_new_class[i][j] = m_automate->Get_Node(j)->Get_Accepting();
	std::vector<std::string> ClassStorer;
	for (j = 0; j < m_automate->Get_Dictionnnary().size(); j++)	//Initialisation du "tableau de transitions" [0]
		for (k = 0; k < m_automate->Get_nbNodes(); k++)
		{
			if (m_automate->Get_Node(k) && m_automate->Get_Node(k)->Get_Transition(j))
			{
				class_tab[0][k][j] = m_automate->Get_Node(k)->Get_Transition(j)->Get_nb_next();
			}
			else
				class_tab[0][j][k] = -1;
		}

	for (j = 0; j < m_automate->Get_Dictionnnary().size(); j++)	//Initialisation du tableau de classes [1]
		for (int k = 0; k < m_automate->Get_nbNodes(); k++)
		{
			if (m_automate->Get_Node(k) && m_automate->Get_Node(k)->Get_Transition(j))
			{
				if (m_automate->Get_Node(k)->Get_Transition(j)->Get_Next()->Get_Accepting())	//Si la transition pointe sur un état acceptant
					class_tab[1][k][j] = 1;
				else
					class_tab[1][k][j] = 0;
			}
			else
				class_tab[1][j][k] = -1;
		}
	/*****************INITIALIZED*********************/
	i = 0;
	std::cout << "Tableau de reference [0]\n";
	PrintTab(class_tab[0]);
	std::cout << "Tableau classes initialisé [1]\n";
	PrintTab(class_tab[1]);
	do //Tant que le truc est pas deux fois le même, donc minim
	{
		for (j = 0; j < m_automate->Get_nbNodes(); j++)
			tmp_new_class[i][j] = WhichClass(class_tab[1][j], m_automate->Get_Dictionnnary().size(), tmp_new_class[(i+1)%2][j], &ClassStorer);	//On mets les nouvelles classes
		class_tab = UpdateTab(class_tab, m_automate->Get_Dictionnnary().size(), m_automate->Get_nbNodes(), tmp_new_class[i]);
		ClassStorer.clear();
		i = (i+1)%2;
		PrintTab(class_tab[1]);
		std::cout << "\n\n\n";
	} while (!CompareTab(tmp_new_class, m_automate->Get_nbNodes()));

	std::cout << "\tREDUCED\n" << std::endl;
	return NULL;
}

int*** CTransform::UpdateTab(int*** p_tab, int p_len, int p_height, int* p_list)
{
	int i, j, k;
	for (i = 0; i < p_len; i++)
		for (j = 0; j < p_height; j++)// Pour chaque case du tableau
			for (k = 0; k < p_len; k++)	//Pour chaque numéro de noeud
				if (p_tab[0][i][j] == k)
					p_tab[1][i][j] = p_list[p_tab[1][i][j]];	//Le noeud est remplacé par sa classe
	return p_tab;
}

int*** CTransform::TabAlloc(unsigned int nb_node, unsigned int dic_size)	//C'est good, ne pas faire attention à l'avertissement visual est bourré
{
	unsigned int i, j;
	int*** class_tab = (int***)malloc(2 * sizeof(int**));
	for (i = 0; i < 2; i++)
		class_tab[i] = (int**)malloc(dic_size * sizeof(int*));
	for (i = 0; i < 2; i++)
		for (j = 0; j < dic_size; j++)
			class_tab[i][j] = (int*)calloc(nb_node, sizeof(int));
	return class_tab;
}

int CTransform::WhichClass(int* p_line, int p_line_size, int p_transition, std::vector<std::string>* p_list)	//Retourne le numéro de classe de la ligne
{
	int i=0;
	std::string tmp = LineToString(p_line, p_line_size);
	tmp.append(std::to_string(p_transition));
	for (i = 0; i < p_list->size(); i++)
	{
		if ((*p_list)[i].compare(tmp.c_str()) == 0)	//Si la ligne correspond à une classe déjà existante
			return i;	//On retourne le numéro de la classe
	}
	p_list->push_back(tmp);	//Sinon on crée une nouvelle classe
	return i;	//Et on retourne son indice
}

std::string CTransform::LineToString(int* p_line, int p_line_size)
{
	std::string line;

	for (int i = 0; i < p_line_size; i++)
	{
		line += p_line[i];
		line += " ";
	}
	return line;
}

void CTransform::PrintTab(int** p_tab)
{
	int j,k;
	std::cout << "\t";
	for (k = 0; k < m_automate->Get_nbNodes(); k++)
		std::cout << k << "\t";
		std::cout << std::endl;
	for (j = 0; j < m_automate->Get_Dictionnnary().size(); j++)
	{
		std::cout << m_automate->Get_Dictionnnary()[j] << "\t";
		for (k = 0; k < m_automate->Get_nbNodes(); k++)
		{
			std::cout << p_tab[k][j] << "\t";
		}
		std::cout << std::endl;
	}
	return;
}

bool CTransform::CompareTab(int** p_tab, int p_tab_size)
{
	for (int i = 0; i < p_tab_size; i++)
		if (p_tab[0][i] != p_tab[1][i])
			return false;
	return true;
}

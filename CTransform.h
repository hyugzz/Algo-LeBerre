#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "classes.h"
#include "parsing.h"
#include <string>


class CAutomate;

class CTransform
{
private:
	CAutomate* m_automate;
public:
	CTransform();
	CTransform(CAutomate* p_automate);
	/*********************************/
	void Set_Automate(CAutomate* p_automate);
	CAutomate * Get_Automate();
	/*********************************/
	bool IsDeterminist();	//Pas sur a 100%
	int SolveUndeterminized(std::vector<std::string> p_words, std::string path);
	CAutomate* Determinize();
	CAutomate* Reduce();
	int*** TabAlloc(unsigned int nb_node, unsigned int dic_size);	//Génere le tableau pour la réduction
	int WhichClass(int* p_line, int p_line_size, int p_transition, std::vector<std::string>* p_list);	//Returns the index of the corresponding class in the forward_list
	std::string LineToString(int* p_line, int p_line_size); //Returns the line into a string
	/******************************************/
	void PrintTab(int ** p_tab);
	bool CompareTab(int** p_tab, int p_tab_size);
	int*** UpdateTab(int*** p_tab, int p_len, int p_height, int* p_list);
	/******************************************/
	void GenerateAutomate(std::vector<std::string>* p_nodes);
};

#endif // !TRANSFORM_H

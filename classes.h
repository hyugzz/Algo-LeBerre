#ifndef CLASS_H
#define CLASS_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#define FILE_ERROR 1
#define ALLOCATE_ERROR 2

class CState {
private:
    bool m_accepting;  //Etat acceptant ?
    bool m_initial;  //Etat initial ?
    int m_state; //Numéro d'état
    std::vector<class CTransition*> m_transition;  //Liste des transitions de l'état
public:
    CState();
    CState(bool p_init, bool p_accept, int p_state, class CTransition** p_next);
    bool Get_Accepting();
    bool Get_Initial();
    int Get_State();
    int Get_nbTransition();
    class CTransition* Get_Transition(int p_transition);
    void Set_State(int p_state);
    void Set_Accepting(bool p_state);
    void Set_Initial(bool p_state);
    void AddTransition(CTransition* p_node);
    bool IsCharInTransitions(char p_char, CState** io_state);
    bool IsCharInTransitions(char p_char, std::vector<CState**> &io_state, int curr_node);
};

class CTransition {
private:
    char m_label; //Label de la transition
    int m_next;  //Numéro du noeud correspondant à la transition
    CState* m_state; //Pointeur vers le noeud correspondant à la transition
public:
    CTransition();
    CTransition(char p_label, int p_next);
    char Get_Label();
    int Get_nb_next();  //Returns the node index
    CState* Get_Next();
    void Set_Label(char p_label);
    void Set_Transition(CState* p_Node);
    void Set_Next(int p_next);
};

class CAutomate {
private:
    std::string m_alphabet;  //Alphabet de l'automate
    std::vector<CState*> m_nodes; //Tableau de pointeur sur noeuds
public:
    CAutomate();  //Constructeur par défaut
    CAutomate(const char* p_alphabet, CState** p_nodes); //"path" = Le chemin vers le fichier de l'automate
    CState* Get_Node(int p_node); //Retourne un pointeur vers le noeud désiré
    unsigned int Get_nbNodes();  //Retourne le nombre de noeuds de l'automate
    void AddNode(CState* p_node);
    int SolveWords(std::vector<std::string> p_words, std::string path);
    bool IsWordInDictionnary(std::string p_word);
    void Set_Dictionnary(std::string p_dictionnary);
    std::string Get_Dictionnnary();
    CState* StartingState(); //Cherche le premier état initial dans le vecteur de CState

};

#endif
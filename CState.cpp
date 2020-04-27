#include "classes.h"
#include <iostream>

CState::CState()
{
    m_accepting = false;
    m_initial = false;
    m_state = -1;
}

CState::CState(bool p_init, bool p_accept, int p_state, class CTransition** p_next)
{
    int i = 0;
    m_initial = p_init;
    m_accepting = p_accept;
    m_state = p_state;
    while (p_next[i])
    {
        m_transition.push_back(p_next[i]);
        i++;
    }
}

bool CState::Get_Accepting()
{
    return m_accepting;
}

bool CState::Get_Initial()
{
    return m_initial;
}

int CState::Get_State()
{
    return m_state;
}

void CState::Set_Accepting(bool p_state)
{
    m_accepting = p_state;
}

void CState::Set_Initial(bool p_state)
{
    m_initial = p_state;
}

void CState::AddTransition(CTransition* p_node)
{
    m_transition.push_back(p_node);
    return;
}

bool CState::IsCharInTransitions(char p_char, CState** io_state)
{
    for (unsigned int i = 0; i < (*io_state)->m_transition.size(); i++)
    {
        if ((*io_state)->m_transition[i]->Get_Label() == p_char)
        {
            (*io_state) = (*io_state)->m_transition[i]->Get_Next();
            return true;
        }
    }
    return false;
}

void CState::Set_State(int p_state)
{
    m_state = p_state;
}

int CState::Get_nbTransition()
{
    return m_transition.size();
}

class CTransition* CState::Get_Transition(int p_transition)
{
    return m_transition[p_transition];
}

bool CState::IsCharInTransitions(char p_char, std::vector<CState**>& io_state, int curr_node)
{
    int nb_state = (*io_state[curr_node])->Get_nbTransition();
    bool IsTrue = false;
    for (unsigned int i = 0; i < nb_state; i++)
    {
        if ((*io_state[curr_node])->Get_Transition(i)->Get_Label() == p_char)
        {
            io_state.push_back((new CState*((*io_state[curr_node])->Get_Transition(i)->Get_Next())));
            IsTrue = true;
        }
    }
    if (nb_state == 0)//|| !(*io_state[curr_node])->Get_Accepting())
    {
        //io_state.push_back((new CState * ((*io_state[curr_node]))));
        IsTrue = true;
    }
   /* else
        io_state.erase(io_state.begin() + curr_node);*/ //Faux car ça décale les indices
    return IsTrue;
}
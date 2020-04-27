#include "classes.h"

CTransition::CTransition()
{
    m_label = 0;
    m_next = -1;
    m_state = NULL;
}

CTransition::CTransition(char p_label, int p_next)
{
    m_label = p_label;
    m_next = p_next;
    m_state = NULL;
}

char CTransition::Get_Label()
{
    return m_label;
}

int CTransition::Get_nb_next()
{
    return m_next;
}

CState* CTransition::Get_Next()
{
    return m_state;
}

void CTransition::Set_Label(char p_label)
{
    m_label = p_label;
    return;
}

void CTransition::Set_Transition(CState* p_Node)
{
    m_state = p_Node;
}

void CTransition::Set_Next(int p_next)
{
    m_next = p_next;
}
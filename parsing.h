#ifndef PARSING_H
#define PARSING_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "classes.h"

class CParse {
private:
    std::string m_afd_path;
    std::string m_word_path;
    std::string m_out_path;
    int m_nbState;
    FILE* m_FILE;
public:
    CParse();
    CParse(std::string p_afdpath, std::string p_wordpath, std::string p_outpath);
    CAutomate* AutomateParser();
    std::vector<std::string> WordParser();
};

#endif
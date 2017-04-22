#ifndef READER
#define READER


#include"header.hpp"
#include"ASM_Table.hpp"


class Reader{
private:
    int TabSpace;
    int lines;
    string cmd;
    string label;
    string action;
    string action_old;
    string data;
    string comment;
    int type;
    fstream file;
    void clear_status();
    string replaceTab(string);
    string toUpper(string);
    string removeHeaderSpace(string);

public:
    Reader(string,int);
    ~Reader();
    bool nextLine();
    AsmCode* get_AsmCode();
};

#endif

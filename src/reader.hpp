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
    TYPE type;
    fstream file;
    void clear_status();
    string replaceTab(string);
    string toUpper(string);
    string removeHeaderSpace(string);
    string version;
    int ni;
public:
    Reader(string,int,string);
    ~Reader();
    bool nextLine();
    AsmCode* get_AsmCode();
};

#endif

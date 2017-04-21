#ifndef READER
#define READER


#include<fstream>
#include<iostream>
#include<string>
#include"ASM_Table.hpp"
using std::cout;
using std::string;
using std::fstream;

class Reader{
private:
    int lines;
    string cmd;
    string label;
    string action;
    string action_old;
    string data;
    string comment;
    int type;
    fstream file;
    AsmTable*AsmTAB;
    void clear_status();
    string replaceTab(string);
    string toUpper(string);
    string removeHeaderSpace(string);
public:
    Reader(string,AsmTable*);
    ~Reader();
    bool nextLine();
    AsmCode* get_AsmCode();
};

#endif

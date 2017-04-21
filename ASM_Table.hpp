#ifndef ASM_TABLE
#define ASM_TABLE

#include<fstream>
#include<string>
#include<sstream>
#include<cstring>
#include<cstdio>
#include<iostream>
#include<iomanip>
using std::fstream;
using std::stringstream;
using std::strlen;
using std::sprintf;
using std::string;
using std::cout;
using std::endl;
using std::setw;
using std::getline;
class AsmCode{

public:
    AsmCode*next;
    class ErrMessage{
        private:
            string Messenge;
            ErrMessage*next;
        public:
            void set_next(ErrMessage*);
            void printErrMes(fstream&);
            ErrMessage(string);
            ~ErrMessage();
    };
    AsmCode(int,string,string,string,string,int,string);
    //AsmCode(int,string,string,string,string,int);
    void set_Address(unsigned int);
    void add_ErrMes(string);
    void set_next(AsmCode*);
    void set_objcode(string);
    void set_objcode(unsigned int);
    int get_type();
    unsigned int get_Address();
    string get_label();
    string get_opcode();
    string get_data();
    string get_objcode();
    string toString();
    ~AsmCode();
    bool has_ErrMes();
    void printErrMes(fstream&);
    void printData();
private:
    int type;
    string lable;
    string opcode;      //upper
    string opcode_old;
    string data;
    string commit;
    int line;
    unsigned int address;
    string objcode;
    ErrMessage*head;
};

class AsmTable{
    private:
        AsmCode*head;
        AsmCode*find_end();
        int lines;
    public:
        AsmCode*findLine(int);
        AsmTable();
        ~AsmTable();
        void append(AsmCode*);
        void printData();
        int get_lines();
};

#endif // ASM_TABLE

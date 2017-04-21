#ifndef OPCODE_TABLE
#define OPCODE_TABLE


#include<string>
#include<cctype>
using std::string;
using std::toupper;

class Opcode{
private:
    typedef struct{
        unsigned char opcode;
        string symbol;
        unsigned int xe_format;
        //xe_format only for sic/xe
        //sic always use 3byte
        //1=1byte 2=2byte 3=3byte 4=4byte
        //5= 3/4 byte
    }Data;

    Data  code[26]={
        {0x18,"ADD",5},{0x40,"AND",5},
        {0x28,"CMOP",5},{0x24,"DIV",5},
        {0x3c,"J",5},{0x30,"JEQ",5},
        {0x34,"JGT",5},{0x38,"JLT",5},
        {0x48,"JSUB",5},{0x00,"LDA",5},
        {0x50,"LDCH",5},{0x08,"LDL",5},
        {0x04,"LDX",5},{0x20,"MUL",5},
        {0x44,"OR",5},{0xd8,"RD",5},
        {0x4c,"RSUB",5},{0x0c,"STA",5},
        {0x54,"STCH",5},{0x14,"STL",5},
        {0xe8,"STSW",5},{0x10,"STX",5},
        {0x1c,"SUB",5},{0xe0,"TD",5},
        {0x2c,"TIX",5},{0xdc,"WD",5}
    };

    string upper(string);

public:
    bool is_in(string);
    unsigned int get_objcode(string);
    unsigned int get_length(string,string);

};
#endif

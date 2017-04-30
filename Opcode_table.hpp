#ifndef OPCODE_TABLE
#define OPCODE_TABLE
#include"header.hpp"

class Opcode{
private:
    class Data{
        public:
        unsigned char opcode;
        string symbol;
        unsigned int xe_format;
        //xe_format only for sic/xe
        //sic always use 3byte
        //1=1byte 2=2byte 3=3byte 4=4byte
        //5= 3/4 byte
    };

    static Data  SICcode[26];

    static string upper(string);

public:
    static bool is_in(string);
    static unsigned int get_objcode(string);
    static unsigned int get_length(string,string);

};
#endif

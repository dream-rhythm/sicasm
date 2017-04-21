#include"Opcode_table.hpp"

string Opcode::upper(string txt){
    for(unsigned int i=0;i<txt.length();i++){
        txt.at(i)=toupper(txt.at(i));
    }
    return txt;
}

bool Opcode::is_in(string str){
    str = Opcode::upper(str);
    for(unsigned int i=0;i<26;i++){
        if(str==Opcode::code[i].symbol)return true;
    }
    return false;
}
unsigned int Opcode::get_objcode(string str){
    str = Opcode::upper(str);
    for(unsigned int i=0;i<26;i++){
        if(str==Opcode::code[i].symbol)return (unsigned int)Opcode::code[i].opcode*65536;
    }
    return 0;
}
unsigned int Opcode::get_length(string str,string version){
    str = Opcode::upper(str);
    if(version=="SIC")return 3;
    else {
        for(unsigned int i=0;i<26;i++){
            if(str==Opcode::code[i].symbol)return Opcode::code[i].xe_format;
        }
        return -1;
    }
}

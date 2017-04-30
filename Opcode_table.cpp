#include"Opcode_table.hpp"
Opcode::Data Opcode::SICcode[26]={
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
string Opcode::upper(string txt){
    for(unsigned int i=0;i<txt.length();i++){
        txt.at(i)=toupper(txt.at(i));
    }
    return txt;
}

bool Opcode::is_in(string str){
    str = Opcode::upper(str);
    for(unsigned int i=0;i<26;i++){
        if(str==Opcode::SICcode[i].symbol)return true;
    }
    return false;
}
unsigned int Opcode::get_objcode(string str){
    str = Opcode::upper(str);
    for(unsigned int i=0;i<26;i++){
        if(str==Opcode::SICcode[i].symbol)return (unsigned int)Opcode::SICcode[i].opcode*65536;
    }
    return 0;
}
unsigned int Opcode::get_length(string str,string version){
    str = Opcode::upper(str);
    if(version=="SIC")return 3;
    else {
        for(unsigned int i=0;i<26;i++){
            if(str==Opcode::SICcode[i].symbol)return Opcode::SICcode[i].xe_format;
        }
        return -1;
    }
}

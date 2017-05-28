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

map<string,Opcode::OPCodeInformation> Opcode::OPTable=initTable();
map<string,Opcode::OPCodeInformation> Opcode::initTable(){
    //建立格式 tmp["OPCode"] = {0xOBJCode,is_xe_only,長度};
    map<string,Opcode::OPCodeInformation> tmp;
    tmp["ADD"]   ={0x18,false,3};
    tmp["ADDF"]  ={0x58, true,3};
    tmp["ADDR"]  ={0x90, true,2};
    tmp["AND"]   ={0x40,false,3};
    tmp["CLEAR"] ={0xb4, true,2};
    tmp["COMP"]  ={0X28,false,3};
    tmp["COMPF"] ={0x88, true,3};
    tmp["COMPR"] ={0xa0, true,2};
    tmp["DIV"]   ={0x24,false,3};
    tmp["DIVF"]  ={0x64, true,3};
    tmp["DIVR"]  ={0x9c, true,2};
    tmp["FIX"]   ={0xc4, true,1};
    tmp["FLOAT"] ={0xc0, true,1};
    tmp["HIO"]   ={0xf4, true,1};

    tmp["J"]     ={0x3c,false,3};
    tmp["JEQ"]   ={0x30,false,3};
    tmp["JGT"]   ={0x34,false,3};
    tmp["JLT"]   ={0x38,false,3};
    tmp["JSUB"]  ={0x48,false,3};
    tmp["LDA"]   ={0x00,false,3};
    tmp["LDB"]   ={0x68, true,3};
    tmp["LDCH"]  ={0x50,false,3};
    tmp["LDF"]   ={0x70, true,3};
    tmp["LDL"]   ={0x08,false,3};
    tmp["LDS"]   ={0x6c, true,3};
    tmp["LDT"]   ={0x74, true,3};
    tmp["LDX"]   ={0x04,false,3};
    tmp["LPS"]   ={0xd0, true,3};
    tmp["MUL"]   ={0x20,false,3};

    tmp["MULF"]  ={0x60, true,3};
    tmp["MULR"]  ={0x98, true,2};
    tmp["NORM"]  ={0xc8, true,1};
    tmp["OR"]    ={0x44,false,3};
    tmp["RD"]    ={0xd8,false,3};
    tmp["RMO"]   ={0xac, true,2};
    tmp["RSUB"]  ={0x4c,false,3};
    tmp["SHIFTL"]={0xa4, true,2};
    tmp["SHIFTR"]={0xa8, true,2};
    tmp["SIO"]   ={0xf0, true,1};


    tmp["SSK"]   ={0xec, true,3};
    tmp["STA"]   ={0x0c,false,3};
    tmp["STB"]   ={0x78, true,3};
    tmp["STCH"]  ={0x54,false,3};
    tmp["STF"]   ={0x80, true,3};
    tmp["STI"]   ={0xd4, true,3};
    tmp["STL"]   ={0x14,false,3};
    tmp["STS"]   ={0x7c, true,3};
    tmp["STSW"]  ={0xe8,false,3};
    tmp["STT"]   ={0x84, true,3};
    tmp["STX"]   ={0x10,false,3};
    tmp["SUB"]   ={0x1c,false,3};
    tmp["SUBF"]  ={0x5c, true,3};

    tmp["SUBR"]  ={0x94, true,2};
    tmp["SVC"]   ={0xb0, true,2};
    tmp["TD"]    ={0xe0,false,3};
    tmp["TIO"]   ={0xf8, true,1};
    tmp["TIX"]   ={0x2c,false,3};
    tmp["TIXR"]  ={0xb8, true,2};
    tmp["WD"]    ={0xdc,false,3};


    return tmp;
}



string Opcode::upper(string txt){
    for(unsigned int i=0;i<txt.length();i++){
        txt.at(i)=toupper(txt.at(i));
    }
    return txt;
}
/*
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
}*/

bool Opcode::is_in(string str,string version){
    map<string,Opcode::OPCodeInformation>::iterator it;
    it = OPTable.find(str);
    if(version=="SIC"){
        if(it!=OPTable.end()){
            if(it->second.xe_only==true)return false;
            else return true;
        }
        else return false;
    }
    else{
        return it!=OPTable.end();
    }

}
unsigned int Opcode::get_objcode(string str,string version){
    map<string,Opcode::OPCodeInformation>::iterator it;
    unsigned int ans;

    if(version=="SIC"){
        it = OPTable.find(str);
        if(it!=OPTable.end()){
            if(it->second.xe_only==false){
                ans = it->second.objcode;
                ans = ans*0x010000;
            }
            else{
                return 1;
            }
        }
        else{
            return 1;
        }
    }
    else{
        if(str.at(0)=='+')it = OPTable.find(str.substr(1));
        else it = OPTable.find(str);
        if(it!=OPTable.end()){
            ans = it->second.objcode;
            if(str.at(0)!='+')ans = ans*0x010000;
            else ans = ans*0x01000000;
            return ans;
        }
        else return 1;
    }
    return 1;
}
unsigned int Opcode::get_length(string str,string version){
    map<string,Opcode::OPCodeInformation>::iterator it;
    if(version=="SIC"){
        it = OPTable.find(str);
        if(it!=OPTable.end()){
            if(it->second.xe_only==true)return 0;
            else return 1;
        }
        else return 0;
    }
    else{
        if(str.at(0)=='+')it = OPTable.find(str.substr(1));
        else it = OPTable.find(str);
        if(it!=OPTable.end()){
            if(str.at(0)=='+'){
                if(it->second.length==3)return 4;
                else return 0;
            }
            else{
                return it->second.length;
            }
        }
        else return 0;
    }
}
bool Opcode::is_2Byte_r1r2(string opcode){
    string r1r2[8]={"ADDR","COMPR","DIVR","MULR","RMO","SHIFTL","SHIFTR","SUBR"};
    for(int i=0;i<8;i++)if(opcode==r1r2[i])return true;
    return false;
}
unsigned Opcode::find_register(string register_name){
    string NAME[10]={"A","X","L","B","S","T","F","","PC","SW"};
    string name[10]={"a","x","l","b","s","t","f","","pc","sw"};
    for(int i=0;i<10;i++)if(register_name==name[i]||register_name==NAME[i])return i;
    return 0;
}

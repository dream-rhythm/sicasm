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
    //尚未建立完成
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
    return tmp;
}



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
/*未來將啟用的code
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
            if(atr.at(0)!='+')ans = ans*0x010000;
            else ans = ans = ans*0x01000000 + 0x00100000;
            return ans;
        }
        else return 1;
    }
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
*/

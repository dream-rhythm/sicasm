#include"LocationCounter.hpp"

LocationCounter::LocationCounter(){
    this->isStart = false;
    this->location = 0;
}

void LocationCounter::add(int byte){
    this->location += byte;
}

void LocationCounter::add(string str,string need){
    if(str=="RESW")this->location += atoi(need.c_str())*3;
    else if(str=="RESB")this->location += atoi(need.c_str())*1;
    else if(str=="WORD")this->location += 3;
    else{
        if(need.at(0)=='X'){
            if(need.length()>=3)this->location += (need.length()-3)/2;
        }
        else this->location += need.length()-3;
    }
}

//0=success -1=it has been set   1=this is not hex number; 2=missing hex number
int  LocationCounter::set_Loc(string address){
    unsigned int loc=0;

    if(address=="")return 2;
    for(unsigned int i=0;i<address.length();i++){
        if(address.at(i)>='0'&&address.at(i)<='9')loc = loc*16 + address.at(i)-'0';
        else if(address.at(i)>='a'&&address.at(i)<='f')loc = loc*16 + address.at(i)-'a';
        else if(address.at(i)>='A'&&address.at(i)<='F')loc = loc*16 + address.at(i)-'A';
        else return 1;
    }
    if(this->isStart==false){
        this->location = loc;
        this->isStart = true;
        return 0;
    }
    else return -1;
}
int LocationCounter::set_Loc(unsigned int address){
    this->location=address;
    return 0;
}
unsigned int  LocationCounter::get_nowLoc(){
    return this->location;
}
bool LocationCounter::is_Start(){
    return this->isStart;
}
bool LocationCounter::set_org(unsigned int address){
    this->OrgStack.push(this->get_nowLoc());
    this->location = address;
    return true;
}
bool LocationCounter::set_org(){
    if(this->OrgStack.empty()){
        return false;
    }
    else{
        this->location = this->OrgStack.top();
        this->OrgStack.pop();
        return true;
    }
}

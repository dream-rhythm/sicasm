#include"reader.hpp"

void Reader::clear_status(){
    type=-1;
    cmd.clear();
    label.clear();
    action.clear();
    action_old.clear();
    data.clear();
    comment.clear();
};

Reader::Reader(string FileName,AsmTable*Asm){
    this->file.open(FileName,std::ios::in);
    this->AsmTAB = Asm;
    this->lines=0;
}
Reader::~Reader(){
    this->file.close();
}
string Reader::toUpper(string from){
    for(unsigned int i=0;i<from.length();i++){
        from.at(i)=toupper(from.at(i));
    }
    return from;
}
string Reader::replaceTab(string from){
    string ans,tmp;
    int txtLong=0;
    for(unsigned int i=0;i<from.length();i++){
        if(from.at(i)=='\t'){
            for(int j=0;j<4-(txtLong%4);j++){
                ans.append(" ");
            }
            txtLong+=4-(txtLong%4);
        }
        else{
            tmp=from.at(i);
            ans.append(tmp.c_str());
            txtLong++;
        }
    }
    return ans;
}
string Reader::removeHeaderSpace(string str){
    unsigned int i=0;
    //cout<<str<<" -> "<<str.substr(i,str.length()-i)<<endl;
    for(i=0;i<str.length();i++)if(str.at(i)!=' ')break;
    return str.substr(i,str.length()-i);
}
bool Reader::nextLine(){
    string tmp,old;
    int SpaceCounter=0;
    bool SpaceFlag=false;
    this->clear_status();
    this->lines++;
    if(getline(this->file,old)){
        old = this->replaceTab(old);
        tmp = this->toUpper(old);

        for(unsigned int i=0;i<tmp.length();i++){
            if(i>=8&&i<15&&SpaceCounter<1)SpaceCounter=1;
            else if(i>=18&&i<35&&SpaceCounter<2)SpaceCounter=2;
            else if(i>=35&&SpaceCounter<3)SpaceCounter=3;
            if(tmp.at(i)==' '){
                if(SpaceFlag==false){
                    SpaceFlag=true;
                    if(SpaceCounter<3)SpaceCounter++;
                }
                else{
                    if(SpaceCounter==3)this->comment+=old.at(i);
                }
            }
            else{
                if(tmp.at(i)=='.'){
                    SpaceCounter = 3;
                    SpaceFlag=true;
                }
                if(SpaceCounter==0){
                    this->label+=old.at(i);
                    SpaceFlag=false;
                }
                else if(SpaceCounter==1){
                    this->action+=tmp.at(i);
                    this->action_old+=old.at(i);
                    SpaceFlag=false;
                }
                else if(SpaceCounter==2){
                    this->data+=old.at(i);
                    SpaceFlag=false;
                }
                else{
                    this->comment+=old.at(i);
                }
            }
        }
        this->comment = this->removeHeaderSpace(this->comment);
        if(this->action=="START")this->type=0;
        else if(this->action=="END")type = 1;
        else if(this->action=="RESW"||this->action=="RESB")this->type=2;
        else if(this->action=="WORD"||this->action=="BYTE")this->type=3;
        else if(this->action=="")this->type=5;
        else this->type = 4;
/*
        cout<<"type= "<<this->type;
        cout<<"\t label= "<<this->label;
        cout<<"\t opcode= "<<this->action;
        cout<<"\t data= "<<this->data;
        cout<<"\t commit= "<<this->comment<<"\n";*/
        return true;
    }
    else return false;
}

AsmCode* Reader::get_AsmCode(){
    return new AsmCode(this->type,this->label,this->action,this->data,this->comment,this->lines,this->action_old);
}

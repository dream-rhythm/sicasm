#include"ASM_Table.hpp"
AsmCode::AsmCode(int type,string label,string opcode,string data,string commit,int line,string opcode_old){
    this->type=type;
    this->lable=label;
    this->opcode=opcode;
    this->opcode_old = opcode_old;
    this->data=data;
    this->commit=commit;
    this->line=line;
    this->head=nullptr;
    this->next=nullptr;
    this->address = 0;
};
AsmCode::~AsmCode(){
    if(this->head!=nullptr)delete this->head;
}
void AsmCode::add_ErrMes(string str){
    AsmCode::ErrMessage* errCode = new AsmCode::ErrMessage(str);
    errCode->set_next(this->head);
    this->head = errCode;
}
void AsmCode::set_Address(unsigned int address){
    this->address = address;
}
void AsmCode::set_objcode(string obj){
    this->objcode = obj;
}
void AsmCode::set_objcode(unsigned int  obj){
    stringstream ss;
    if(this->type==4)ss<<std::setfill('0')<<std::hex<<std::setw(6)<<obj<<std::setw(0)<<std::setfill(' ');
    else if(this->opcode=="WORD")ss<<std::setfill('0')<<std::hex<<std::setw(6)<<obj<<std::setw(0)<<std::setfill(' ');
    ss>>this->objcode;
}
unsigned int AsmCode::get_Address(){
    return this->address;
}
string AsmCode::get_objcode(){
    return this->objcode;
}

AsmCode::ErrMessage::ErrMessage(string str){
    this->Messenge=str;
    this->next = nullptr;
}
AsmCode::ErrMessage::~ErrMessage(){
    delete this->next;
}
void AsmCode::ErrMessage::set_next(ErrMessage*ptr){
    this->next=ptr;
}
void AsmCode::ErrMessage::printErrMes(fstream&fout){
    fout<<this->Messenge<<endl;
    if(this->next!=nullptr)this->next->printErrMes(fout);
}
void AsmCode::set_next(AsmCode*ptr){
    this->next = ptr;
}
int AsmCode::get_type(){
    return this->type;
}
string AsmCode::get_data(){
    return  this->data;
}
string AsmCode::get_label(){
    return this->lable;
}
string AsmCode::get_opcode(){
    return this->opcode;
}
void AsmCode::printData(){
    cout<<this->type<<"  "<<this->toString()<<endl;
}

string AsmCode::toString(){
    stringstream ss;
    string ans;
    ss<<setw(3)<<this->line<<setw(0);
    ss<<"\t "<<setw(6)<<std::hex<<this->address<<std::dec<<setw(0);
    ss<<"\t "<<setw(6)<<this->objcode<<setw(0);
    ss<<"\t "<<setw(6)<<this->lable<<setw(0);
    ss<<"\t "<<setw(6)<<std::left<<this->opcode_old<<std::right<<setw(0);
    ss<<"\t "<<setw(6)<<std::left<<this->data<<std::right<<setw(0);
    ss<<"\t   "<<this->commit;
    getline(ss,ans);
    return ans;
}
void AsmCode::printErrMes(fstream&fout){
    if(this->head!=nullptr){
        this->head->printErrMes(fout);
    }
}
bool AsmCode::has_ErrMes(){
    return this->head!=nullptr;
}
AsmTable::AsmTable(){
    this->head=nullptr;
    this->lines = 0;
}
AsmCode* AsmTable::find_end(){
    if(this->head==nullptr)return nullptr;
    AsmCode*tmp = this->head;
    while(tmp->next!=nullptr){
        tmp = tmp->next;
    }
    return tmp;
}
AsmTable::~AsmTable(){
    if(this->head!=nullptr)delete this->head;
}
void AsmTable::append(AsmCode*ptr){
    if(this->head==nullptr)this->head=ptr;
    else this->find_end()->set_next(ptr);
    this->lines++;
}
void AsmTable::printData(){
    if(this->head==nullptr);
    else{
        AsmCode* tmp =this->head;
        while(tmp!=nullptr){
            tmp->printData();
            tmp = tmp->next;
        }
    }
}
int AsmTable::get_lines(){
    return this->lines;
}
AsmCode*AsmTable::findLine(int line){
    if(this->lines < line)return nullptr;
    else {
        AsmCode* tmp =this->head;
        while(--line){
            tmp = tmp->next;
        }
        return tmp;
    }
}

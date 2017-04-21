#include"Label_table.hpp"

Label_table::Node::Node(string name,unsigned int address){
    this->address = address;
    this->name = name;
    this->isErr =false;
    this->next =nullptr;
}
Label_table::Node::~Node(){
    if(this->next!=nullptr)delete this->next;
}
Label_table::Label_table(){
    this->head = nullptr;
}
Label_table::~Label_table(){
    if(this->head!=nullptr)delete this->head;
}

Label_table::Node* Label_table::find_Node(string name){
    Label_table::Node*tmp = this->head;
    if(this->head==nullptr)return nullptr;
    else {
        while(tmp->name!=name&&tmp->next!=nullptr)tmp = tmp->next;
        if(tmp->name==name)return tmp;
        else return nullptr;
    }
}
bool Label_table::is_in(string name){
    return this->find_Node(name)!=nullptr;
}
Label_table::Node* Label_table::find_end(){
    if(this->head==nullptr)return nullptr;
    else{
        Label_table::Node*tmp=this->head;
        while(tmp->next!=nullptr)tmp=tmp->next;
        return tmp;
    }
}
void Label_table::append(string name,unsigned int address){
    if(this->is_in(name)){
        this->find_Node(name)->isErr =true;
    }
    else{
         Label_table::Node* newNode = new Label_table::Node(name,address);
         if(this->head==nullptr)this->head=newNode;
         else this->find_end()->next =newNode;
    }

}

unsigned int Label_table::get_address(string name){
    Label_table::Node * tmp = this->find_Node(name);
    if(tmp==nullptr){
        return 0;
    }
    else return tmp->address;
}

void Label_table::setErrflag(string name){
    this->find_Node(name)->isErr =true;
}

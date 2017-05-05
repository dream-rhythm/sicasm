#include"LiteralPool.hpp"

LiteralPool::LiteralPool(){
    this->head=nullptr;
}
LiteralPool::~LiteralPool(){
    delete this->head;
}


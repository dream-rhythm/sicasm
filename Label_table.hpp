#ifndef LABEL_TABLE
#define LABEL_TABLE


#include"header.hpp"

class Label_table{
    public:
        class Node{
            public:
                string name;
                bool isErr;
                unsigned int address;
                Node*next;
                Node(string,unsigned int);
                ~Node();
        };
        Label_table();
        ~Label_table();
        bool is_in(string);
        void append(string,unsigned int);
        unsigned int get_address(string);
        Node* find_Node(string);
        void setErrflag(string);
    private:
        Node* head;
        Node* find_end();


};
#endif

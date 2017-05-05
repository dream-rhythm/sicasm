#ifndef LITERAL
#define LITERAL
#include"header.hpp"
#include"ASM_Table.hpp"

class LiteralPool{
    private:
        AsmCode*head;

    public:
        LiteralPool();
        ~LiteralPool();
        void append(AsmCode*);
        AsmCode*output();
        bool has_next();
};
#endif // LITERAL

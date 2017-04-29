#ifndef Writer
#define Writer
#include"ASM_Table.hpp"
#include"header.hpp"
class writer{
    public:
        writer(string,string);
        ~writer();
        void outputList(AsmCode*);
        void outputHCard(string,unsigned int,unsigned int);
        void outputECard(unsigned int);
        void outputTCard(unsigned int,string);
        void writeTCard();
        void stopOutputObjFILE(bool);
    private:
        void initList();
        fstream ListFile;
        fstream ObjFile;
        char CardType;
        unsigned int startLoc;
        int ByteCounter;
        string CardContentBuff;
        bool stopOBJFILE;
};

#endif // Writer

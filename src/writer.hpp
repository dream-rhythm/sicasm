#ifndef Writer
#define Writer
#include"ASM_Table.hpp"
#include"header.hpp"
class writer{
    public:
        writer(string,string,bool);
        ~writer();
        void outputList(AsmCode*);
        void outputHCard(string,unsigned int,unsigned int);
        void outputECard(unsigned int);
        void outputTCard(unsigned int,string);
        void writeTCard();
        void stopOutputObjFILE(bool);
    private:
        void initList();
        string toUpper(string);
        fstream ListFile;
        fstream ObjFile;
        char CardType;
        unsigned int startLoc;
        int ByteCounter;
        string CardContentBuff;
        bool stopOBJFILE;
        bool is_Upper;
};

#endif // Writer

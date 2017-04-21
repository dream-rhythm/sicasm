#ifndef Writer
#define Writer
#include"ASM_Table.hpp"
#include <fstream>
#include<iomanip>
#include<string>
using std::fstream;
using std::setw;
using std::endl;
using std::setfill;
using std::dec;
using std::hex;
using std::string;
using std::ios;
class writer{
    public:
        writer(string,string);
        ~writer();
        void outputList(AsmCode*);
        void outputHCard(string,unsigned int,unsigned int);
        void outputECard(unsigned int);
        void outputTCard(unsigned int,string);
        void writeTCard();
        void initList();
    private:
        fstream ListFile;
        fstream ObjFile;
        char CardType;
        unsigned int startLoc;
        int ByteCounter;
        string CardContentBuff;

};

#endif // Writer

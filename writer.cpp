#include"writer.hpp"

writer::writer(string ListFileName,string objFileName){
    this->ObjFile.open(objFileName,ios::out);
    this->ListFile.open(ListFileName,ios::out);
}
writer::~writer(){
    this->ListFile.close();
    this->ObjFile.close();
    this->ByteCounter = 0;
    this->CardContentBuff="";
    this->CardType = ' ';
    this->startLoc=0;
}
void writer::outputHCard(string ProgramName,unsigned int startAddress,unsigned int ProgramLength){
    if(ProgramName.length()>6)this->ObjFile<<"H"<<ProgramName.substr(0,6);
    else{
        this->ObjFile<<"H"<<ProgramName;
        for(unsigned int i=0;i<6-ProgramName.length();i++)this->ObjFile<<" ";
        this->ObjFile<<setw(6)<<setfill('0')<<hex<<startAddress;
        this->ObjFile<<ProgramLength<<setw(0)<<setfill(' ')<<dec<<endl;
    }
}

void writer::outputECard(unsigned int mainAddress){
    this->writeTCard();
    this->ObjFile<<"E"<<hex<<setw(6)<<setfill('0')<<mainAddress<<dec<<setw(0)<<setfill(' ');
}
void writer::outputList(AsmCode*data){
    string outTxt =data->toString();
    this->ListFile<<outTxt<<endl;
    data->printErrMes(this->ListFile);
}
void writer::outputTCard(unsigned int address,string data){
    if(this->CardContentBuff.length()==0){
        this->ByteCounter=0;
        this->startLoc = address;
    }

    if(this->ByteCounter+data.length()/2>30){
        this->writeTCard();
        this->startLoc = address;
    }
    this->CardContentBuff = this->CardContentBuff + data;
    this->ByteCounter += data.length()/2;
}
void writer::writeTCard(){
    if(this->ByteCounter==0);
    else{
        this->ObjFile<<"T";
        this->ObjFile<<hex<<setw(6)<<setfill('0')<<this->startLoc<<dec<<setw(0)<<setfill(' ');
        this->ObjFile<<hex<<setw(2)<<setfill('0')<<this->ByteCounter<<dec<<setw(0)<<setfill(' ');
        this->ObjFile<<this->CardContentBuff<<endl;
        this->ByteCounter=0;
        this->CardContentBuff="";
        this->startLoc=0;
    }
}
void writer::initList(){
    this->ListFile<<"Sic Assamber v1.0 Made By Dream Rhythm"<<endl;
    this->ListFile<<"Line   Loc   obj     Label   OPCode  Data      comment"<<endl;
    this->ListFile<<"====   ====  ======  ======  ======  ========  =========================="<<endl;
}
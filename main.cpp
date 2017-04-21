#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include"Opcode_table.hpp"
#include"reader.hpp"
#include"ASM_Table.hpp"
#include"Label_table.hpp"
#include"LocationCoumter.hpp"
#include"writer.hpp"

using namespace std;
int setFileName(int,char*[],string&,string&,string&);
typedef struct{
    string FileName;
    unsigned int start_address;
    unsigned int end_address;
    unsigned int start_from;
    AsmCode*AsmTable;
}ProgramInfo;

AsmTable* Pass0(){
    AsmTable*ASM = new AsmTable();
    Reader * fin = new Reader("SRCFILE",ASM);
    while(fin->nextLine()){
        ASM->append(fin->get_AsmCode());
    }
    delete fin;
    return ASM;
}


int main(int arg_num,char* arg_data[]){

    string srcFileName,objFileName,listFileName;
    stringstream ss;
    string tmpStringData;
    unsigned int tmpData;
    bool stopObjFileOutput;
    LocationCounter*LocCtr= new LocationCounter();
    Label_table*SymbolTAB = new Label_table();
    Opcode * OPTAB = new Opcode();
    AsmTable* AsmTAB;
    AsmCode * tmpCode;
    writer * fout ;
    Reader * fin;

    int success = setFileName(arg_num,arg_data,srcFileName,objFileName,listFileName);
    if(success==0){
        cout<<"程式參數錯誤!!!\n編譯失敗..."<<endl;
        return 1;
    }

    //Pass0
    AsmTAB = new AsmTable();
    fin = new Reader(srcFileName,AsmTAB);
    while(fin->nextLine()){
        AsmTAB->append(fin->get_AsmCode());
    }

    //pass1:
    for(int i=1;i<=AsmTAB->get_lines();i++){
        tmpCode = AsmTAB->findLine(i);
        tmpCode->set_Address(LocCtr->get_nowLoc());
        if(LocCtr->get_nowLoc()>=0x8000)tmpCode->add_ErrMes("ErrorCode:63X17  Out of addressing range");
        //START
        if(tmpCode->get_type()==0){
            int code=LocCtr->set_Loc(tmpCode->get_data());
            if(code==-1)tmpCode->add_ErrMes("ErrorCode:19S0  Duplicate START statement");
            else if(code==1)tmpCode->add_ErrMes("ErrorCode: 5S0  Illegal hex string in START statement");
            else if(code==2)tmpCode->add_ErrMes("ErrorCode: 4S0  Missing operand in START statement");
            else tmpCode->set_Address(LocCtr->get_nowLoc());
            if(tmpCode->get_label()=="")tmpCode->add_ErrMes("ErrorCode: 3S0  Missing label in START statement");
        }
        //Comment
        else if(tmpCode->get_type()==5);
        else{
            if(tmpCode->get_label()!=""){
                tmpStringData = tmpCode->get_label();
                bool isok=true;
                for(unsigned int j=0;j<tmpStringData.length();j++)if(!isalnum(tmpStringData.at(j)))isok=false;
                if(!isok)tmpCode->add_ErrMes("ErrorCode: 1S0  Illegal format in label field");
                if( SymbolTAB->is_in(tmpStringData) ){
                    tmpCode->add_ErrMes("ErrorCode:26S0  Duplicate label definition");
                    SymbolTAB->setErrflag(tmpCode->get_label());
                }
                else if(OPTAB->is_in(tmpCode->get_label()))tmpCode->add_ErrMes("ErrorCode: 1S0  Illegal format in label field");
                else SymbolTAB->append(tmpCode->get_label(),LocCtr->get_nowLoc());
            }
            if(OPTAB->is_in(tmpCode->get_opcode()))LocCtr->add(OPTAB->get_length(tmpCode->get_opcode(),"SIC"));
            else if(tmpCode->get_type()==2||tmpCode->get_type()==3)LocCtr->add(tmpCode->get_opcode(),tmpCode->get_data());//WORD BYTE RESW RESB
            else if(tmpCode->get_type()==1);    //END
            else tmpCode->add_ErrMes("ErrorCode: 2S0  Illegal format in operation field");
        }
    }


    //Pass2
    for(int i=1;i<=AsmTAB->get_lines();i++){
        tmpCode = AsmTAB->findLine(i);
        if(tmpCode->get_type()==0);         //START
        else if(tmpCode->get_type()==1){    //END
            if(tmpCode->get_data()=="");
            else if(SymbolTAB->is_in(tmpCode->get_data()));
            else tmpCode->add_ErrMes("ErrorCode: 2S0  Illegal format in operation field");
        }
        else if(tmpCode->get_type()==2);    //RESW RESB
        else if(tmpCode->get_type()==3){    //WORD BYTE
            if(tmpCode->get_opcode()=="WORD"){
                ss.clear();
                ss.str("");
                ss<<tmpCode->get_data();
                ss>>tmpData;
                if(!ss.fail()&&tmpData>=0&&tmpData<=16777215)tmpCode->set_objcode(tmpData);
                else tmpCode->add_ErrMes("ErrorCode: 9S1  Illegal dec string in WORD statement");
            }
            else if(tmpCode->get_opcode()=="BYTE"){
                if(tmpCode->get_data().at(0)=='X'||tmpCode->get_data().at(0)=='x'){
                    bool errFlag=false;
                    if(tmpCode->get_data().length()>3){
                        if(tmpCode->get_data().at(1)!='\'')errFlag=true;
                        if(tmpCode->get_data().at(tmpCode->get_data().length()-1)!='\'')errFlag=true;
                    }
                    else errFlag=true;
                    ss.clear();
                    ss.str("");
                    for(unsigned int j=2;j<tmpCode->get_data().length()-1;j++){
                        if(isxdigit(tmpCode->get_data().at(j)))
                            ss<<(char)tolower(tmpCode->get_data().at(j));
                    else errFlag=true;
                    }
                    ss>>tmpStringData;
                    if(errFlag==false)tmpCode->set_objcode(tmpStringData);
                    else tmpCode->add_ErrMes("ErrorCode: 9S0  Illegal hex string in BYTE statement");
                }
                else if(tmpCode->get_data().at(0)=='C'||tmpCode->get_data().at(0)=='c'){
                    bool errFlag=false;
                    if(tmpCode->get_data().length()>3){
                        if(tmpCode->get_data().at(1)!='\'')errFlag=true;
                        if(tmpCode->get_data().at(tmpCode->get_data().length()-1)!='\'')errFlag=true;
                    }
                    else errFlag=true;
                    ss.clear();
                    ss.str("");
                    for(unsigned int j=2;j<tmpCode->get_data().length()-1;j++)ss<<std::hex<<(unsigned int)tmpCode->get_data().at(j)<<std::dec;
                    ss>>tmpStringData;
                    if(errFlag==false) tmpCode->set_objcode(tmpStringData);
                    else tmpCode->add_ErrMes("ErrorCode: 9S0  Illegal hex string in BYTE statement");
                }
                else tmpCode->add_ErrMes("ErrorCode: 9S0  Illegal string in BYTE statement");
            }
        }
        else if(tmpCode->get_type()==4){//has OPcode
            if(OPTAB->is_in(tmpCode->get_opcode())){
                tmpData = OPTAB->get_objcode(tmpCode->get_opcode());
                if(tmpCode->get_opcode()=="RSUB"){
                    tmpCode->set_objcode(tmpData);
                    if(tmpCode->get_data()!="")tmpCode->add_ErrMes("ErrorCode:  X    Operand should not follow RSUB statement");
                }
                else{
                    tmpStringData = tmpCode->get_data();
                    if(tmpStringData.length()>=3){
                        if((tmpStringData.at(tmpStringData.length()-1)=='X'||tmpStringData.at(tmpStringData.length()-1)=='x')&&tmpStringData.at(tmpStringData.length()-2)==',')tmpStringData = tmpStringData.substr(0,tmpStringData.length()-2);
                    }
                    if(SymbolTAB->is_in(tmpStringData)){
                        tmpData+=SymbolTAB->get_address(tmpStringData);
                    }
                    else tmpCode->add_ErrMes("ErrorCode:54X11  Undefined symbol in operand field");
                    if(tmpCode->get_data().length()>=3){
                        if(tmpCode->get_data().substr(tmpCode->get_data().length()-3,2)==",X")tmpData+=0x8000;
                    }
                    tmpCode->set_objcode(tmpData);
                }
            }
            else{
                cout<<tmpCode->toString()<<"has an error!!!\n";
                tmpCode->add_ErrMes("ErrorCode: 2S0  Illegal format in operation field");
            }
        }
        else if(tmpCode->get_type()==5);    //Comment
    }

    //Pass3
    fout = new writer(listFileName,objFileName);
    stopObjFileOutput=false;
    fout->initList();
    for(int i=1;i<=AsmTAB->get_lines();i++){
        tmpCode = AsmTAB->findLine(i);
        fout->outputList(tmpCode);
        if(tmpCode->has_ErrMes())stopObjFileOutput=true;
        if(stopObjFileOutput)continue;
        if(tmpCode->get_type()==0)fout->outputHCard(tmpCode->get_label(),tmpCode->get_Address(),LocCtr->get_nowLoc());//START
        else if(tmpCode->get_type()==1){
            if(tmpCode->get_data()=="")fout->outputECard(AsmTAB->findLine(1)->get_Address());
            else if(SymbolTAB->is_in(tmpCode->get_data()))fout->outputECard(SymbolTAB->get_address(tmpCode->get_data()));    //END
            else fout->outputECard(AsmTAB->findLine(1)->get_Address());
        }

        else if(tmpCode->get_type()==2)fout->writeTCard();    //RESW RESB
        else if(tmpCode->get_type()==3)fout->outputTCard(tmpCode->get_Address(),tmpCode->get_objcode());    //WORD BYTE
        else if(tmpCode->get_type()==4)fout->outputTCard(tmpCode->get_Address(),tmpCode->get_objcode());    //opcode
    }
    delete fout;
    delete AsmTAB;
    delete OPTAB;
    delete SymbolTAB;
    delete LocCtr;
    delete fin;
    return 0;

}

int setFileName(int arg_num,char* arg_data[],string&asmFileName,string&objFileName,string&listFileName){
    listFileName = "ListFile";
    asmFileName = "SRCFILE";
    objFileName = "OBJFILE";
    if(arg_num==1){
        //use defalut value;
    }
    else if(arg_num==2){
        if(strcmp(arg_data[1],"/?")==0||strcmp(arg_data[1],"/help")==0){
            cout<<"sicsim [/i][filename] [/o][filename]"<<endl;
            cout<<"/i  指定輸入檔檔名"<<endl;
            cout<<"/o  指定輸出檔檔名"<<endl;
            cout<<"如未指定,將使用srcfile為輸入檔,objfile為輸出檔"<<endl;
            cout<<"ListFile為固定輸出檔"<<endl;
            return 2;
        }
        else{
            return 0;
        }
    }
    else if(arg_num==3){
        if(strcmp(arg_data[1],"/i")){
            asmFileName = arg_data[2];
        }
        else if(strcmp(arg_data[1],"/o")){
            objFileName = arg_data[2];
        }
        else{
            return 0;
        }
    }
    else if(arg_num==5){
        if(strcmp(arg_data[1],"/i")==0){
            asmFileName = arg_data[1];
            if(strcmp(arg_data[3],"/o")==0){
                objFileName = arg_data[4];
            }
            else  return 0;
        }
        else if(strcmp(arg_data[1],"/o")==0){
            if(strcmp(arg_data[3],"/i")==0){
                asmFileName = arg_data[4];
            }
            else return 0;
            objFileName = arg_data[2];
        }
        else{
            return 0;
        }
    }
    else return 0;
    return 1;
}

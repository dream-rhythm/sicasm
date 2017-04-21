#include"header.hpp"
#include"Opcode_table.hpp"
#include"reader.hpp"
#include"ASM_Table.hpp"
#include"Label_table.hpp"
#include"LocationCoumter.hpp"
#include"writer.hpp"
#include"ErrMes.hpp"
int setFileName(int,char*[],string&,string&,string&,string&);

int main(int arg_num,char* arg_data[]){

    string srcFileName,objFileName,listFileName;
    string language;
    stringstream ss;
    string tmpStringData;
    unsigned int tmpData;
    bool stopObjFileOutput;
    LocationCounter*LocCtr= new LocationCounter();
    Label_table*SymbolTAB = new Label_table();
    AsmTable* AsmTAB;
    AsmCode * tmpCode;
    writer * fout ;
    Reader * fin;

    int success = setFileName(arg_num,arg_data,srcFileName,objFileName,listFileName,language);
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
        if(LocCtr->get_nowLoc()>=0x8000)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(64,language));
        //START
        if(tmpCode->get_type()==0){
            int code=LocCtr->set_Loc(tmpCode->get_data());
            if(code==-1)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(19,language));
            else if(code==1)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(5,language));
            else if(code==2)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(4,language));
            else tmpCode->set_Address(LocCtr->get_nowLoc());
            if(tmpCode->get_label()=="")tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(3,language));
        }
        //Comment
        else if(tmpCode->get_type()==5);
        else{
            if(tmpCode->get_label()!=""){
                tmpStringData = tmpCode->get_label();
                bool isok=true;
                for(unsigned int j=0;j<tmpStringData.length();j++)if(!isalnum(tmpStringData.at(j)))isok=false;
                if(!isok)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(1,language));
                if( SymbolTAB->is_in(tmpStringData) ){
                    tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(26,language));
                    SymbolTAB->setErrflag(tmpCode->get_label());
                }
                else if(Opcode::is_in(tmpCode->get_label()))tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(1,language));
                else SymbolTAB->append(tmpCode->get_label(),LocCtr->get_nowLoc());
            }
            if(Opcode::is_in(tmpCode->get_opcode()))LocCtr->add(Opcode::get_length(tmpCode->get_opcode(),"SIC"));
            else if(tmpCode->get_type()==2||tmpCode->get_type()==3)LocCtr->add(tmpCode->get_opcode(),tmpCode->get_data());//WORD BYTE RESW RESB
            else if(tmpCode->get_type()==1);    //END
            else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(2,language));
        }
    }


    //Pass2
    for(int i=1;i<=AsmTAB->get_lines();i++){
        tmpCode = AsmTAB->findLine(i);
        if(tmpCode->get_type()==0);         //START
        else if(tmpCode->get_type()==1){    //END
            if(tmpCode->get_data()=="");
            else if(SymbolTAB->is_in(tmpCode->get_data()));
            else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(2,language));
        }
        else if(tmpCode->get_type()==2);    //RESW RESB
        else if(tmpCode->get_type()==3){    //WORD BYTE
            if(tmpCode->get_opcode()=="WORD"){
                ss.clear();
                ss.str("");
                ss<<tmpCode->get_data();
                ss>>tmpData;
                if(!ss.fail()&&tmpData>=0&&tmpData<=16777215)tmpCode->set_objcode(tmpData);
                else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,language));
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
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,language));
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
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,language));
                }
                else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,language));
            }
        }
        else if(tmpCode->get_type()==4){//has OPcode
            if(Opcode::is_in(tmpCode->get_opcode())){
                tmpData = Opcode::get_objcode(tmpCode->get_opcode());
                if(tmpCode->get_opcode()=="RSUB"){
                    tmpCode->set_objcode(tmpData);
                    if(tmpCode->get_data()!="")tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(23,language));
                }
                else{
                    tmpStringData = tmpCode->get_data();
                    if(tmpStringData.length()>=3){
                        if((tmpStringData.at(tmpStringData.length()-1)=='X'||tmpStringData.at(tmpStringData.length()-1)=='x')&&tmpStringData.at(tmpStringData.length()-2)==',')tmpStringData = tmpStringData.substr(0,tmpStringData.length()-2);
                    }
                    if(SymbolTAB->is_in(tmpStringData)){
                        tmpData+=SymbolTAB->get_address(tmpStringData);
                    }
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(54,language));
                    if(tmpCode->get_data().length()>=3){
                        if(tmpCode->get_data().substr(tmpCode->get_data().length()-3,2)==",X")tmpData+=0x8000;
                    }
                    tmpCode->set_objcode(tmpData);
                }
            }
            else{
                //cout<<tmpCode->toString()<<"has an error!!!\n";
                tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(2,language));
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
    delete SymbolTAB;
    delete LocCtr;
    delete fin;
    return 0;

}

int setFileName(int arg_num,char* arg_data[],string&asmFileName,string&objFileName,string&listFileName,string&language){
    listFileName = "ListFile";
    asmFileName = "SRCFILE";
    objFileName = "OBJFILE";
    language = "zh";
    if(arg_num==1){
        //use defalut value;
    }
    else if(arg_num==2){
        if(strcmp(arg_data[1],"/?")==0||strcmp(arg_data[1],"/help")==0){
            cout<<endl<<"sicasm [/i][filename] [/o][filename] [/l][filename] [/L][eng/zh]"<<endl<<endl;
            cout<<"/i  指定輸入檔檔名"<<endl;
            cout<<"/o  指定objFile檔名"<<endl;
            cout<<"/l  指定ListFile檔名"<<endl;
            cout<<"default值:使用SRCFILE為輸入檔,OBJFILE與ListFile為輸出檔"<<endl<<endl;;
            cout<<"/L  指定錯誤訊息的語言[eng為英文 zh為繁體中文]"<<endl;
            cout<<"default值:使用繁體中文為預設語言"<<endl;
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

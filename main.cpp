#include"header.hpp"
#include"Opcode_table.hpp"
#include"reader.hpp"
#include"ASM_Table.hpp"
#include"Label_table.hpp"
#include"LocationCoumter.hpp"
#include"writer.hpp"
#include"ErrMes.hpp"
#include"AsmSetting.hpp"
int setFileName(int,char*[],AsmSetting&);

int main(int arg_num,char* arg_data[]){
    AsmSetting Setting;
    stringstream ss;
    string tmpStringData;
    unsigned int tmpData;
    bool stopObjFileOutput;
    bool is_end_statament=false;
    LocationCounter*LocCtr= new LocationCounter();
    Label_table*SymbolTAB = new Label_table();
    AsmTable* AsmTAB;
    AsmCode * tmpCode;
    writer * fout ;
    Reader * fin;

    int success = setFileName(arg_num,arg_data,Setting);
    if(success==1){
        cout<<"程式參數錯誤!!!\n組譯失敗..."<<endl;
        return 1;
    }
    else if(success==2)return 0;

    //Pass0
    AsmTAB = new AsmTable();                                                        //new 一個中繼暫存表
    fin = new Reader(Setting.srcFileName,Setting.TabSpace);                         //new 一個讀取器
    while(fin->nextLine()){                                                         //當成功讀取下一句時
        AsmTAB->append(fin->get_AsmCode());                                         //把這句話加入中繼暫存表
    }

    //pass1:
    for(int i=1;i<=AsmTAB->get_lines();i++){                                        //從第一行開始
        tmpCode = AsmTAB->findLine(i);                                              //找到那行
        tmpCode->set_Address(LocCtr->get_nowLoc());                                 //設定該行的Location
        if(LocCtr->get_nowLoc()>=0x8000)tmpCode->add_ErrMes(64,Setting.language);   //如果超出0x8000 噴錯誤64(Out of addressing range)
        //START
        if(tmpCode->get_type()==START){                                             //如果是START語句
            int code=LocCtr->set_Loc(tmpCode->get_data());                          //設定LocationCounter
            if(code==-1)tmpCode->add_ErrMes(19,Setting.language);                       //設定失敗(重複設定)   噴錯誤19(Duplicate START statement)
            else if(code==1)tmpCode->add_ErrMes(5,Setting.language);                    //設定失敗(16進制錯誤) 噴錯誤5(Illegal hex string in START statement)
            else if(code==2)tmpCode->add_ErrMes(4,Setting.language);                    //設定失敗(欄位為空)   噴錯誤4(Missing operand in START statement)
            else tmpCode->set_Address(LocCtr->get_nowLoc());                            //設定成功             重設該句Loction為指定的值
            if(tmpCode->get_label()=="")tmpCode->add_ErrMes(3,Setting.language);    //如果程式名稱欄位為空     噴錯誤3(Missing label in START statement)
        }
        //Comment
        else if(tmpCode->get_type()==COMMENT);                                      //如果是註解  啥都不做
        else if(tmpCode->get_type()==ORG){                                          //如果是ORG指令
            if(tmpCode->get_data()==""){                                                //如果未指定ORG的位址
                if(LocCtr->set_org());                                                  //跳回原本的地方
                else tmpCode->add_ErrMes(63,Setting.language);                          //跳回失敗的話        噴錯誤63(ORG statement must be used before jump back)
            }
            else{
                tmpStringData = tmpCode->get_data();
                unsigned int loc=0;
                if(tmpStringData.find("+")){
                    if(SymbolTAB->is_in(tmpCode->get_data().substr(0,tmpStringData.find("+"))))loc = SymbolTAB->get_address(tmpCode->get_data().substr(0,tmpStringData.find("+")));
                    else tmpCode->add_ErrMes(36,Setting.language);
                    loc+=atoi(tmpCode->get_data().substr(tmpStringData.find("+")).c_str());
                }
                else if(tmpStringData.find("-")){
                    if(SymbolTAB->is_in(tmpCode->get_data().substr(0,tmpStringData.find("-"))))loc = SymbolTAB->get_address(tmpCode->get_data().substr(0,tmpStringData.find("+")));
                    else tmpCode->add_ErrMes(36,Setting.language);
                    loc+=atoi(tmpCode->get_data().substr(tmpStringData.find("-")).c_str());
                }
                else{
                    if(SymbolTAB->is_in(tmpCode->get_data()))loc = SymbolTAB->get_address(tmpCode->get_data());
                    else tmpCode->add_ErrMes(36,Setting.language);
                }
                LocCtr->set_org(loc);
            }
        }
        else{
            if(tmpCode->get_label()!=""){
                tmpStringData = tmpCode->get_label();
                bool isok=true;
                for(unsigned int j=0;j<tmpStringData.length();j++)if(!isalnum(tmpStringData.at(j)))isok=false;
                if(!isok)tmpCode->add_ErrMes(1,Setting.language);
                if( SymbolTAB->is_in(tmpStringData) ){
                    tmpCode->add_ErrMes(26,Setting.language);
                    SymbolTAB->setErrflag(tmpCode->get_label());
                }
                else if(Opcode::is_in(tmpCode->get_label()))tmpCode->add_ErrMes(1,Setting.language);
                else SymbolTAB->append(tmpCode->get_label(),LocCtr->get_nowLoc());
            }
            if(Opcode::is_in(tmpCode->get_opcode()))LocCtr->add(Opcode::get_length(tmpCode->get_opcode(),"SIC"));
            else if(tmpCode->get_type()==2||tmpCode->get_type()==3)LocCtr->add(tmpCode->get_opcode(),tmpCode->get_data());//WORD BYTE RESW RESB
            else if(tmpCode->get_type()==1);    //END
            else tmpCode->add_ErrMes(2,Setting.language);
        }
    }


    //Pass2
    for(int i=1;i<=AsmTAB->get_lines();i++){
        tmpCode = AsmTAB->findLine(i);
        if(is_end_statament&&tmpCode->get_type()!=5)tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(6,Setting.language));
        if(tmpCode->get_type()==0);         //START
        else if(tmpCode->get_type()==1){    //END
            if(tmpCode->get_data()=="");
            else if(SymbolTAB->is_in(tmpCode->get_data()));
            else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(2,Setting.language));
            is_end_statament =true;
        }
        else if(tmpCode->get_type()==2);    //RESW RESB
        else if(tmpCode->get_type()==3){    //WORD BYTE
            if(tmpCode->get_opcode()=="WORD"){
                ss.clear();
                ss.str("");
                ss<<tmpCode->get_data();
                ss>>tmpData;
                if(!ss.fail()&&tmpData>=0&&tmpData<=16777215)tmpCode->set_objcode(tmpData);
                else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,Setting.language));
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
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,Setting.language));
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
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,Setting.language));
                }
                else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(9,Setting.language));
            }
        }
        else if(tmpCode->get_type()==4){//has OPcode
            if(Opcode::is_in(tmpCode->get_opcode())){
                tmpData = Opcode::get_objcode(tmpCode->get_opcode());
                if(tmpCode->get_opcode()=="RSUB"){
                    tmpCode->set_objcode(tmpData);
                    if(tmpCode->get_data()!="")tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(23,Setting.language));
                }
                else{
                    tmpStringData = tmpCode->get_data();
                    if(tmpStringData.length()>=3){
                        if((tmpStringData.at(tmpStringData.length()-1)=='X'||tmpStringData.at(tmpStringData.length()-1)=='x')&&tmpStringData.at(tmpStringData.length()-2)==',')tmpStringData = tmpStringData.substr(0,tmpStringData.length()-2);
                    }
                    if(SymbolTAB->is_in(tmpStringData)){
                        tmpData+=SymbolTAB->get_address(tmpStringData);
                    }
                    else tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(54,Setting.language));
                    if(tmpCode->get_data().length()>=3){
                        if(tmpCode->get_data().substr(tmpCode->get_data().length()-3,2)==",X")tmpData+=0x8000;
                    }
                    tmpCode->set_objcode(tmpData);
                }
            }
            else{
                //cout<<tmpCode->toString()<<"has an error!!!\n";
                tmpCode->add_ErrMes(ErrMes::get_ErrorMessage(2,Setting.language));
            }
        }
        else if(tmpCode->get_type()==5);    //Comment
    }

    //Pass3
    fout = new writer(Setting.listFileName,Setting.objFileName);
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
        else if(tmpCode->get_type()==6){
            fout->writeTCard();
        }
    }
    delete fout;
    delete AsmTAB;
    delete SymbolTAB;
    delete LocCtr;
    delete fin;
    return 0;

}

int setFileName(int arg_num,char* arg_data[],AsmSetting&Setting){
    bool is_src=false,is_obj=false,is_list=false,is_help=false,is_tab=false,is_Language=false;
    string src,obj,list,language,tmp;
    int tab;
    for(int i=2;i<=arg_num;i++){
        tmp = arg_data[i-1];
        if(tmp=="/?")is_help=true;
        else if(tmp=="/i"){
            if(is_src)return 1;
            is_src=true;
            src = arg_data[i];
            i++;
        }
        else if(tmp=="/o"){
            if(is_obj)return 1;
            is_obj=true;
            obj = arg_data[i];
            i++;
        }
        else if(tmp=="/l"){
            if(is_list)return 1;
            is_list=true;
            list=arg_data[i];
            i++;
        }
        else if(tmp=="/L"){
            if(is_Language)return 1;
            is_Language = true;
            language  = arg_data[i];
            if(language=="zh"||language=="eng");
            else return 1;
            i++;
        }
        else if(tmp=="/t"){
            if(is_tab)return 1;
            is_tab=true;
            tab=atoi(arg_data[i]);
            i++;
        }
    }
    if(is_help){
        cout<<endl<<"sicasm [/i][filename] [/o][filename] [/l][filename] [/L][eng/zh] [/t][TabLength]"<<endl<<endl;
        cout<<"/i  指定輸入檔檔名"<<endl;
        cout<<"/o  指定objFile檔名"<<endl;
        cout<<"/l  指定ListFile檔名"<<endl;
        cout<<"default值:使用SRCFILE為輸入檔,OBJFILE與ListFile為輸出檔"<<endl<<endl;;
        cout<<"/L  指定錯誤訊息的語言[eng為英文 zh為繁體中文]"<<endl;
        cout<<"default值:使用繁體中文為預設語言"<<endl<<endl;
        cout<<"/t  指定一個tab鍵所代表的空白鍵"<<endl;
        cout<<"default值:一個tab代表4個空白鍵"<<endl;
        return 2;
    }
    else{
        if(is_Language){
            if(language.at(0)!='/')Setting.language=language;
            else return 1;
        }
        else Setting.language ="zh";

        if(is_list){
            if(list.at(0)!='/')Setting.listFileName=list;
            else return 1;
        }
        else Setting.listFileName="ListFile";

        if(is_obj){
            if(obj.at(0)!='/')Setting.objFileName=obj;
            else return 1;
        }
        else Setting.objFileName="OBJFILE";

        if(is_src){
            if(src.at(0)!='/')Setting.srcFileName=src;
            else return 1;
        }
        else Setting.srcFileName="SRCFILE";

        if(is_tab){
            if(tab!=0)Setting.TabSpace=tab;
            else return 1;
        }
        else Setting.TabSpace=4;
    }
    return 0;
}

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
    ProgramInfo Info;
    stringstream ss;
    string tmpStr;
    unsigned int tmpData;
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
            else {
                tmpCode->set_Address(LocCtr->get_nowLoc());                             //設定成功   重設該句Loction為指定的值
                Info.start=LocCtr->get_nowLoc();                                        //設定成功   設定程式起始點
            }
            if(tmpCode->get_label()=="")tmpCode->add_ErrMes(3,Setting.language);    //如果程式名稱欄位為空     噴錯誤3(Missing label in START statement)
            else Info.Name=tmpCode->get_label();                                    //設定程式名稱
        }
        //Comment
        else if(tmpCode->get_type()==COMMENT);                                      //如果是註解  啥都不做
        else if(tmpCode->get_type()==END)Info.end=LocCtr->get_nowLoc();             //設定程式結尾
        else if(tmpCode->get_type()==ORG){                                          //如果是ORG指令
            if(tmpCode->get_data()==""){                                            //如果未指定ORG的位址
                if(LocCtr->set_org());                                                  //跳回原本的地方
                else tmpCode->add_ErrMes(63,Setting.language);                          //跳回失敗的話        噴錯誤63(ORG statement must be used before jump back)
            }
            else{                                                                   //如果有指定ORG的位址
                tmpStr = tmpCode->get_data();                                       //建立暫存
                unsigned int loc=0;                                                     //設LOC=0
                if(tmpStr.find("+")){                                                   //如果位址中找到+
                    string label=tmpCode->get_data().substr(0,tmpStr.find("+"));            //分割出label
                    string number = tmpCode->get_data().substr(tmpStr.find("+"));           //以及後面的數字
                    if(SymbolTAB->is_in(label))loc = SymbolTAB->get_address(label);         //如果有找到label 設定對應的location
                    else tmpCode->add_ErrMes(36,Setting.language);                          //否則    噴錯誤36(Symbol must be defined before used in ORG statement)
                    loc+=atoi(number.c_str());                                              //加上最後的數字
                }
                else if(tmpStr.find("-")){                                              //如果位址中找到-
                    string label=tmpCode->get_data().substr(0,tmpStr.find("-"));            //分割出label
                    string number = tmpCode->get_data().substr(tmpStr.find("-"));           //以及後面的數字
                    if(SymbolTAB->is_in(label))loc = SymbolTAB->get_address(label);         //如果有找到label 設定對應的location
                    else tmpCode->add_ErrMes(36,Setting.language);                          //否則    噴錯誤36(Symbol must be defined before used in ORG statement)
                    loc-=atoi(number.c_str());                                              //減去最後的數字
                }
                else{                                                                   //如果沒有+-
                    if(SymbolTAB->is_in(tmpCode->get_data()))loc = SymbolTAB->get_address(tmpCode->get_data());//如果有找到label 設定對應的location
                    else tmpCode->add_ErrMes(36,Setting.language);                          //否則    噴錯誤36(Symbol must be defined before used in ORG statement)
                }
                LocCtr->set_org(loc);                                                   //設定LocationCounter為指定的目標
            }
        }
        else{                                                                       //其他(operator,word,byte,resw,resb)
            if(tmpCode->get_label()!=""){                                           //如果label不為空
                tmpStr = tmpCode->get_label();                                      //存入暫存用變數
                bool isok=true;                                                     //設檢查器為true
                for(unsigned j=0;j<tmpStr.length();j++)if(!isalnum(tmpStr.at(j)))isok=false;//如果有不合規定的字 設檢查器為false
                if(!isok)tmpCode->add_ErrMes(1,Setting.language);                   //如果檢查失敗     噴錯誤1(Illegal format in label field)
                if( SymbolTAB->is_in(tmpStr) ){                                     //如果這個label已經存在
                    tmpCode->add_ErrMes(26,Setting.language);                       //噴錯誤26(Duplicate label definition)
                    SymbolTAB->setErrflag(tmpCode->get_label());                    //並把這個label設成錯誤標籤
                }
                else if(Opcode::is_in(tmpStr))tmpCode->add_ErrMes(1,Setting.language);//如果這個label是operator 噴錯誤1(Illegal format in label field)
                else SymbolTAB->append(tmpStr,LocCtr->get_nowLoc());                 //否則 加入symbol存起來
            }
            tmpStr=tmpCode->get_opcode();                                           //獲取指令
            if(Opcode::is_in(tmpStr))LocCtr->add(Opcode::get_length(tmpStr,"SIC")); //如果他是opcode 把LocationCounter加上對應的空間
            else if(tmpCode->get_type()==RES||tmpCode->get_type()==CONST)LocCtr->add(tmpStr,tmpCode->get_data());//WORD BYTE RESW RESB
            else tmpCode->add_ErrMes(2,Setting.language);                           //如果都不是   噴錯誤2(Illegal format in operation field)
        }
    }


    //Pass2
    for(int i=1;i<=AsmTAB->get_lines();i++){                                       //從第一行開始
        tmpCode = AsmTAB->findLine(i);                                             //指定該行
        if(is_end_statament&&tmpCode->get_type()!=COMMENT)tmpCode->add_ErrMes(6,Setting.language);//如果讀到END語句 後面還有非註解的東西 噴錯誤6(Undefined label after END statement)
        if(tmpCode->get_type()==START);                                            //如果是START語句   啥都不做
        else if(tmpCode->get_type()==END){                                         //如果是END語句
            if(tmpCode->get_data()=="")Info.main=Info.start;                            //如果沒設定起始點  設起始點為程式第一行
            else if(SymbolTAB->is_in(tmpCode->get_data()))Info.main=SymbolTAB->get_address(tmpCode->get_data());//找的到label 設該label為程式起點                             //如果找的到指定的位址 也是啥都不做
            else {                                                                      //如果找不到
                Info.main=Info.start;                                                        //設起始點為程式第一行
                tmpCode->add_ErrMes(2,Setting.language);                                     // 噴錯誤2(Illegal format in operation field)
            }
            is_end_statament =true;                                                     //把end旗標設成true
        }
        else if(tmpCode->get_type()==RES);                                          //RESW RESB    啥都不做
        else if(tmpCode->get_type()==CONST){                                        //WORD BYTE
            if(tmpCode->get_opcode()=="WORD"){                                          //如果是word
                ss.clear();                                                             //清空轉換器旗標
                ss.str("");                                                             //清空轉換器內容
                ss<<tmpCode->get_data();                                                //輸入
                ss>>tmpData;                                                            //輸出
                if(!ss.fail()&&tmpData>=0&&tmpData<=0xffffff)tmpCode->set_objcode(tmpData); //如果輸出成功 就設定objcode
                else tmpCode->add_ErrMes(9,Setting.language);                               //轉換失敗 噴錯誤9(Illegal hex string in BYTE statement)
            }
            else if(tmpCode->get_opcode()=="BYTE"){                                     //如果是byte
                bool errFlag=false;                                                         //設錯誤旗標為false
                if(tmpCode->get_data().length()>3){                                         //若長度>3
                    if(tmpCode->get_data().at(1)!='\'')errFlag=true;                        //檢查有沒有被單引號刮起來
                    if(tmpCode->get_data().at(tmpCode->get_data().length()-1)!='\'')errFlag=true;   //若缺少其中一邊 把錯誤旗標設成true
                }
                else errFlag=true;                                                          //若長度不足3直接設成錯誤
                ss.clear();                                                                 //清空轉換器旗標
                ss.str("");                                                                 //清空轉換器內容
                if(tmpCode->get_data().at(0)=='X'||tmpCode->get_data().at(0)=='x'){         //如果是x
                    for(unsigned j=2;j<tmpCode->get_data().length()-1;j++){                      //一個字一個字
                        if(isxdigit(tmpCode->get_data().at(j)))                                      //如果是合法的字
                            ss<<(char)tolower(tmpCode->get_data().at(j));                            //塞到轉換器
                        else errFlag=true;                                                           //不合法就設錯誤旗標為true
                    }
                }
                else if(tmpCode->get_data().at(0)=='C'||tmpCode->get_data().at(0)=='c'){     //如果是c
                    for(unsigned j=2;j<tmpCode->get_data().length()-1;j++)                      //一個字一個字
                        ss<<std::hex<<(unsigned int)tmpCode->get_data().at(j)<<std::dec;            //塞到轉換器
                }
                else tmpCode->add_ErrMes(9,Setting.language);                                //如果不是x,c 噴錯誤9(Illegal hex string in BYTE statement)
                ss>>tmpStr;                                                                  //輸出
                if(errFlag==false)tmpCode->set_objcode(tmpStr);                              //如果通過檢驗 設定objcode
                else tmpCode->add_ErrMes(9,Setting.language);                                //否則 噴錯誤9(Illegal hex string in BYTE statement)
            }
        }
        else if(tmpCode->get_type()==OPERATOR){                                     //如果是OPcode
            if(Opcode::is_in(tmpCode->get_opcode())){                                   //且在optable找的到
                tmpData = Opcode::get_objcode(tmpCode->get_opcode());                       //設定他的數值
                if(tmpCode->get_opcode()=="RSUB"){                                          //如果是rsub
                    if(tmpCode->get_data()!="")tmpCode->add_ErrMes(23,Setting.language);        //而且data有資料 噴錯誤23(Operand should not follow RSUB statement)
                }
                else{                                                                       //其他部分
                    tmpStr = tmpCode->get_data();                                               //獲取資料
                    if(tmpStr.length()>=3){                                                     //如果字數>3
                        if((tmpStr.at(tmpStr.length()-1)=='X'||tmpStr.at(tmpStr.length()-1)=='x')   //而且末兩碼是",x"
                           &&tmpStr.at(tmpStr.length()-2)==','){
                                tmpStr = tmpStr.substr(0,tmpStr.length()-2);                        //切出label
                                tmpData+=0x8000;                                                    //objcode+0x8000
                           }
                    }
                    if(SymbolTAB->is_in(tmpStr)){                                               //如果找的到label
                        tmpData+=SymbolTAB->get_address(tmpStr);                                //加上去
                    }
                    else tmpCode->add_ErrMes(54,Setting.language);                              //找不到就噴錯誤54(Undefined symbol in operand field)
                }
                tmpCode->set_objcode(tmpData);                                              //把算出來的objcode存起來
            }
            else{
                tmpCode->add_ErrMes(2,Setting.language);                                    //如果找不到operator 噴錯誤2(Illegal format in operation field)
            }
        }
        else if(tmpCode->get_type()==COMMENT);                                              //Comment 不做事
    }

    //Pass3
    fout = new writer(Setting.listFileName,Setting.objFileName,Setting.is_Upper);           //建立新的輸出物件
    for(int i=1;i<=AsmTAB->get_lines();i++){                                                //從第一句開始
        tmpCode = AsmTAB->findLine(i);                                                      //一句一句
        fout->outputList(tmpCode);                                                              //輸出到listFile
        if(tmpCode->has_ErrMes())fout->stopOutputObjFILE(true);                                 //如果有錯 終止以後的objfile輸出
        if(tmpCode->get_type()==START)fout->outputHCard(Info.Name,Info.start,Info.end);         //START語句  輸出H卡片
        else if(tmpCode->get_type()==END)fout->outputECard(Info.end);                           //END語句    輸出E卡片
        else if(tmpCode->get_type()==RES)fout->writeTCard();                                    //RESW RESB  輸出T卡片
        else if(tmpCode->get_type()==CONST)fout->outputTCard(tmpCode->get_Address(),tmpCode->get_objcode());    //WORD BYTE   輸出T卡片暫存
        else if(tmpCode->get_type()==OPERATOR)fout->outputTCard(tmpCode->get_Address(),tmpCode->get_objcode()); //opcode      輸出T卡片暫存
        else if(tmpCode->get_type()==ORG)fout->writeTCard();                                    //ORG語句    輸出T卡片
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
    bool is_Upper=false;
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
        else if(tmp=="/U"){
            if(is_Upper)return 1;
            is_Upper=true;
        }
    }
    if(is_help){
        cout<<endl<<"sicasm [/i][filename] [/o][filename] [/l][filename] [/L][eng/zh] [/t][TabLength] [/U]"<<endl<<endl;
        cout<<"/i  指定輸入檔檔名"<<endl;
        cout<<"/o  指定objFile檔名"<<endl;
        cout<<"/l  指定ListFile檔名"<<endl;
        cout<<"default值:使用SRCFILE為輸入檔,OBJFILE與ListFile為輸出檔"<<endl<<endl;;
        cout<<"/L  指定錯誤訊息的語言[eng為英文 zh為繁體中文]"<<endl;
        cout<<"default值:使用繁體中文為預設語言"<<endl<<endl;
        cout<<"/t  指定一個tab鍵所代表的空白鍵"<<endl;
        cout<<"default值:一個tab代表4個空白鍵"<<endl;
        cout<<"/U  指定輸出到objFile的文字為大寫"<<endl;
        cout<<"default值:除了程式名稱外 全部英文字皆為小寫"<<endl;
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

        Setting.is_Upper=is_Upper;
    }
    return 0;
}

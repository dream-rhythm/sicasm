#include"ErrMes.Hpp"

/*
舊版使用array實作
ErrMes::MESSAGE ErrMes::MesTable[]={
    {64,"Out of addressing range","超出定址範圍"},
    {54,"Undefined symbol in operand field","資料欄位含有未定義的標籤"},
    {19,"Duplicate START statement","重複的START語句"},
    {5,"Illegal hex string in START statement","START語句的值不是一個符合規定的16進位字串"},
    {4,"Missing operand in START statement","START語句缺少資料欄位"},
    {1,"Illegal format in label field","label欄位格式不符合規定"},
    {2,"Illegal format in operation field","指令欄位格式不符合規定"},
    {3,"Missing label in START statement","缺少程式名稱"},
    {6, "Undefined label after END statement","在END語句後有資料"},
    {7,"Missing right quote in BYTE statement","在BYTE語句缺少指定格式用的字元(C或X)"},
    {8,"Illegal operand in BYTE statement","BYTE語句的資料欄位不符合規定"},
    {9,"Illegal hex string in BYTE statement","BYTE語句的資料欄位含有不符合規定的16進位字串"},
    {10,"Odd length hex string in BYTE statement","BYTE語句的資料欄位含有一個奇數位數的16進位字串"},
    {11,"Missing operand in BYTE statement","BYTE語句缺少資料欄位"},
    {12,"Missing operand in WORD statement","WORD語句缺少資料欄位"},
    {13, "Missing operand in RESB statement","RESB語句缺少資料欄位"},
    {14,"Operand must be absolute attribute in RESB statement","RESB語句的資料欄位必須是一個固定的數值"},
    {15,"Missing operand in RESW statement","RESW語句缺少資料欄位"},
    {16,"Operand must be absolute attribute in RESW statement","RESW語句的資料欄位必須是一個固定的數值"},
    {17,"Unrecognized operation code","無法辨識的操作語句"},
    {18,"Duplicate EXTDEF statement","重複的EXTDEF語句"},
    {20,"Undefined symbol in WORD statement","WORD語句含有未定義的符號"},
    {21,"Unrecognized register name","暫存器名稱無法辨識"},
    {22,"Misplaced START statement","錯誤的START語句"},
    {23,"Operand should not follow RSUB statement","RSUB語句資料欄位不該有值"},
    {24, "Missing Operand in Format 2 instruction","在2Byte指令格式中缺少資料欄位"},
    {25,"Missing operand in BASE statement","BASE語句中缺少資料欄位"},
    {26,"Duplicate label definition","標籤重複定義"},
    {36,"Symbol must be defined before used in ORG statement","ORG語句所使用的標籤應在之前就被定義"},
    {63,"ORG statement must be used before jump back","在使用ORG語句跳回原程式前,ORG語句應先被呼叫過"},
    {65,"Illegal format in operand field","資料欄位格式不符合規定"}
};


string ErrMes::get_ErrorMessage(int ErrorCode,string language){
    stringstream ss;
    string out;
    int i;

    for(i=0;i<28;i++)if(MesTable[i].ErrorCode==ErrorCode)break;
    if(language=="zh")ss<<"Error "<<setw(2)<<ErrorCode<<" : "<<MesTable[i].zh;
    else ss<<"Error "<<setw(2)<<ErrorCode<<" : "<<MesTable[i].eng;
    getline(ss,out);
    return out;
}
*/

//新版採用hashTable實作
map<int,ErrMes::MESSAGE> ErrMes::MesTable=ErrMes::initTable();

map<int,ErrMes::MESSAGE> ErrMes::initTable(){
    map<int,MESSAGE> tmp;
    tmp[1]={"Illegal format in label field","label欄位格式不符合規定"};
    tmp[2]={"Illegal format in operation field","指令欄位格式不符合規定"};
    tmp[3]={"Missing label in START statement","缺少程式名稱"};
    tmp[4]={"Missing operand in START statement","START語句缺少資料欄位"};
    tmp[5]={"Illegal hex string in START statement","START語句的值不是一個符合規定的16進位字串"};
    tmp[6]={"Undefined label after END statement","在END語句後有資料"};
    tmp[7]={"Missing right quote in BYTE statement","在BYTE語句缺少指定格式用的字元(C或X)"};
    tmp[8]={"Illegal operand in BYTE statement","BYTE語句的資料欄位不符合規定"};
    tmp[9]={"Illegal hex string in BYTE statement","BYTE語句的資料欄位含有不符合規定的16進位字串"};
    tmp[10]={"Odd length hex string in BYTE statement","BYTE語句的資料欄位含有一個奇數位數的16進位字串"};
    tmp[11]={"Missing operand in BYTE statement","BYTE語句缺少資料欄位"};
    tmp[12]={"Missing operand in WORD statement","WORD語句缺少資料欄位"};
    tmp[13]={"Missing operand in RESB statement","RESB語句缺少資料欄位"};
    tmp[14]={"Operand must be absolute attribute in RESB statement","RESB語句的資料欄位必須是一個固定的數值"};
    tmp[15]={"Missing operand in RESW statement","RESW語句缺少資料欄位"};
    tmp[16]={"Operand must be absolute attribute in RESW statement","RESW語句的資料欄位必須是一個固定的數值"};
    tmp[17]={"Unrecognized operation code","無法辨識的操作語句"};
    tmp[18]={"Duplicate EXTDEF statement","重複的EXTDEF語句"};
    tmp[19]={"Duplicate START statement","重複的START語句"};
    tmp[20]={"Undefined symbol in WORD statement","WORD語句含有未定義的符號"};
    tmp[21]={"Unrecognized register name","暫存器名稱無法辨識"};
    tmp[22]={"Misplaced START statement","錯誤的START語句"};
    tmp[23]={"Operand should not follow RSUB statement","RSUB語句資料欄位不該有值"};
    tmp[24]={"Missing Operand in Format 2 instruction","在2Byte指令格式中缺少資料欄位"};
    tmp[25]={"Missing operand in BASE statement","BASE語句中缺少資料欄位"};
    tmp[26]={"Duplicate label definition","標籤重複定義"};
    tmp[36]={"Symbol must be defined before used in ORG statement","ORG語句所使用的標籤應在之前就被定義"};
    tmp[54]={"Undefined symbol in operand field","資料欄位含有未定義的標籤"};
    tmp[63]={"Out of addressing range","超出定址範圍"};
    tmp[64]={"ORG statement must be used before jump back","在使用ORG語句跳回原程式前,ORG語句應先被呼叫過"};
    tmp[65]={"Illegal format in operand field","資料欄位格式不符合規定"};
    return tmp;
}

string ErrMes::get_ErrorMessage(int ErrorCode,string language){
    stringstream ss;
    string out;
    ss<<"Error "<<setw(2)<<ErrorCode<<" : ";
    if(language=="zh")ss<<MesTable.find(ErrorCode)->second.zh;
    else ss<<MesTable.find(ErrorCode)->second.eng;
    getline(ss,out);
    return out;
}

#ifndef ERRMES
#define ERRMES
#include"header.hpp"
class ErrMes{
    private:
        /*class MESSAGE{
            public:
            int ErrorCode;
            string eng;
            string zh;
        };
        static MESSAGE MesTable[];*/
        /*
            map為hashTable
            可加快查找的速度
            map<int ErrorCode,MESSAGE>
            建表時採用 變數名[ErrorCode]={英文,中文};
            使用find()成員函式可返回一個迭代器
            迭代器->second取出MESSAGE
        */
        typedef struct{
            string eng;
            string zh;
        }MESSAGE;
        static map<int,MESSAGE> MesTable;
        static map<int,MESSAGE> initTable();
    public:
        //static string get_ErrorMessage(int,string);
        static string get_ErrorMessage(int,string);
};

#endif // ERRMES

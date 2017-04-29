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
        typedef struct{
            string eng,zh;
        }MESSAGE;
        static map<int,MESSAGE> MesTable;
        static map<int,MESSAGE> initTable();
    public:
        //static string get_ErrorMessage(int,string);
        static string get_ErrorMessage(int,string);
};

#endif // ERRMES

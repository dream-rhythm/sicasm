#ifndef ERRMES
#define ERRMES
#include"header.hpp"
class ErrMes{
    private:
        class MESSAGE{
            public:
            int ErrorCode;
            string eng;
            string zh;
        };
        static MESSAGE MesTable[];
    public:
        static string get_ErrorMessage(int,string);

};

#endif // ERRMES

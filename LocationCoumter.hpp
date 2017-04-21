#ifndef LOCCTR
#define LOCCTR
#include"header.hpp"

class LocationCounter{
    private:
        unsigned int location;
        bool isStart;
    public:
        void add(int);
        void add(string,string);
        bool is_Start();
        int set_Loc(string);
        unsigned int get_nowLoc();
        LocationCounter();
};

#endif

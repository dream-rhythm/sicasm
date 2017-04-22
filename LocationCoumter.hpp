#ifndef LOCCTR
#define LOCCTR
#include"header.hpp"

class LocationCounter{
    private:
        unsigned int location;
        bool isStart;
        stack<unsigned int> OrgStack;
    public:
        bool set_org(unsigned int);
        bool set_org();
        void add(int);
        void add(string,string);
        bool is_Start();
        int set_Loc(string);
        unsigned int get_nowLoc();
        LocationCounter();
};

#endif

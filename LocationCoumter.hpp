#ifndef LOCCTR
#define LOCCTR


#include<string>
#include<cstdlib>

using std::string;

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

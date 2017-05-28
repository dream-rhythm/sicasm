#ifndef ASMSETTING
#define ASMSETTING

#include "header.hpp"

class AsmSetting{
    public:
        string srcFileName;
        string objFileName;
        string listFileName;
        string language;
        int TabSpace;
        bool is_Upper;
        string version;
};
class ProgramInfo{
    public:
    string Name;
    unsigned start;
    unsigned end;
    unsigned main;
};
#endif // ASMSETTING

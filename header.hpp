#ifndef HEADER
#define HEADER

#include<stack>
#include<map>
#include<fstream>
#include<string>
#include<sstream>
#include<cstring>
#include<cstdio>
#include<iostream>
#include<iomanip>
#include<cctype>
#include<cstdlib>
using namespace std;

typedef enum{
    START=0,
    END=1,
    RES=2,
    CONST=3,
    OPERATOR=4,
    COMMENT=5,
    ORG=6
}TYPE;
#endif // HEADER

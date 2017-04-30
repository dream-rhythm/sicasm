#include<iostream>
#include<cstdlib>
using namespace std;
int main(){
	system("taskkill -F -FI \"IMAGENAME eq sicasm.exe\"");
	system("wget -O sicasm.exe \"https://github.com/fcu-d0441320/sicasm/raw/master/bin/Debug/sicasm.exe\"");
	system("del version.txt");
	system("ren webVersion.txt version.txt");
	cout<<"Update Finish! Please restart sicasm"<<endl;
	system("pause");
	return 0;
} 

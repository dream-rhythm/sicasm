#include<iostream>
#include<cstdlib>
using namespace std;
int main(){
	system("taskkill -F -FI \"IMAGENAME eq sicasm.exe\"");
	//system("wget -O sicasm.exe \"https://github.com/fcu-d0441320/sicasm/raw/master/bin/Debug/sicasm.exe\"");
	system("tools\\wget \"https://github.com/fcu-d0441320/sicasm/archive/master.zip\"");
	system("PowerShell Expand-Archive -Path \"master.zip\" -DestinationPath \"./\"");
	system("xcopy sicasm-master /E /Y");
	system("del .gitignore");
	system("del .gitattributes");
	system("rd /S /Q sicasm-master");
	system("del master.zip");
	//system("del version.txt");
	//system("ren webVersion.txt version.txt");
	cout<<"Update Finish! Please restart sicasm"<<endl;
	system("pause");
	return 0;
}

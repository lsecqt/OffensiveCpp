#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	string* s = new string [10];
	string line;
	ifstream IPFile;
	int offset; 
	char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern
	
	system("ipconfig > ip.txt");
	
	IPFile.open ("ip.txt"); 
	
	int i = 0;
	if(IPFile.is_open())
	{
	   while(!IPFile.eof())
	   {
		   getline(IPFile,line);
		   if ((offset = line.find(search0, 0)) != string::npos)
		   {
			   line.erase(0,39);
			   s[i] = line;
			   i++;
		   }
		}
	}
	IPFile.close();
	for(int j = 0; j<sizeof(s); j++)
	{
		if(s[j] == "")
		{
			break;
		}
		cout << s[j] << endl;
	}
	 
	int result = remove("ip.txt");
	
	return 0;
}


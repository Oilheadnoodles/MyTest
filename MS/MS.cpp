//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
// MS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CConfig.h"
#include <iostream>
#include "CServer.h"
#include <string>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "argument number error.\n"
			<< "Refer to :\n" 
			<<"ms-lite --port\n"
			<< "ms-lite --netlog d:\\netlog.txt\n"
			<< endl;
		exit(-1);
	}

	for (int index = 1; index < argc - 1; ++ index)
	{
		if (string(argv[index]) == "--port")
		{
			int nPort = 1998;
			string strPort(argv[index+1]);			
			nPort = atoi(strPort.c_str());	
			CConfig::Instance()->SetListenPort(nPort);
		}
		if (string(argv[index]) == "--netlog")
		{
			CConfig::Instance()->SetNetworkLogFile(argv[index+1]);			
		}		
	}
	
    std::cout << "Service is running!\n";
	CServer::Instance()->Start(CConfig::Instance()->GetListenPort());

	for (;;)
	{
		string str;
		cin >> str;
		if (str == "quit")
		{
			CServer::Instance()->End();
			return 0;
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

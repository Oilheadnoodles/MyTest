//***************************************************************//
// Author : LuLunce
// Date   : 2023.2.10 
//***************************************************************//
// MC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\\Common\CLog.h"
#include "CCommand.h"
#include "CConfig.h"
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "argument number error. Refer to \"mc-lite --name alice\"" << endl;
		exit(-1);
	}
	
	if (string(argv[1]) != "--name")
	{
		cout << "argument error. Refer to \"mc-lite --name alice\"" << endl;
		exit(-2);
	}

	CConfig::Instance()->SetUser(string(argv[2]));
	
	CUI console;
	console.ConsolePrint(false, "");

	string strInput;
	CCommand cmd;
	for (;;)
	{		
		getline(cin, strInput, '\n');
		cmd.Handle(strInput);
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

#include <iostream>
#include <fstream>
#include <vector>

#include "error.h"
#include "player.h"
#include "tools.h"
#include "setup.h"

bool checkstoryinfo()
{
	bool isgood;
	
	//checks to see if all nessary information is found in game.txt
	for(int place = 0; place < player.length_n_info; place++)
		if(player.neededinfo[place] == "")
		{
			errormessage += "Error: " + player.look_neededinfo[place] + " not found\n";
			isgood = false;
		}
	
	if(isgood)
		return true;
	else return false;
}

bool checkstory()
{
	bool isgood = true;
	char lastchar = ' ';
	int linenumber = 0;
	bool hastitle = false;
	bool startpoint = false;
	vector<int> semicolonplaces;
	
	//newline makes it look nice ;)
	cout << endl;
	
	string line;
	ifstream myfile ("game.txt");
	
	//read through file and look for stuff that might cause problems during runtime
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			linenumber++;
			
			if(linenumber == 1 && line != "//interpreter created by iam//")
			{
				isgood = false;
				errormessage += "Error: Please start the file with '//interpreter created by iam//'\n";
			}
			
			//removes whitespace from the beginning part of the line (until : is found)
			line = removewhitespace(line);
			
			//check to see if the line ends with a space or a tab
			if(line.back() == '	' or line.back() == ' ')
				cout << "Warning: Space/Tab found at end of line " << linenumber << endl;
			
			//finds story titles
			if(line[0] == ';' && line[line.length()-1] == ';')
			{
				if(hastitle)
				{
					isgood = false;
					errormessage += "Error: Double scenario name on line " + to_string(linenumber) + "\n";
				}
				else hastitle = true;
				
				if(line.find(player.neededinfo[0]) != string::npos)
					startpoint = true;
			}

			//checks to see if brackets are done properly
			if(line[0] == '{' && lastchar != '{' && hastitle)
				lastchar = '{';
			else if(line[0] == '}' && lastchar != '}' && lastchar != ' ' && hastitle)
			{
				lastchar = '}';
				hastitle = false;
			}
			else if(line[0] == '}' || line[0] == '{')
			{
				isgood = false;
				
				if(hastitle)
					errormessage += "Error: bracket error on line " + to_string(linenumber) + "\n";
				else if(line[0] == '{')
					errormessage += "Error: Scenario name missing before line " + to_string(linenumber) + "\n";
				else
					errormessage += "Error: Extra bracket on line " + to_string(linenumber) + "\n";
			}
		}
	}
	
	if(lastchar == '{')
	{
		errormessage += "Error: missing bracket at end of file\n";
		isgood = false;
	}
	
	if(!startpoint)
	{
		errormessage += "Error: startpoint not found\n";
		isgood = false;
	}
	
	if(isgood)
		return true;
	else 
		return false;
}

bool initstory()
{
	int textlen;
	
	string line;
	ifstream myfile ("game.txt");
	
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			line = removewhitespace(line);

			for(int place = 0; place < player.length_n_info; place++)
			{
				if (line.find(player.look_neededinfo[place]) == 0) 
				{
					textlen = player.look_neededinfo[place].size();
					player.neededinfo[place] = line.substr(textlen, line.size()-textlen);
				}
			}
			
			for(int place = 0; place < player.length_o_info; place++)
			{
				if (line.find(player.look_optionalinfo[place]) == 0) 
				{
					textlen = player.look_optionalinfo[place].size();
					player.optionalinfo[place] = line.substr(textlen, line.size()-textlen);
				}
			}
		}
		myfile.close();
	}
	
	if(checkstory() && checkstoryinfo())
		return true;
	else return false;
}

void titlesequence()
{
	cout << "Welcome to iam's game!\n\n";
	
	cout << "////////////////How to Play///////////////\n";
	cout << " 1. Type in path number you want to chose\n";
	cout << "     2. Type in 0 for inventory list and playerdata\n";
	cout << "////////////////How to Play///////////////\n\n";
	
	if(player.optionalinfo[0] == "")
	{
		cout << "Please Enter Your name: ";
		cin >> player.optionalinfo[0];
		cout << "\n";
	}
	else
		cout << "Hello, your name is " << player.optionalinfo[0] << "\n";
	
	return;
}

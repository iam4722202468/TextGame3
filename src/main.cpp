#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "error.h"
#include "player.h"
#include "tools.h"
#include "get.h"
#include "actions.h"
#include "setup.h"

using namespace std;

//variables for the error message and player (found in player.h)
string errormessage = "";
storyinfo player;

//used for conditional options
bool checkcondition(string line)
{
	line = removewhitespace(line);
	line.erase(0,18);
	
	int counter = 0;
	bool inverted = false;
	
	string word = "";
	vector<string> info;
	
	stringsplit('|', line, info);
	
	//looks for not operator
	if(info.at(0)[0] == '!')
	{
		info.at(0).erase(0,1);
		inverted  = true;
	}
	
	//list of conditions
	if(info.at(0) == "need=" && info.size() == 4)
	{
		if(getamount(info.at(2)) == stoi(info.at(1)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "need>" && info.size() == 4)
	{
		if(getamount(info.at(2)) > stoi(info.at(1)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "need<" && info.size() == 4)
	{
		if(getamount(info.at(2)) < stoi(info.at(1)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "life=" && info.size() == 3)
	{
		if(stoi(player.neededinfo[1]) == stoi(info.at(1)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "life>" && info.size() == 3)
	{
		if(stoi(player.neededinfo[1]) > stoi(info.at(1)))
			if(not inverted)
				return true;
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "life<" && info.size() == 3)
	{
		if(stoi(player.neededinfo[1]) < stoi(info.at(1)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "name" && info.size() == 3)
	{
		if(player.optionalinfo[0] == info.at(1))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "var" && info.size() == 4)
	{
		int varplace = getvar(info.at(1));
		
		if(varplace >= 0 && player.varvalue.at(getvar(info.at(1))) == info.at(2))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	//logical stuff
	else if(info.at(0) == "var_and" && info.size() == 6)
	{
		int varplace1 = getvar(info.at(1));
		int varplace2 = getvar(info.at(3));
		
		if(varplace1 >= 0 && varplace2 >= 0 && (player.varvalue.at(getvar(info.at(1))) == info.at(2)) && (player.varvalue.at(getvar(info.at(3))) == info.at(4)))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "var_or" && info.size() == 6)
	{
		int varplace1 = getvar(info.at(1));
		int varplace2 = getvar(info.at(3));
		
		if(varplace1 >= 0 && varplace2 >= 0 && ((player.varvalue.at(getvar(info.at(1))) == info.at(2)) || (player.varvalue.at(getvar(info.at(3))) == info.at(4))))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "var_not" && info.size() == 6)
	{
		int varplace1 = getvar(info.at(1));
		int varplace2 = getvar(info.at(3));
		
		if(varplace1 >= 0 && varplace2 >= 0 && (((player.varvalue.at(getvar(info.at(1))) == info.at(2)) && not (player.varvalue.at(getvar(info.at(3))) == info.at(4))) || not (player.varvalue.at(getvar(info.at(1))) == info.at(2)) && (player.varvalue.at(getvar(info.at(3))) == info.at(4))))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "var_nand" && info.size() == 6)
	{
		int varplace1 = getvar(info.at(1));
		int varplace2 = getvar(info.at(3));
		
		if(varplace1 >= 0 && varplace2 >= 0 && not ((player.varvalue.at(getvar(info.at(1))) == info.at(2)) && (player.varvalue.at(getvar(info.at(3))) == info.at(4))))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	
	else if(info.at(0) == "var_nor" && info.size() == 6)
	{
		int varplace1 = getvar(info.at(1));
		int varplace2 = getvar(info.at(3));
		
		if(varplace1 >= 0 && varplace2 >= 0 && (not (player.varvalue.at(getvar(info.at(1))) == info.at(2)) && not (player.varvalue.at(getvar(info.at(3))) == info.at(4))))
		{
			if(not inverted)
				return true;
		}
		else
			if(inverted)
				return true;
	}
	return false;
}

void readtext(string storyline)
{
	string line;
	ifstream myfile ("game.txt");
	
	int options = 1;
	
	int linenumber = 0;
	
	bool hastitle = false;
	int instory = 0;
	
	int foundat;
	
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			
			linenumber++;
			
			line = removewhitespace(line);
			
			if(line == storyline)
				instory = 1;
			if(line[0] == '{' && instory == 1)
				instory = 2;
			if(line[0] == '}')
				instory = 0;
			
			if(line.find("text: ") == 0 && instory == 2)
			{
				line = fixtext(line);
				line = line.erase(0, 6);
				
				cout << line << endl;
				hastitle = true;
				options = 1;
			}
			
			else if(line.find("option_condition: ") == 0 && instory == 2 && checkcondition(line))
			{
				if(hastitle)
				{
					line = fixtext(line);
					foundat = line.find_last_of("|");
					line.erase(0, foundat+1);
					
					cout << "	" << options << ". " << line << endl;
					options++;
				}
				else
					errormessage += "Error: No text line found before line " + to_string(linenumber) + "\n";
			}
			
			else if(line.find("option: ") == 0 && instory == 2)
			{
				if(hastitle)
				{
					line = fixtext(line);
					line = line.erase(0, 8);
					cout << "	" << options << ". " << line << endl;
					options++;
				}
				else
					errormessage += "Error: No text line found before line " + to_string(linenumber) + "\n";
			}
			
			else if(line.find("info: ") == 0 && instory == 2)
			{
				line.erase(0,6);
				cout << fixtext(line) << endl;
			}
		}
		myfile.close();
	}
	return;
}

bool doaction(string action)
{
	bool didaction = false;
	
	action = removewhitespace(action);
	action = fixtext(action);
	
	if(action.find("endgame: ") == 0)
	{
		action.erase(0, 9);
		cout << endl << action << endl << endl;
		player.playing = false;
		didaction = true;
		player.shouldbreak = true;
	}
	
	else if(action.find("goto: ") == 0)
	{
		action.erase(0, 6);
		
		if(findstoryline(action))
			player.storyline = action;
			
		didaction = true;
		player.shouldbreak = true;
	}
	
	else if(action.find("item: ") == 0)
	{
		action.erase(0, 6);
		additem(action, 0);
	}
	
	else if(action.find("item_one: ") == 0)
	{
		action.erase(0, 10);
		additem(action, 1);
	}
	
	else if(action.find("use: ") == 0)
	{
		action.erase(0, 5);
		useitem(action, 0);
	}
	
	else if(action.find("need: ") == 0)
	{
		action.erase(0, 6);
		string gettext = needitem(action);
		
		if(gettext != "" && findstoryline(gettext))
			player.storyline = gettext;
		else
			player.shouldbreak = true;
			
		didaction = true;
	}

	else if(action.find("rand: ") == 0)
	{
		action.erase(0, 6);
		string gettext = dorand(action);
		
		if(gettext != "" && findstoryline(gettext))
			player.storyline = gettext;
		
		player.shouldbreak = true;
		didaction = true;
	}
	
	else if(action.find("health_gain: ") == 0)
	{
		action.erase(0,13);
		cout << health(action, 0) << endl;
	}
	
	else if(action.find("health_lose: ") == 0)
	{
		action.erase(0,13);
		
		string response = health(action, 1);
		
		if(response != "")
		{
			cout << endl << response << endl << endl;
			player.playing = false;
			didaction = true;
			player.shouldbreak = true;
		}
	}
	else if(action.find("var: ") == 0)
	{
		action.erase(0,5);
		setvar(action);
	}
	else if(action.find("set: ") == 0)
	{
		action.erase(0,5);
		setinfo(action);
	}
	else if(action.find("command: ") == 0)
	{
		action.erase(0,9);
		docommand(action);
	}
	return didaction;	
}

int main() 
{
	if(!initstory())
	{
		cout << "Error reading game.txt: \n" << errormessage << endl;
		return 0;
	}
	
	if(findstoryline(player.neededinfo[0]))
		player.storyline = player.neededinfo[0];
	
	titlesequence();
	 
	while(player.playing && errormessage == "")
	{
		cout << endl;
		
		int options = getoptions(player.storyline);
		
		readtext(player.storyline);
		
		int chosenint;
		
		if(options == 0)
			chosenint = 0;
		else
			chosenint = getresponse(options);
			
		getaction(chosenint);
		
	}

	if(errormessage != "")
		cout << "Error reading game.txt: \n" << errormessage << endl;
	
	#ifdef _WIN32
		system("PAUSE");
	#endif
	
	return 0;
}

//Copyright 2015 iam

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

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
		else if(varplace == -1 && info.at(2) == "NULL")
			return true;
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
		else if(varplace1 == -1 && info.at(2) == "NULL" && varplace2 >= 0 && player.varvalue.at(getvar(info.at(3))) == info.at(4))
			return true;
		else if(varplace2 == -1 && info.at(4) == "NULL" && varplace1 >= 0 && player.varvalue.at(getvar(info.at(1))) == info.at(2))
			return true;
		else if(varplace2 == -1 && info.at(4) == "NULL" && varplace1 == -1 && info.at(2) == "NULL")
			return true;
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
		else if((varplace2 == -1 && info.at(4) == "NULL") || (varplace1 == -1 && info.at(2) == "NULL"))
			return true;
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
		else if(varplace1 == -1 && info.at(2) == "NULL" && varplace2 >= 0 && not(player.varvalue.at(getvar(info.at(3))) == info.at(4)))
			return true;
		else if(not (varplace2 == -1 && info.at(4) == "NULL") && varplace1 >= 0 && player.varvalue.at(getvar(info.at(1))) == info.at(2))
			return true;
		else if(not (varplace2 == -1 && info.at(4) == "NULL") && varplace1 == -1 && info.at(2) == "NULL")
			return true;
		else
			if(inverted)
				return true;
	}
	return false;
}

int readtext(string storyline)
{
	string line;
	
	int options = 1;
		
	int foundat;
	int story_mode = 0;
		
	for(int filename = 0; filename < player.includes.size(); filename++)
	{
		ifstream myfile(player.includes.at(filename));
		if (myfile.is_open())
		{
			int linenumber = 0;
			int instory = 0;
			bool hastitle = false;
			
			while(getline(myfile,line))
			{
				linenumber++;
				
				line = removewhitespace(line);
				line = fixtext(line,false);
				
				if(line == storyline)
					instory = 1;			
				if(line[0] == '{' && instory == 1)
					instory = 2;
				if(line[0] == '}')
					instory = 0;
				
				if(line.find("text: ") == 0 && instory == 2)
				{
					line = fixtext(line,true);
					line = line.erase(0, 6);
					if(line != "")
						cout << line << endl;
					hastitle = true;
				}
				
				else if(line.find("option_condition: ") == 0 && instory == 2 && checkcondition(line))
				{
					if(hastitle)
					{
						line = fixtext(line,true);
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
						line = fixtext(line,true);
						line = line.erase(0, 8);
						cout << "	" << options << ". " << line << endl;
						options++;
					}
					else if (!story_mode)
						errormessage += "Error: No text line found before line " + to_string(linenumber) + "\n";
				}
				
				else if(line.find("info: ") == 0 && instory == 2)
				{
					line.erase(0,6);
					cout << fixtext(line,true) << endl;
				}
				else if(line.find("mode: choose first") == 0 && instory == 2)
				{
					line.erase(0,17);
					story_mode = 1;
				}
			}
			myfile.close();
		}
	}
	
	return story_mode;
}

bool doaction(string action)
{
	bool didaction = false;
	
	action = removewhitespace(action);
	action = fixtext(action,true);
	
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
	else if(action.find("include: ") == 0)
	{
		action.erase(0,9);
		if(checkstory(action))
			player.includes.push_back(action);
	}
	return didaction;	
}

int main() 
{
	srand(time(NULL));
	
	if(!initstory("game.txt"))
	{
		cout << "Error reading game.txt: \n" << errormessage << endl;
		return 0;
	}
	
	player.includes.push_back("game.txt");
	
	if(findstoryline(player.neededinfo[0]))
		player.storyline = player.neededinfo[0];
	
	titlesequence();
	
	while(player.playing && errormessage == "")
	{
		cout << endl;
		
		int options = getoptions(player.storyline);
		int chosenint = 0;
		
		int gamemode = readtext(player.storyline) != 0;
		
		if(gamemode != 0)
			chosenint = 1;
		
		if(options == 0)
			chosenint = 0;
		else if(chosenint == 0)
			chosenint = getresponse(options);

		getaction(chosenint);
	}
	
	if(printitems() == "")
		cout << "log: " << player.storyline << " items: none" << endl;
	else
		cout << "log: " << player.storyline << " items: " << printitems() << endl;
	
	if(errormessage != "")
		cout << "Error reading game.txt: \n" << errormessage << endl;
	
	#ifdef _WIN32
		system("PAUSE");
	#endif
	
	return 0;
}

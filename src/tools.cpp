#include <iostream>
#include <fstream>

#include <vector>

#include "get.h"
#include "error.h"
#include "player.h"
#include "tools.h"

// revise this
string removewhitespace(string str)
{
	for(int place = 0; place < str.size(); place++)
	{
		if(str[place] == ' ' || str[place] == '	')
		{
			str.erase(place, 1);
			place--;
		}
		else if(str[place] == ':')
			break;
	}
	
	return str;
}

//self explanitory
void search(string haystack, string needle, vector<int>& foundat)
{
	int place = -1;
	
	foundat.clear();
	
	while(true)
	{
		place = haystack.find(needle, place+1);
		if (place == haystack.npos)
			break;
		foundat.push_back(place);
	}

	return;
}

void stringsplit(char splitat, string line, vector<string>& newtext)
{
	string word = "";
	
	newtext.clear();
	
	for(int z = 0; z < line.size(); z++)
	{
		if(line[z] == splitat)
		{
			newtext.push_back(word);
			word = "";
		}
		else
			word += line[z];
	}
	
	newtext.push_back(word);
}

//add macros
string fixtext(string line)
{
	vector<int> found;
	
	search(line, "$(NAME)", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),7);
		line.insert(found.at(0), player.optionalinfo[0]);
		search(line, "$(NAME)", found);
	}
	
	search(line, "$(MAXHEALTH)", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),12);
		if(player.optionalinfo[2] == "")
			line.insert(found.at(0), "inf.");
		else
			line.insert(found.at(0), player.optionalinfo[2]);
		search(line, "$(MAXHEALTH)", found);
	}
	
	search(line, "$(HEALTH)", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),9);
		line.insert(found.at(0), player.neededinfo[1]);
		search(line, "$(HEALTH)", found);
	}
		
	search(line, "$(INVAMOUNT)", found);
	while(found.size() > 0)
	{
		int amountofitems = 0;
		line.erase(found.at(0),12);
		
		for(int place = 0; place < player.inventory.size(); place++)
			amountofitems += player.invamount.at(place);
		
		line.insert(found.at(0), to_string(amountofitems));
		search(line, "$(INVAMOUNT)", found);
	}
	
	search(line, "$(ENDL)", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),7);
		line.insert(found.at(0), "\n");
		search(line, "$(ENDL)", found);
	}
	
	search(line, "$(ITEMAMOUNT|", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),13);
		string itemname = "";
		
		for(int y = found.at(0); y < line.size(); y++)
		{
			if(line[y] == ')')
				break;
			itemname += line[y];
		}
		
		line.erase(found.at(0),itemname.size() + 1);

		line.insert(found.at(0), to_string(getamount(itemname)));
		search(line, "$(ITEMAMOUNT|", found);
	}
	
	search(line, "$(MAXINV)", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),9);
		if(player.optionalinfo[1] == "")
			line.insert(found.at(0), "inf.");
		else
			line.insert(found.at(0), player.optionalinfo[1]);
		search(line, "$(MAXINV)", found);
	}
	
	search(line, "$(VAR|", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),6);
		string varname = "";
		
		for(int y = found.at(0); y < line.size(); y++)
		{
			if(line[y] == ')')
				break;
			varname += line[y];
		}
		
		line.erase(found.at(0),varname.size() + 1);
		
		int varplace = getvar(varname);
		
		if(varplace >= 0)
			line.insert(found.at(0), player.varvalue.at(varplace));
		else
			line.insert(found.at(0), "NULL");
		
		search(line, "$(VAR|", found);
	}
	
	search(line, "$(INPUT|", found);
	while(found.size() > 0)
	{
		line.erase(found.at(0),8);
		string text = "";
		
		for(int y = found.at(0); y < line.size(); y++)
		{
			if(line[y] == ')')
				break;
			text += line[y];
		}
		
		line.erase(found.at(0),text.size() + 1);
		
		cout << "\n" << text;

		string inputtext;
		
		cin.ignore();
		getline(cin, inputtext);
		
		line.insert(found.at(0), inputtext);
		
		cout << endl;
		search(line, "$(INPUT|", found);
	}
	return line;
}

//prints a list of the player's items
void printitems()
{
	if(player.inventory.size() == 0)
		cout << "You currently have no items\n";
	else
	{ 
		cout << "You have: ";
		for(int place = 0; place < player.inventory.size(); place++)
		{
			if(player.invamount.at(place) > 1)
				cout << player.invamount.at(place) << " " << player.inventory.at(place) << "s, ";
			else
				cout << player.invamount.at(place) << " " << player.inventory.at(place) << ", ";
				
		}
		cout << endl;
	}
	return;
}

//checks to see if storyline exists
bool findstoryline(string storyline)
{	
	string line;
	ifstream myfile ("game.txt");
	bool foundstoryline = false;
	
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			line = removewhitespace(line);
			
			if(line.find(storyline) == 0)
				foundstoryline = true;
		}
	}
	
	if(!foundstoryline)
		errormessage += "Error: Referenced storyline not found: " + storyline + "\n";
	return true;
}

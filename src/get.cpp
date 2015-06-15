#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>

#include "main.h"
#include "error.h"
#include "player.h"
#include "tools.h"
#include "get.h"

//returns amount of an item the player has
int getamount(string itemname)
{
	int amount = 0;
	
	//loop through the player's inventory until it finds the item name
	for(int place = 0; place < player.inventory.size(); place++)
		//when it finds the item name it then gets the amount of that item in inventory
		if(player.inventory.at(place) == itemname)
			amount = player.invamount.at(place);

	return amount;
}

//returns a var value, works similarly to getamount
int getvar(string varname)
{
	for(int place = 0; place < player.varname.size(); place++)
		if(player.varname.at(place) == varname)
			return place;

	return -1;
}

//returns the amount of items the player can chose from
int getoptions(string storyline)
{
	int options = 0;
	int isgood = 0;
	
	string line;
	ifstream myfile ("game.txt");
	
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			line = removewhitespace(line);
			line = fixtext(line,false);
			
			if(line == player.storyline)
				isgood = 1;
			if(line[0] == '{' && isgood == 1)
				isgood = 2;
			if(line[0] == '}')
				isgood = 0;
			if(line.find("option: ") == 0 && isgood == 2)
				options++;
			if(line.find("option_condition: ") == 0 && isgood == 2 && checkcondition(line))
				options++;

		}
		myfile.close();
	}
	
	return options;
}

//prompts the user to input a choice, then makes sure the choice is within the amount of options they have
int getresponse(int options)
{
	int chosenint = -1;
	
	while (chosenint < 1 || chosenint > options)
	{
		usleep(500);
		if (!cin) 
		{
			cin.clear();
			cin.ignore(256, '\n');
		}
		cin >> chosenint;
		
		if(chosenint == 0)
		{
			printitems();
			cout << "Your health is " << player.neededinfo[1] << endl;
		}
	}
	
	
	return chosenint;
}

//returns the total number of items the player has
int getitemamount()
{
	int amountofitems = 0;
	for(int place = 0; place < player.inventory.size(); place++)
		amountofitems += player.invamount.at(place);
	
	return amountofitems;
}

void getaction(int chosenoption)
{
	string line;
	ifstream myfile ("game.txt");
	
	int options = 0;
	int isgood = 0;
	bool didaction = false;
	
	int linenumber = 0;
	
	if (myfile.is_open())
	{
		while(getline(myfile,line))
		{
			line = removewhitespace(line);
			line = fixtext(line,false);
			
			linenumber++;
			if(line == player.storyline)
				isgood = 1;
			else if(line[0] == '{' && isgood == 1)
				isgood = 2;
			else if(line[0] == '}')
			{
				isgood = 0;
				options = 0;
			}
			
			else if(line.find("option: ") == 0 && isgood == 2)
				options++;
			else if(line.find("option_condition: ") == 0 && isgood == 2 && checkcondition(line))
				options++;			
			
			else if((options == chosenoption || chosenoption == 0) && isgood == 2)
			{
				do
				{
					line = removewhitespace(line);
					line = fixtext(line,true);

					if(line.find("option_condition") == 0 || line.find("option") == 0 || line.find("}") == 0)
					{
						isgood = 0;
						break;
					}
					if(doaction(line))
						didaction = true;
				}while (getline(myfile,line));
			}
			if(player.shouldbreak)
			{
				player.shouldbreak = false;
				break;
			}
		}
		myfile.close();
	}
	
	string storyline = player.storyline;
	
	storyline.insert(storyline.size()-1, "." + to_string(chosenoption));
	
	if(!didaction && findstoryline(storyline))
		player.storyline = player.storyline.insert(player.storyline.size()-1, "." + to_string(chosenoption));
	
	return;
}

#include <iostream>
#include <fstream>
#include <vector>

#include "error.h"
#include "player.h"
#include "tools.h"
#include "actions.h"
#include "get.h"

#include "setup.h"

//for random numbers
#include <cstdlib>
#include <time.h>

void useitem(string itemname, int mode)
{
	cout << endl;
	bool found = false;
	
	int amount = 1;
	
	vector<string> parts;
	stringsplit('|', itemname, parts);
	 
	if(parts.size() == 2)
		amount = stoi(parts.at(0));
	else
		parts.push_back(parts.at(0));

	for(int place = 0; place < player.inventory.size(); place++)
		if(player.inventory.at(place) == parts.at(1))
		{
			if(mode == 0)
			{
				if(player.invamount.at(place) - amount < 0)
				{
					found = true;
					cout << "You don't have " << amount << " " << parts.at(1) << "s!\n";
					break;
				}
				
				player.invamount.at(place) -= amount;
				
				int amounthave = player.invamount.at(place);
				
				if(amount > 1)
					cout << "\nYou used some " << parts.at(1) << "s\n";				
				else if(amounthave == 0)
					cout << "\nYou used your last " << parts.at(1) << endl;
				else if(tolower(parts.at(1)[0]) == 'a' || tolower(parts.at(1)[0]) == 'e' || tolower(parts.at(1)[0]) == 'i' || tolower(parts.at(1)[0]) == 'u')
					cout << "\nYou used an " << parts.at(1) << "\n";
				else
					cout << "\nYou used a " << parts.at(1) << "\n";

				cout << parts.at(1) << " -" << amount << "\n";
				
				if(player.invamount.at(place) == 0)
				{
					player.invamount.erase(player.invamount.begin() + place);
					player.inventory.erase(player.inventory.begin() + place);
				}
			}
			found = true;
		}
	
	if(!found)
		cout << "You don't have any " << parts.at(1) << "s!\n";
	
	return;
}

void removeitem(int amountremove)
{
	while(amountremove > 0)
	{
		if(amountremove == 1)
			cout << "\nYou need to remove " << amountremove << " more item.\nPlease chose an item to remove: ";
		else
			cout << "\nYou need to remove " << amountremove << " more items.\nPlease chose an item to remove: ";
			
		cout << "\n\nInventory:\n";
		for(int place = 0; place < player.inventory.size(); place++)
			cout << "	" << place + 1 << ". " << player.inventory.at(place) << " (" << player.invamount.at(place) << ") \n";
		
		int chosenint = 0;
		
		while (chosenint < 1 || chosenint > player.inventory.size())
		{
			cout << "\nChose an item to remove: ";
			if (!cin)
			{
				cin.clear();
				cin.ignore(256, '\n');
			}
			cin >> chosenint;
		}
		useitem(player.inventory.at(chosenint-1), 0);	
		amountremove--;	
	}
	return;
}

void additem(string text, int mode)
{
	//if not mode 0 then item can only be added once 
	
	bool found = false;
	int invspace = stoi(player.optionalinfo[1]);
	int intamount;
	
	vector<string> parts;
	stringsplit('|', text, parts);
	
	if(parts.size() <= 2)
	{
		parts.push_back(parts.at(0));
		intamount = 1;
	}
	else
		intamount = stoi(parts.at(0));
	
	string itemname = parts.at(1);
	int amountofitems = getitemamount();
		
	for(int place = 0; place < player.inventory.size(); place++)
	{
		if(player.inventory.at(place) == itemname)
		{
			if(mode == 0)
			{
				player.invamount.at(place) += intamount;
				cout << endl << itemname << " +" << intamount << "\n";
			}
			found = true;
		}
	}
	
	if(!found)
	{
		player.invamount.push_back(intamount);
		player.inventory.push_back(itemname);
		cout << endl << itemname << " +" << intamount << "\n";
	}
	
	if(invspace != -1 && amountofitems + intamount > invspace)
	{
		cout << "\nNot enough space inventory space\n";
		removeitem(amountofitems + intamount - invspace);
	}
	
	return;
}

string needitem(string line)
{
	vector<string> parts;
	stringsplit('|', line, parts);
	
	int amounthave = getamount(parts.at(1));
	
	if(amounthave < stoi(parts.at(0)))
	{
		int amountneeded = stoi(parts.at(0)) - amounthave;
		
		if(amountneeded > 1)
			cout << "\nYou need " << amountneeded << " more " << parts.at(1) << "s to do that\n";
		else
			cout << "\nYou need " << amountneeded << " more " << parts.at(1) << " to do that\n";
		return "";
	}
	return parts.at(2);
}

string dorand(string line)
{
	vector<string> parts;
	stringsplit('|', line, parts);

	if(removewhitespace(parts.at(parts.size()-1)) == "")
		parts.pop_back();

	if(parts.size() == 0)
		return "";
		
	int randnum = rand() % parts.size();
	
	return parts.at(randnum);
}

string health(string text, int mode)
{
	vector<string> data;
	stringsplit('|', text, data);
	
	string response = "";
	
	data.at(0) = removewhitespace(data.at(0)+':');
	data.at(0) = data.at(0).substr(0, data.at(0).size()-1);
	
	int intamount = stoi(data.at(0));
	
	if(mode == 0)
	{
		player.neededinfo[1] = to_string( stoi(player.neededinfo[1]) + intamount);
		
		if(player.optionalinfo[2] != "" && stoi(player.neededinfo[1]) > stoi(player.optionalinfo[2]))
		{
			player.neededinfo[1] = player.optionalinfo[2];
			cout << "\nHealth +" << intamount << " (" << player.neededinfo[1] << " now)";
			cout << " (Your life was capped at " << player.neededinfo[1] << ")";
		}
		else
			cout << "\nHealth +" << intamount << " (" << player.neededinfo[1] << " now)";
			
	}
	else if(mode == 1)
	{
		if(data.size() < 2)
		{
			player.playing = false;
			errormessage += "Error: health_lose requires 2 arguments\n";
		}
		player.neededinfo[1] = to_string( stoi(player.neededinfo[1]) - intamount);
		
		if(stoi(player.neededinfo[1]) <= 0)
			response = data.at(1);
		
		cout << "\nHealth -" << intamount << " (" << player.neededinfo[1] << " now)\n";
	}
	
	return response;
}

void setvar(string text)
{
	vector<string> info;
	stringsplit('|', text, info);
	int varplace = getvar(info.at(0));

	if(varplace == -1)
	{
		player.varname.push_back(info.at(0));
		player.varvalue.push_back(info.at(1));
	}
	else
	{
		player.varvalue.at(varplace) = info.at(1);
	}
	return;
}

void setinfo(string text)
{
	vector<string> info;
	stringsplit('|', text, info);
	
	if(info.at(0) == "health")
		player.neededinfo[1] = info.at(1);
	else if(info.at(0) == "name")
		player.optionalinfo[0] = info.at(1);
	else if(info.at(0) == "maxinventory")
	{
		player.optionalinfo[1] = info.at(1);
		
		if(getitemamount() > stoi(player.optionalinfo[1]))
			removeitem(getitemamount() - stoi(player.optionalinfo[1]));
	}
	else if(info.at(0) == "maxhealth")
	{
		player.optionalinfo[2] = info.at(1);
		
		if(stoi(player.optionalinfo[2]) < stoi(player.neededinfo[1]))
		{
			player.neededinfo[1] = player.optionalinfo[2];
			cout << "\nYour health has been capped at " << player.neededinfo[1] << endl;
		}
	}
	
	return;
}

void docommand(string text)
{
	vector<string> info;
	stringsplit('|', text, info);
	
	if(info.at(0) == "clearinv")
	{
		player.invamount.clear();
		player.inventory.clear();
	}
	else if(info.at(0) == "clearvariables")
	{
		player.varname.clear();
		player.varvalue.clear();
	}
	else if(info.at(0) == "resetplayer")
		initstory();
	
	return;
}

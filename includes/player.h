#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

using namespace std;

struct storyinfo
{
	int length_n_info = 2;
	int length_o_info = 3;
	
	string look_neededinfo[2] = {"startpoint: ", "health: "};
	string look_optionalinfo[3] = {"playername: ", "maxinventory: ", "maxhealth: "};
	
	string neededinfo[2] = {"",""};
	string optionalinfo[3] = {"","",""};
	
	//vectors for inventory
	vector<string> inventory;
	vector<int> invamount;
	vector<string> includes;
	
	string storyline;
	
	vector<string> varname;
	vector<string> varvalue;
	
	bool playing = true;
	
	bool shouldbreak = false;
	
} extern player;

#endif

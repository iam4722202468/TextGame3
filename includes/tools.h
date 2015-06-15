#ifndef TOOLS_H
#define TOOLS_H

#include <string>
using namespace std;

string removewhitespace(string str);
void search(string haystack, string needle, vector<int>& foundat);
void stringsplit(char splitat, string line, vector<string>& newtext);
string fixtext(string line, bool include_input);
void printitems();
bool findstoryline(string storyline);

#endif

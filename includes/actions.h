#ifndef ACTION_H
#define ACTION_H

#include <string>
using namespace std;

void useitem(string itemname, int mode);
void removeitem(int amountremove);
void additem(string itemname, int mode);
string needitem(string line);
string dorand(string line);
string health(string text, int mode);
void setvar(string text);
void setinfo(string text);
void docommand(string text);

#endif

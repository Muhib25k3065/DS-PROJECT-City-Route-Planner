#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include <string>
using namespace std;

void printTitle(const string& title);

string readLine(const string& prompt);

int readInt(const string& prompt);

int readIntInRange(const string& prompt, int low, int high);

double readNonNegativeDouble(const string& prompt);

bool readYesNo(const string& prompt);

#endif

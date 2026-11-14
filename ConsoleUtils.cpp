#include "ConsoleUtils.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
using namespace std;

string trimLocal(const string& text) {
    int length = text.size();
    int start = 0;
    int end = length - 1;

    while (start <= end && (text[start] == ' ' || text[start] == '\t' ||
                            text[start] == '\r' || text[start] == '\n')) {
        start++;
    }
    while (end >= start && (text[end] == ' ' || text[end] == '\t' ||
                            text[end] == '\r' || text[end] == '\n')) {
        end--;
    }

    if (start > end) {
        return "";
    }
    return text.substr(start, end - start + 1);
}

string lowerCase(string text) {
    int length = text.size();
    for (int i = 0; i < length; i++) {
        text[i] = tolower((unsigned char)text[i]);
    }
    return text;
}

void printTitle(const string& title) {
    cout << "\n" << title << "\n";
    int length = title.size();
    for (int i = 0; i < length; i++) {
        cout << "=";
    }
    cout << "\n";
}

string readLine(const string& prompt) {
    cout << prompt << flush;
    string line;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    int length = line.size();
    if (length > 0 && line[length - 1] == '\r') {
        line.erase(length - 1);
    }
    return line;
}

int readInt(const string& prompt) {
    while (true) {
        string text = trimLocal(readLine(prompt));
        istringstream in(text);
        int value = 0;
        char extra;
        if (!text.empty() && (in >> value) && !(in >> extra)) {
            return value;
        }
        cout << "  Please enter a whole number.\n";
    }
}

int readIntInRange(const string& prompt, int low, int high) {
    while (true) {
        int value = readInt(prompt);
        if (value >= low && value <= high) {
            return value;
        }
        cout << "  Please enter a number from " << low << " to " << high << ".\n";
    }
}

double readNonNegativeDouble(const string& prompt) {
    while (true) {
        string text = trimLocal(readLine(prompt));
        istringstream in(text);
        double value = 0.0;
        char extra;
        if (!text.empty() && (in >> value) && !(in >> extra) && value >= 0.0) {
            return value;
        }
        cout << "  Please enter a number that is 0 or more.\n";
    }
}

bool readYesNo(const string& prompt) {
    while (true) {
        string text = lowerCase(trimLocal(readLine(prompt + " (y/n): ")));
        if (text == "y" || text == "yes") {
            return true;
        }
        if (text == "n" || text == "no") {
            return false;
        }
        cout << "  Please type y or n.\n";
    }
}

#include "Location.h"
#include <cctype>
using namespace std;

bool isSpaceChar(char c) {
    if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
        return true;
    }
    return false;
}

string trimText(const string& text) {
    int length = text.size();
    int start = 0;
    int end = length - 1;

    while (start <= end && isSpaceChar(text[start])) {
        start++;
    }
    while (end >= start && isSpaceChar(text[end])) {
        end--;
    }

    if (start > end) {
        return "";
    }
    return text.substr(start, end - start + 1);
}

bool isValidLocationName(const string& rawName, string& error) {
    string name = trimText(rawName);
    int length = name.size();

    if (length == 0) {
        error = "Location name cannot be empty.";
        return false;
    }
    if (length > MAX_LOCATION_NAME_LENGTH) {
        error = "Location name is too long (maximum 40 characters).";
        return false;
    }
    for (int i = 0; i < length; i++) {
        unsigned char c = name[i];
        if (c == '|') {
            error = "Location name cannot contain the '|' character.";
            return false;
        }
        if (c < 32) {
            error = "Location name cannot contain control characters.";
            return false;
        }
    }
    return true;
}

bool namesEqualIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) {
        return false;
    }
    int length = a.size();
    for (int i = 0; i < length; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
            return false;
        }
    }
    return true;
}

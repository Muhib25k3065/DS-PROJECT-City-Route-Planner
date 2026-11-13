#ifndef LOCATION_H
#define LOCATION_H

#include <string>
using namespace std;

const int MAX_LOCATION_NAME_LENGTH = 40;

struct Location {
    int id;
    string name;
    bool active;

    Location() {
        id = -1;
        active = false;
    }

    Location(int locationID, const string& locationName) {
        id = locationID;
        name = locationName;
        active = true;
    }
};

string trimText(const string& text);

bool isValidLocationName(const string& rawName, string& error);

bool namesEqualIgnoreCase(const string& a, const string& b);

#endif

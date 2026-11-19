#include "FileManager.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
using namespace std;

vector<string> splitBar(const string& line) {
    vector<string> fields;
    string current;
    int length = line.size();
    for (int i = 0; i < length; i++) {
        if (line[i] == '|') {
            fields.push_back(current);
            current.clear();
        } else {
            current += line[i];
        }
    }
    fields.push_back(current);
    return fields;
}

bool parseInt(const string& text, int& value) {
    istringstream in(text);
    char extra;
    if (!(in >> value)) {
        return false;
    }
    if (in >> extra) {
        return false;
    }
    return true;
}

bool parseDouble(const string& text, double& value) {
    istringstream in(text);
    char extra;
    if (!(in >> value)) {
        return false;
    }
    if (in >> extra) {
        return false;
    }
    return true;
}

bool parseFlag(const string& text, bool& value) {
    if (text == "0") {
        value = false;
        return true;
    }
    if (text == "1") {
        value = true;
        return true;
    }
    return false;
}

bool nextLine(ifstream& in, string& line, int& lineNumber) {
    while (getline(in, line)) {
        lineNumber++;
        int length = line.size();
        if (length > 0 && line[length - 1] == '\r') {
            line.erase(length - 1);
        }
        if (!trimText(line).empty()) {
            return true;
        }
    }
    return false;
}

bool parseCountLine(const string& line, const string& keyword, int& count) {
    istringstream in(line);
    string word;
    char extra;
    if (!(in >> word >> count)) {
        return false;
    }
    if (word != keyword) {
        return false;
    }
    if (in >> extra) {
        return false;
    }
    if (count < 0) {
        return false;
    }
    return true;
}

string lineLabel(int lineNumber) {
    ostringstream out;
    out << "Line " << lineNumber << ": ";
    return out.str();
}

bool saveMap(const Graph& graph, const string& filename, string& error) {
    ofstream out(filename.c_str());
    if (!out) {
        error = "Cannot open \"" + filename + "\" for writing.";
        return false;
    }
    out << setprecision(12);
    out << "CRP_MAP 1\n";
    out << "LOCATIONS " << graph.locationCount() << "\n";
    for (int id = 0; id < graph.capacity(); id++) {
        if (graph.isValidLocation(id)) {
            out << id << "|" << graph.nameOf(id) << "\n";
        }
    }

    vector<Road> roads;
    for (int id = 0; id < graph.capacity(); id++) {
        for (const RoadNode* node = graph.getOutgoing(id); node != NULL; node = node->next) {
            if (!node->isTwoWay || node->fromID < node->toID) {
                roads.push_back(nodeToRoad(*node));
            }
        }
    }

    int total = roads.size();
    for (int i = 1; i < total; i++) {
        Road current = roads[i];
        int j = i;
        while (j > 0 && roads[j - 1].roadID > current.roadID) {
            roads[j] = roads[j - 1];
            j--;
        }
        roads[j] = current;
    }

    out << "ROADS " << total << "\n";
    for (int i = 0; i < total; i++) {
        int openFlag = 0;
        if (roads[i].isOpen) {
            openFlag = 1;
        }
        int twoWayFlag = 0;
        if (roads[i].isTwoWay) {
            twoWayFlag = 1;
        }
        out << roads[i].roadID << "|" << roads[i].fromID << "|" << roads[i].toID << "|"
            << roads[i].distance << "|" << roads[i].travelTime << "|"
            << openFlag << "|" << twoWayFlag << "\n";
    }
    out.flush();
    if (!out) {
        error = "Writing to \"" + filename + "\" failed.";
        return false;
    }
    return true;
}

bool loadMap(Graph& graph, const string& filename, string& error) {
    ifstream in(filename.c_str());
    if (!in) {
        error = "Cannot open \"" + filename + "\" for reading.";
        return false;
    }

    Graph temp;
    string line;
    int lineNumber = 0;
    int count = 0;

    if (!nextLine(in, line, lineNumber) || trimText(line) != "CRP_MAP 1") {
        error = "Not a City Route Planner map file (missing \"CRP_MAP 1\" header).";
        return false;
    }

    if (!nextLine(in, line, lineNumber) || !parseCountLine(line, "LOCATIONS", count)) {
        error = lineLabel(lineNumber) + "expected \"LOCATIONS <count>\".";
        return false;
    }
    for (int i = 0; i < count; i++) {
        if (!nextLine(in, line, lineNumber)) {
            error = lineLabel(lineNumber) + "file ended before all locations were listed.";
            return false;
        }
        vector<string> fields = splitBar(line);
        int id = 0;
        if (fields.size() != 2 || !parseInt(trimText(fields[0]), id)) {
            error = lineLabel(lineNumber) + "expected \"<id>|<name>\".";
            return false;
        }
        if (!temp.addLocationWithId(id, fields[1])) {
            error = lineLabel(lineNumber) + temp.getLastError();
            return false;
        }
    }

    if (!nextLine(in, line, lineNumber) || !parseCountLine(line, "ROADS", count)) {
        error = lineLabel(lineNumber) + "expected \"ROADS <count>\".";
        return false;
    }
    for (int i = 0; i < count; i++) {
        if (!nextLine(in, line, lineNumber)) {
            error = lineLabel(lineNumber) + "file ended before all roads were listed.";
            return false;
        }
        vector<string> fields = splitBar(line);
        int roadID = 0;
        int from = 0;
        int to = 0;
        double distance = 0.0;
        double time = 0.0;
        bool open = true;
        bool twoWay = false;

        bool valid = true;
        if (fields.size() != 7) {
            valid = false;
        }
        if (valid && !parseInt(trimText(fields[0]), roadID)) {
            valid = false;
        }
        if (valid && !parseInt(trimText(fields[1]), from)) {
            valid = false;
        }
        if (valid && !parseInt(trimText(fields[2]), to)) {
            valid = false;
        }
        if (valid && !parseDouble(trimText(fields[3]), distance)) {
            valid = false;
        }
        if (valid && !parseDouble(trimText(fields[4]), time)) {
            valid = false;
        }
        if (valid && !parseFlag(trimText(fields[5]), open)) {
            valid = false;
        }
        if (valid && !parseFlag(trimText(fields[6]), twoWay)) {
            valid = false;
        }
        if (!valid) {
            error = lineLabel(lineNumber) + "expected \"<roadID>|<from>|<to>|<km>|<min>|<open 0/1>|<twoWay 0/1>\".";
            return false;
        }

        if (!temp.restoreRoad(Road(roadID, from, to, distance, time, open, twoWay))) {
            error = lineLabel(lineNumber) + temp.getLastError();
            return false;
        }
    }

    if (nextLine(in, line, lineNumber)) {
        error = lineLabel(lineNumber) + "unexpected extra content after the last road.";
        return false;
    }

    graph.swapWith(temp);
    return true;
}

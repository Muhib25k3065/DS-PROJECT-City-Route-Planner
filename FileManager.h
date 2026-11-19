#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "Graph.h"
using namespace std;

bool saveMap(const Graph& graph, const string& filename, string& error);

bool loadMap(Graph& graph, const string& filename, string& error);

#endif

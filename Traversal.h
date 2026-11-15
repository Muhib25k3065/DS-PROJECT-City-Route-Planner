#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#include <string>
#include <vector>
#include "Graph.h"
#include "RouteResult.h"
using namespace std;

RouteResult bfsFewestRoads(const Graph& graph, int startID, int endID);

bool bfsReachable(const Graph& graph, int startID, vector<int>& reachable, string& error);

#endif

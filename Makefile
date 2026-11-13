CXX      = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -g
LIB_SRC  = $(filter-out src/main.cpp,$(wildcard src/*.cpp))

app:
	$(CXX) $(CXXFLAGS) src/main.cpp $(LIB_SRC) -o city_route_planner.exe

test:
	$(CXX) $(CXXFLAGS) tests/$(T).cpp $(LIB_SRC) -o $(T).exe

.PHONY: app test

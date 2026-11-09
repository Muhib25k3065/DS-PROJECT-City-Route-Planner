# City Route Planner - build helper (works with g++ / MinGW)
#   Build the whole program :  make app
#   Build ONE test          :  make test T=test_minheap
# On Windows with Dev-C++ use  mingw32-make  instead of  make.
CXX      = g++
CXXFLAGS = -std=c++98 -Wall -Wextra -pedantic -g
LIB_SRC  = $(filter-out src/main.cpp,$(wildcard src/*.cpp))

app:
	$(CXX) $(CXXFLAGS) src/main.cpp $(LIB_SRC) -o city_route_planner.exe

test:
	$(CXX) $(CXXFLAGS) tests/$(T).cpp $(LIB_SRC) -o $(T).exe

.PHONY: app test

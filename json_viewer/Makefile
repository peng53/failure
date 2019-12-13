CXXFLAGS=-pedantic -O2 -march=native -Wall
OBJ=../objects
objects=$(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/app.o $(OBJ)/view.o $(OBJ)/viewer.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o ../bin/jso_viewer


$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/jso.o: ../json_adt/jso.cpp ../json_adt/jso.o
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/jso_f.o: ../json_adt/jso_f.cpp ../json_adt/jso_f.h 
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/viewer.o: main.cpp view.h app.h
	$(CXX) -c $(CXXFLAGS) $< -o $@


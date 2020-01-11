CXXFLAGS=-pedantic -O2 -march=native -Wall
OBJ=../objects
objects=$(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/app.o $(OBJ)/view.o $(OBJ)/viewer.o $(OBJ)/parser.o $(OBJ)/ireaderfactory.o $(OBJ)/stream_test.o $(OBJ)/chread.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o ../bin/jso_viewer -lncurses


$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/jso.o: ../json_adt/jso.cpp ../json_adt/jso.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/jso_f.o: ../json_adt/jso_f.cpp ../json_adt/jso_f.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/parser.o: ../json_adt/parser.o ../json_adt/parser.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/ireaderfactory.o: ../chunkreader/ireaderfactory.cpp ../chunkreader/ireaderfactory.h
	$(CXX) -c $(CXXFLAGS) $< -o $@
	
$(OBJ)/stream_test.o: ../chunkreader/stream_test.cpp ../chunkreader/stream_test.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chread.o: ../chunkreader/chread.cpp ../chunkreader/chread.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/viewer.o: main.cpp view.h app.h
	$(CXX) -c $(CXXFLAGS) $< -o $@


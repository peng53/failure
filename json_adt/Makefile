# Makefile for json_adt

CXXFLAGS=-pedantic -O2 -march=native -Wall -Werror
OBJ=../objects

build: $(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/json_adt_main.o $(OBJ)/stream_test.o $(OBJ)/parser.o $(OBJ)/bmarksg.o
	$(CXX) $(CXXFLAGS) $(OBJ)/jso_f.o $(OBJ)/jso.o $(OBJ)/stream_test.o $(OBJ)/parser.o $(OBJ)/json_adt_main.o $(OBJ)/bmarksg.o -o ../bin/jso_f -lsqlite3


debug: CXXFLAGS += -DDEBUG -g
debug: build

$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/stream_test.o: ../chunkreader/stream_test.cpp ../chunkreader/stream_test.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/bmarksg.o: ../bmarksg/bmarksg.cpp ../bmarksg/bmarksg.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/json_adt_main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm ../bin/jso_f $(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/json_adt_main.o $(OBJ)/chread.o $(OBJ)/parser.o

test: ../bin/jso_f
	../bin/jso_f ../in_out/$(in)

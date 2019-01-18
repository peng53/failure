# Makefile for json_adt

CXXFLAGS=-pedantic -O2 -march=native -Wall
OBJ=../objects
objects=$(OBJ)/jso.o $(OBJ)/jso_f.o $(OBJ)/json_adt_main.o $(OBJ)/stream_test.o $(OBJ)/chread.o $(OBJ)/parser.o $(OBJ)/bmarksg.o $(OBJ)/ireaderfactory.o

build: $(objects)
	$(CXX) $(CXXFLAGS) $^ -o ../bin/jso_f -lsqlite3

debug: CXXFLAGS += -DDEBUG -g
debug: build

$(OBJ)/%.o: %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/stream_test.o: ../chunkreader/stream_test.cpp ../chunkreader/stream_test.h ../chunkreader/ireader.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/chread.o: ../chunkreader/chread.cpp ../chunkreader/chread.h ../chunkreader/ireader.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/ireaderfactory.o: ../chunkreader/ireaderfactory.cpp ../chunkreader/ireaderfactory.h ../chunkreader/ireader.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/bmarksg.o: ../bmarksg/bmarksg.cpp ../bmarksg/bmarksg.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ)/json_adt_main.o: main.cpp ../chunkreader/ireaderfactory.h ../bmarksg/bmarksg.h ../chunkreader/ireader.h ../chunkreader/stream_test.h ../chunkreader/chread.h jso_f.h jso.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm ../bin/jso_f $(objects)

test: ../bin/jso_f
	../bin/jso_f ../in_out/$(in)

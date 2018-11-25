# Makefile for srstrgen_pm

CXXFLAGS=-pedantic -O2 -march=native -std=c++11
CSC=csc
OBJ=../objects
objects= main.o $(OBJ)/ste.o $(OBJ)/b_inc.o

srstrgen: $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o srstrgen

$(OBJ)/%.o : %.cpp %.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

test:
	./srstrgen -c 3 -b 001000 abc - BOB CAS EGD -s 0,4,1,2 " cool cat" " " 2,4,0
	./srstrgen -c 6 1 2 3 _ -s 0,2,0 3 0,2,0 3 0,2,0
clean:
	rm $(objects) srstrgen

parted.dll: parted.cs
	$(CSC) /t:library /out:parted.dll parted.cs

sr_csc.exe: parted.dll main.cs
	$(CSC) /reference:parted.dll /t:exe /out:sr_csc.exe main.cs

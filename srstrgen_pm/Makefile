CXXFLAGS=-pedantic -O2 -march=native
CXX=clang++
CSC=csc

srstrgen: main.o ste.o b_inc.o
	$(CXX) $(CXXFLAGS) main.o ste.o b_inc.o -o srstrgen
main.o: main.cpp ste.h b_inc.h
	$(CXX) -c $(CXXFLAGS) -std=c++11 main.cpp
ste.o: ste.cpp ste.h
	$(CXX) -c $(CXXFLAGS) -std=c++11 ste.cpp
b_inc.o: b_inc.cpp b_inc.h
	$(CXX) -c $(CXXFLAGS) b_inc.cpp

st.o: st.cpp st.h
	$(CXX) -c $(CXXFLAGS) -std=c++11 st.cpp -o ste.o

test:
	./srstrgen -c 3 -b 001000 abc - BOB CAS EGD -s 0,4,1,2 " cool cat" " " 2,4,0
	./srstrgen -c 6 1 2 3 _ -s 0,2,0 3 0,2,0 3 0,2,0
clean:
	rm -I *.o *.gch srstrgen
	rm -I sr_csc.exe
	rm -I parted.dll

parted.dll: parted.cs
	$(CSC) /t:library /out:parted.dll parted.cs

sr_csc.exe: parted.dll main.cs
	$(CSC) /reference:parted.dll /t:exe /out:sr_csc.exe main.cs

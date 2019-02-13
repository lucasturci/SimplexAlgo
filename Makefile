all:
	g++ -o simplex simplex.cpp matriz.cpp -std=c++11
clean:
	rm *.o simplex
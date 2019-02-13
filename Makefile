all:
	g++ -o simplex simplex.cpp matriz.cpp input_parser.cpp -std=c++11
clean:
	rm *.o simplex
mycal: calculate.cc error.cc funchandle.cc main.cc varhandle.cc
	g++ -c -o calculate.o calculate.cc
	g++ -c -o error.o error.cc
	g++ -c -o funchandle.o funchandle.cc
	g++ -c -o main.o main.cc
	g++ -c -o varhandle.o varhandle.cc
	g++ -o mycal calculate.o error.o funchandle.o main.o varhandle.o -g

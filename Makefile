recommender.exe : recommender.o
	g++ -std=c++11 -o recommender.exe recommender.o

recommender.o : recommender.cpp
	g++ -std=c++11 -c -o recommender.o recommender.cpp


all:
	g++ main.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system
lint:
	cpplint *.cpp *.hpp
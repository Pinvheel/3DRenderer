all:
	g++ main.cpp structs.cpp Engine3D.cpp math_utils.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system
lint:
	cpplint *.cpp *.hpp
debug:
	g++ ../../backward.cpp main.cpp -std=c++23 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	g++ main.cpp -std=c++23 -o out -O3 -g

retail:
	g++ main.cpp -std=c++23 -o outr -O3

all: release retail debug



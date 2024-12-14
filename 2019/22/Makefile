debug:
	clang++ ../../backward.cpp main.cpp -std=c++17 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	clang++ main.cpp -std=c++17 -o out -O3 -g

retail:
	clang++ main.cpp -std=c++17 -o outr -O3

all: release retail debug



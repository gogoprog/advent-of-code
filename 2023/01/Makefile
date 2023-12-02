debug:
	g++ ../../backward.cpp main.cpp -std=c++23 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	g++ main.cpp -std=c++23 -o out -O3 -g -static-libgcc -static-libstdc++

retail:
	g++ main.cpp -std=c++23 -o outr -O3 -static-libgcc -static-libstdc++

all: release retail debug



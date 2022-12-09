visual:
	g++ main.cpp -std=c++20 -lraylib -o outv -DRAYLIB

visual-retail:
	g++ main.cpp -std=c++20 -lraylib -o outv -DRAYLIB -O3

debug:
	g++ ../../backward.cpp main.cpp -std=c++20 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	g++ main.cpp -std=c++20 -o out -O3 -g

retail:
	g++ main.cpp -std=c++20 -o outr -O3

all: release retail debug



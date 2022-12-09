visual:
	# g++ main2.cpp -std=c++20 -lraylib -o outv -DRAYLIB
	g++ ../../backward.cpp main2.cpp -std=c++20 -lraylib -o outv -DRAYLIB -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

debug:
	g++ ../../backward.cpp main.cpp -std=c++17 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	g++ main.cpp -std=c++17 -o out -O3 -g

retail:
	g++ main.cpp -std=c++17 -o outr -O3

all: release retail debug visual



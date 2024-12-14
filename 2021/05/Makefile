debug:
	g++ ../../backward.cpp main.cpp -std=c++17 -o outd -g -lbfd -ldl -D BACKWARD_HAS_BFD=1

release:
	g++ main.cpp -std=c++17 -o out -O3 -g

retail:
	g++ main.cpp -std=c++17 -o outr -O3

haxe:
	haxe Main.hx -main Main -js main.js -lib coroutine


all: release retail debug



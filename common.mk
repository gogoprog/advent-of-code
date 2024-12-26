
OUTD = outd
OUT = out
OUTR = outr

SRCS = ../../backward.cpp main.cpp

CXX = g++
CXXFLAGS = -std=c++23
LDFLAGS =

debug: $(OUTD)

release: $(OUT)

retail: $(OUTR)

$(OUTD): $(SRCS)
	$(CXX) $(CXXFLAGS) -g -DBACKWARD_HAS_BFD=1 $(SRCS) -o $@ -lbfd -ldl $(LDFLAGS)

$(OUT): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -O3 -g -static -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)

$(OUTR): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -O3 -static -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)

clean:
	rm -f $(OUTD) $(OUT) $(OUTR)

run-debug:
	./outd

run-release:
	./out

run-retail:
	./outr


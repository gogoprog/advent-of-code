
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
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -std=c++23 -O3 -g -static -static-libgcc -static-libstdc++ -fno-exceptions

$(OUTR): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -std=c++23 -O3 -static -static-libgcc -static-libstdc++ -fno-exceptions

clean:
	rm -f $(OUTD) $(OUT) $(OUTR)

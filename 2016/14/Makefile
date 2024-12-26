include ../../common.mk

LDFLAGS = -lcrypto

$(OUT): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -std=c++23 -O3 -g -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)

$(OUTR): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -std=c++23 -O3 -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)


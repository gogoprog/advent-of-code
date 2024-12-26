include ../../common.mk

LDFLAGS := -lcrypto

$(OUT): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -O3 -g -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)

$(OUTR): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@ -O3 -static-libgcc -static-libstdc++ -fno-exceptions $(LDFLAGS)


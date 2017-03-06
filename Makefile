CXX=g++
CXXFLAGS=-Ilibdiscord/include -std=c++14 -c -fPIC -DELPP_DISABLE_TRACE_LOGS -DELPP_DISABLE_DEBUG_LOGS
LDLIBS=-lboost_system -lcrypto -lssl -lcpprest -lz -lpthread
SRCS=$(wildcard libdiscord/src/*.cpp) $(wildcard libdiscord/src/api/*.cpp) $(wildcard libdiscord/src/event/*.cpp) $(wildcard libdiscord/src/external/*.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))
LIB=bin/libdiscord.so

all: $(SRCS) $(LIB)

$(LIB): $(OBJS)
	$(CXX) $(OBJS) -shared -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< $(LDLIBS) -o $@

rebuild: clean all

clean:
	rm $(OBJS)
	rm $(LIB)


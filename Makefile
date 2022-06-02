CXXFLAGS = -g -Wall
CXX = g++

SRCS = poll.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all : $(OBJS)

clean :
	rm -f *.o

depend :
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak

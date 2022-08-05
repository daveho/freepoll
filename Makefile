CXXFLAGS = -g -Wall -I/usr/include/hidapi
CXX = g++

SRCS = poll.cpp main.cpp base.cpp exception.cpp message.cpp response_callback.cpp
OBJS = $(SRCS:%.cpp=%.o)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

freepoll : $(OBJS)
	$(CXX) -o $@ $(OBJS) -lhidapi-libusb -lpthread

clean :
	rm -f *.o freepoll

depend :
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak

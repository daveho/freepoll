CXXFLAGS = -g -Wall -I/usr/include/hidapi $(shell wx-config --cxxflags)
CXX = g++

COMMON_SRCS = poll.cpp base.cpp exception.cpp message.cpp \
	response_callback.cpp poll_response_collector.cpp poll_runner.cpp \
	observable.cpp observer.cpp timer.cpp
TUI_SRCS = main.cpp
GUI_SRCS = guimain.cpp poll_view.cpp timer_view.cpp \
	poll_response_count_view.cpp poll_model.cpp

ALL_SRCS = $(COMMON_SRCS) $(TUI_SRCS) $(GUI_SRCS)

COMMON_OBJS = $(COMMON_SRCS:%.cpp=%.o)
TUI_OBJS = $(TUI_SRCS:%.cpp=%.o)
GUI_OBJS = $(GUI_SRCS:%.cpp=%.o)

TUI_LIBS = -lhidapi-libusb -lpthread
GUI_LIBS = -lhidapi-libusb $(shell wx-config --libs)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

all : freepoll freepoll-gui

freepoll : $(TUI_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $(TUI_OBJS) $(COMMON_OBJS) $(TUI_LIBS)

freepoll-gui : $(GUI_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $(GUI_OBJS) $(COMMON_OBJS) $(GUI_LIBS)

clean :
	rm -f *.o freepoll freepoll-gui depend.mak

depend :
	$(CXX) $(CXXFLAGS) -M $(ALL_SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak

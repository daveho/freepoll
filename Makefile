# Makefile for FreePoll

# Determine whether we're compiling on MSYS2
IS_MINGW64 := $(findstring MINGW64,$(shell uname -s))

# On Windows/MSYS2, we use the "hidapi" package.
# On Linux, we use the "hidapi-libusb" package.
ifeq ($(IS_MINGW64),MINGW64)
HIDAPI_PKG = hidapi
EXTRA_DEFS = -DFREEPOLL_IS_WINDOWS
else
HIDAPI_PKG = hidapi-libusb
EXTRA_DEFS = -DFREEPOLL_IS_POSIX
endif

CXXFLAGS = -g -Wall -std=c++17 $(EXTRA_DEFS) \
	$(shell pkg-config $(HIDAPI_PKG) --cflags-only-I) \
	$(shell wx-config --cxxflags)
CXX = g++

COMMON_SRCS = poll.cpp base.cpp frequency.cpp exception.cpp message.cpp \
	response_callback.cpp poll_response_collector.cpp poll_runner.cpp \
	observable.cpp observer.cpp timer.cpp datastore.cpp course.cpp \
	screenshot.cpp
TUI_SRCS = main.cpp
GUI_SRCS = guimain.cpp poll_view.cpp timer_view.cpp \
	poll_response_count_view.cpp poll_model.cpp poll_frequency_view.cpp \
	bar_graph_view.cpp

ALL_SRCS = $(COMMON_SRCS) $(TUI_SRCS) $(GUI_SRCS)

COMMON_OBJS = $(COMMON_SRCS:%.cpp=%.o)
TUI_OBJS = $(TUI_SRCS:%.cpp=%.o)
GUI_OBJS = $(GUI_SRCS:%.cpp=%.o)

HIDAPI_LDFLAGS = $(shell pkg-config $(HIDAPI_PKG) --libs)
TUI_LIBS = $(HIDAPI_LDFLAGS) -lpthread
GUI_LIBS = $(HIDAPI_LDFLAGS) $(shell wx-config --libs)

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

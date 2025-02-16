# Makefile for fltk GUI

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

HIDAPI_CXXFLAGS = $(shell pkg-config $(HIDAPI_PKG) --cflags-only-I)
HIDAPI_LDFLAGS = $(shell pkg-config $(HIDAPI_PKG) --libs)

FLTK_CXXFLAGS = -std=gnu++17
FLTK_LDFLAGS = -lfltk_images -lfltk_forms -lfltk_gl -lfltk -lSM -lICE -lX11 -lXext -lm

CXX = g++
CXXFLAGS = -g -Wall -Iinclude $(FLTK_CXXFLAGS) $(HIDAPI_CXXFLAGS) $(EXTRA_DEFS)

COMMON_SRCS = poll.cpp base.cpp frequency.cpp exception.cpp message.cpp \
	poll_model.cpp \
	response_callback.cpp poll_response_collector.cpp poll_runner.cpp \
	observable.cpp observer.cpp timer.cpp datastore.cpp course.cpp \
	screenshot.cpp
GUI_SRCS = F_guimain.cpp F_freepoll_window.cpp F_async_notification.cpp \
	F_notification_queue.cpp F_bar_graph_view.cpp
TUI_SRCS = main.cpp
ALL_SRCS = $(COMMON_SRCS) $(GUI_SRCS) $(TUI_SRCS)

COMMON_OBJS = $(COMMON_SRCS:%.cpp=build/%.o)
GUI_OBJS = $(GUI_SRCS:%.cpp=build/%.o)
TUI_OBJS = $(TUI_SRCS:%.cpp=build/%.o)

build/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c src/$*.cpp -o build/$*.o

all : build/freepoll-gui-fltk build/freepoll

build/freepoll-gui-fltk : $(GUI_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $(GUI_OBJS) $(COMMON_OBJS) $(FLTK_LDFLAGS) $(HIDAPI_LDFLAGS)

build/freepoll : $(TUI_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $(TUI_OBJS) $(COMMON_OBJS) $(HIDAPI_LDFLAGS)

clean :
	rm -f build/*.o build/freepoll-gui-fltk build/freepoll

depend :
	$(CXX) $(CXXFLAGS) -M $(ALL_SRCS:%=src/%) \
		| scripts/fixdeps.rb \
		> depend.mak

depend.mak :
	touch $@

include depend.mak

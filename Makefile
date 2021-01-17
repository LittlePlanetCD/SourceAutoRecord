# Note that multiarch compilation details are highly system-dependent.

# The sfml dependency is resolved with pkg-config; you may have to set
# the PKG_CONFIG_PATH environment variable to something like
# /usr/lib/i386-linux-gnu/pkgconfig before running make.

.PHONY: all clean cvars get-deps

CXX=g++
SDIR=src
ODIR=obj

SRCS=$(wildcard $(SDIR)/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Demo/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Hud/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Routing/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Speedrun/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Speedrun/Rules/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Stats/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/ReplaySystem/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Tas/*.cpp)
SRCS+=$(wildcard $(SDIR)/Features/Timer/*.cpp)
SRCS+=$(wildcard $(SDIR)/Games/Linux/*.cpp)
SRCS+=$(wildcard $(SDIR)/Modules/*.cpp)
SRCS+=$(wildcard $(SDIR)/Utils/*.cpp)

OBJS=$(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))

# Header dependency target files; generated by g++ with -MMD
DEPS=$(OBJS:%.o=%.d)

# Ensure pkg-config can resolve dependencies
PKGCONFIG_DEPS=sfml-network
PKGCONFIG_CXXFLAGS!=pkg-config --cflags $(PKGCONFIG_DEPS)
PKGCONFIG_LDFLAGS!=pkg-config --libs $(PKGCONFIG_DEPS)
ifneq ($(.SHELLSTATUS),0)
$(error pkg-config could not find a package (do you need to set PKG_CONFIG_PATH?))
endif

WARNINGS=-Wall -Wno-unused-function -Wno-unused-variable -Wno-parentheses -Wno-unknown-pragmas -Wno-register
CXXFLAGS=-std=c++17 -m32 $(WARNINGS) -I$(SDIR) $(PKGCONFIG_CXXFLAGS)
LDFLAGS=-m32 -fPIC -shared -lstdc++fs $(PKGCONFIG_LDFLAGS)

all: sar.so
clean:
	rm -rf $(ODIR) sar.so

-include $(DEPS)

sar.so: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

cvars: doc/cvars.md
doc/cvars.md:
	node cvars.js "$(STEAM)Portal 2"

get-deps:
	sudo apt-get install -y g++-8-multilib libsfml-dev:i386

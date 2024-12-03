CXX = clang++
CXXFLAGS = -g -O3 `llvm-config --cxxflags --ldflags --system-libs --libs core`
# LDFLAGS = $(shell llvm-config --ldflags --system-libs --libs core)
TARGET = main
SRCS = main.cc
OBJS = $(SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
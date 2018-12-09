CXX := g++
CPPFLAGS := -O2 -Wall -I ./

LDLIBS := -lGL -lGLEW -lglfw -lIL -lassimp

COMMON := $(wildcard common/*.cpp)
COMMON_OBJ := $(patsubst %.cpp, %.o, $(COMMON))

run: all
	./a.out

all: main.o $(COMMON_OBJ)
	$(CXX) main.o $(COMMON_OBJ) $(LDLIBS)

%.o: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

.PHONY: clean dependencies help
clean:
	rm -f *.o *.out $(COMMON_OBJ)

help:
	@echo "$(COMMON_OBJ)"

dependencies:
	sudo apt-get -y install \
					libx11-dev \
					libxi-dev \
					libgl1-mesa-dev \
					libglu1-mesa-dev \
					libxrandr-dev \
					libxext-dev \
					libxcursor-dev \
					libxinerama-dev \
					libglew-dev\
					libglfw3-dev \
					libglfw3 \
					libglm-dev\
					libdevil-dev\
					libassimp-dev
					

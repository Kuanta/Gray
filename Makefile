CC = g++
VPATH = src/
OBJ_DIR := build/
INCLUDE_PATHS = -I include -I D:\Documents\Codes\Cpp\Libraries\Includes -I D:\Documents\Codes\Cpp\Libraries\Includes\glad -I ./include
LIB_PATHS = -L ../Libraries/lib/Assimp

LIBS = -lassimp.dll -lglfw3.dll -lgdi32 -lopengl32
SRC_CPP     :=                      \
   $(wildcard src/*.cpp)         \
   $(wildcard ../Gray_Make/*.cpp)         \

SRC_C := \
   $(wildcard src/*.c)         \
   $(wildcard ../Gray_Make/*.c)         \

BASENAMES_CPP := $(notdir $(SRC_CPP))
BASENAMES_C := $(notdir $(SRC_C))
OBJECTS  := $(BASENAMES_CPP:%.cpp=%.o)  #Don't wrap BASENAMES with $()
OBJECTS  += $(BASENAMES_C:%.c=%.o)  #Don't wrap BASENAMES with $()

OBJECT_PATHS = $(addprefix $(OBJ_DIR),$(OBJECTS))
CPP_FLAGS = $(INCLUDE_PATHS) -w -std=c++17

vpath %.cpp src
vpath %.h include
vpath %.o build

all: main

# Dep
deps := $(patsubst %.o, %.d, $(OBJECT_PATHS))
-include $(deps)
DEPFLAGS = -MMD -MF build/deps/$(@:.o=.d)

test:
	echo $(OBJECTS)
# Targets
main: $(OBJECTS)
	g++ $(OBJECT_PATHS) $(CPP_FLAGS) $(LIB_PATHS) $(LIBS) -o build/main.exe  2> log.txt

#Patterns
%.o: %.cpp
	$(CC) -c $< $(CPP_FLAGS) -o build/$@ $(DEPFLAGS)

%.o: %.c
	$(CC) -c $< $(CPP_FLAGS) -o build/$@ $(DEPFLAGS)


.PHONY: clean
clean: 
	rm -f build/*.o


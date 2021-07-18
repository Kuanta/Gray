CC = g++
VPATH = src/ src/Lights
OBJ_DIR := build/
INCLUDE_PATHS = -I ./include -I ./examples -I D:\Documents\Codes\Cpp\Libraries\Includes -I D:\Documents\Codes\Cpp\Libraries\Includes\glad 
INCLUDE_PATHS_UNIX = -I ../Libraries/include
LIB_PATHS =  
LIB_PATHS_UNIX = -L ../Libraries/lib/Assimp
LIBS_UNIX = -lassimp -lGL -lglfw3 -ldl -lX11 -lpthread -lXrandr -ldl
LIBS = -lassimp.dll -lglfw3.dll -lgdi32 -lopengl32
SRC_CPP     :=                      \
   $(wildcard src/*.cpp)         \
   $(wildcard examples/*.cpp) \
   $(wildcard src/Lights/*.cpp)         \

SRC_C := \
   $(wildcard src/*.c)         \

BASENAMES_CPP := $(notdir $(SRC_CPP))
BASENAMES_C := $(notdir $(SRC_C))
OBJECTS  := $(BASENAMES_CPP:%.cpp=%.o)  #Don't wrap BASENAMES with $()
OBJECTS  += $(BASENAMES_C:%.c=%.o)  #Don't wrap BASENAMES with $()

OBJECT_PATHS = $(addprefix $(OBJ_DIR),$(OBJECTS))
CPP_FLAGS = $(INCLUDE_PATHS) -w -std=c++17 -ggdb

vpath %.cpp src
vpath %.h include
vpath %.cpp examples
vpath %.o build

all: main
compileall:
	make clean
	make all

compileallwin:
	make clean
	make all

# Dep
deps := $(patsubst %.o, %.d, $(OBJECT_PATHS))
-include $(deps)
DEPFLAGS = -MMD -MF build/deps/$(@:.o=.d)

test:
	echo $(OBJECT_PATHS)
# Targets
main: $(OBJECTS)
	g++ $(OBJECT_PATHS) $(CPP_FLAGS) $(LIB_PATHS) $(LIBS) -o build/main.exe  2> log.txt

unix: $(OBJECTS)
	g++ $(OBJECT_PATHS) $(CPP_FLAGS) $(LIB_PATHS_UNIX) $(LIBS_UNIX) -o build/main  2> log.txt
#Patterns
%.o: %.cpp
	$(CC) -c $< $(CPP_FLAGS) -o build/$@ $(DEPFLAGS)

%.o: %.c
	$(CC) -c $< $(CPP_FLAGS) -o build/$@ $(DEPFLAGS)


.PHONY: clean, cleanwin
clean: 
	rm -f build\*.o

cleanwin:
	del /S build\*.o
	del /S build\deps\*.d

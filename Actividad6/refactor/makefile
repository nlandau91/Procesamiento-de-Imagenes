#OBJS specifies which files to compile as part of the project
OBJS = Actividad6.cpp Cartoonify.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w `pkg-config --cflags opencv4`

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = `pkg-config --libs opencv4`

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Actividad6

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

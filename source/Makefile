LIBNAME = libvisNET.a
FLAGS = -fPIC -Wall -D_BUILDLIBRARY
DEBUG = 0
BUILDMODE = Release
ifeq ($(DEBUG), 1)
	BUILDMODE = Debug
	FLAGS += -DDEBUG -g -O0
else
	FLAGS += -O3
endif

OBJ = ./obj
BIN = ./bin

$(OBJ)/$(BUILDMODE)/%.o: %.cpp createdir
	g++ $(FLAGS) -c $< -o $@

$(LIBNAME): $(patsubst %.cpp, $(OBJ)/$(BUILDMODE)/%.o, $(wildcard *.cpp))
	ar -r -s $(BIN)/$(BUILDMODE)/$(LIBNAME) $^

createdir:
	mkdir $(OBJ)/$(BUILDMODE) -p
	mkdir $(BIN)/$(BUILDMODE) -p

clean:
	rm -f $(OBJ)/$(BUILDMODE)/*.o $(BIN)/$(BUILDMODE)/$(LIBNAME)

rebuild: clean
	make


# PARAMS
EXE = tp
MAIN = main.cpp
FOLDERSRC = ./src
FOLDERHEAD = ./header
FOLDEREIGEN = ./extern/eigen-3.4.0
FOLDERPLY = ./extern/happly-2.0
INCLUDE_DIRS = -iquote $(FOLDERHEAD) -iquote $(FOLDEREIGEN) -iquote $(FOLDERPLY)

# VARS
CXX = g++
CXXFLAGS = -g -O3 -std=c++17 -pedantic -Wall
LIBS = -lglut -lGLU -lGL -lm -lpthread
SRC = $(wildcard $(FOLDERSRC)/*.cpp)
OBJ = $(SRC:.cpp=.o)

# PROG
all: $(OBJ)
	$(CXX) $(CXXFLAGS) $(MAIN) $(INCLUDE_DIRS) -o $(EXE) $(OBJ) $(LIBS)
	rm -f $(FOLDERSRC)/*.o
	@echo "$(EXE) prêt!"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@ $(LIBS)

# CLEAN
clean:
	rm -f $(OBJ) $(EXE)
	@echo "Cleaned up."
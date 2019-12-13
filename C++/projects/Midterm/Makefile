CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb

exclude_dirs = lib bin old data out
Dirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .
Dirs := $(basename $(patsubst ./%,%,$(Dirs)))
Dirs := $(filter-out $(exclude_dirs),$(Dirs))

BIN		:= bin
SRC 	:= $(foreach n,$(Dirs) , $(wildcard $(n)/*.cpp))
INCLUDE := $(foreach n,$(Dirs) , -I$(n)) -I/usr/include/python2.7
LIB		:= lib	

LIBRARIES	:= -lboost_date_time -lpython2.7
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) 

clean:
	-rm $(BIN)/*

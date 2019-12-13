CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

exclude_dirs = lib bin old
Dirs := $(shell ls -R | grep '^\./.*:$$' | awk '{gsub(":","");print}') .
Dirs := $(basename $(patsubst ./%,%,$(Dirs)))
Dirs := $(filter-out $(exclude_dirs),$(Dirs))

BIN		:= bin
SRC 	:= $(foreach n,$(Dirs) , $(wildcard $(n)/*.cpp))
# SRC		:= src/*.cpp src/test/*.cpp     # 指定所有源文件
# INCLUDE	:= -Iinclude -Iinclude/test #指定所有包含路径
INCLUDE := $(foreach n,$(Dirs) , -I$(n))
LIB		:= lib	

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*

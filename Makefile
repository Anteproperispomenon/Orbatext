CC  = gcc
CXX = g++
CXXFLAGS = -Wall

# Warning: I'm not great at Makefiles.
# SRCS = src/main.cpp src/text-box.cpp src/text-entry.cpp src/optimise.cpp src/reader.cpp src/writer.cpp
SRCC = main.cpp text-box.cpp text-entry.cpp optimise.cpp reader.cpp writer.cpp
SRCS = $(addprefix src/,$(SRCC))
OBJS = $(SRCS:.cpp=.o)
# OBJS = $(addprefix bin/,$(OBJC))

# Used OS detection from https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS),Windows_NT)
EXENAME = orbatext.exe
else
EXENAME = orbatext
endif

$(EXENAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXENAME) $(OBJS)

main.o : text-box.o text-entry.o optimise.o reader.o writer.o

text-box.o : text-box.cpp

text-entry.o : text-entry.cpp text-box.cpp 

optimise.o : text-box.cpp text-entry.cpp optimise.cpp

reader.o : reader.cpp

writer.o : writer.cpp

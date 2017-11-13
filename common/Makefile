DEFINE	:= 
CFLAGS	:= -Wall -std=gnu++11 -g

INCLUDES    := .

include ../rules/compile.mk

all: $(patsubst %.cpp, %.o, $(wildcard *.cpp))

include $(wildcard *.d) 


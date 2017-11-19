DEFINE	:= 
CFLAGS	:= -Wall -std=gnu++11 -g
LIBS 	:= 
L_LIBS	:= -lstdc++
LFLAGS	:= -pipe -pthread

INCLUDES := . ../common
OBJECTS  := ../common/main.o \
            ../common/pseudo.o
            
OBJECTS  += $(patsubst %.cpp, %.o, $(wildcard *.cpp))

APP_NAME := $(shell basename $(CURDIR))
RULES := $(wildcard ../rules/*.mk)


all: $(APP_NAME) move_to_bin

include $(RULES)
include $(wildcard *.d) 


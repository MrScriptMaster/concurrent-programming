
LESSONS := lesson_1
MAKE := 

all: make_main $(LESSONS)

make_main:
	make --directory=common

lesson_1:
	make --directory=les_1



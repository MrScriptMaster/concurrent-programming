
LESSONS := lesson_1 \
	   lesson_2 \
	   lesson_3
MAKE := 

all: make_main $(LESSONS)

make_main:
	make --directory=common

lesson_1:
	make --directory=les_1

lesson_2:
	make --directory=les_2

lesson_3:
	make --directory=les_3
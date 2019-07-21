
CC = gcc
LIBS = -lm
SUBDIRS = lua

target = libhll.so
test_target = test.out

all : $(target) test subdirs

$(target) : hyperloglog.c hyperloglog.h
	$(CC) -fPIC -shared $(LIBS) $^ -o $@

test : hyperloglog_test.c hyperloglog.h
	$(CC) -L./ -lhll $^ -o $(test_target)

.PHONY : subdirs
subdirs:
	for dir in $(SUBDIRS); do	\
		$(MAKE) -C $$dir;		\
	done

.PHONY : clean
clean : 
	rm *.so *.o *.out -rf
	for dir in $(SUBDIRS); do	\
		$(MAKE) clean -C $$dir;		\
	done
	

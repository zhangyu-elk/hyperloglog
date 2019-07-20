CC = gcc
LIBS = -lm

target = libhll.so
lua_target = ./lua/
test_target = test.out

all : $(target) test $(lua_target)

$(target) : hyperloglog.c hyperloglog.h
	$(CC) -fPIC -shared $(LIBS) $^ -o $@

$(lua_target) : 
	$(MAKE) -C $@

	
test : hyperloglog_test.c hyperloglog.h
	$(CC) -L./ -lhll $^ -o $(test_target)

.PHONY : clean
clean : 
	rm *.so *.o *.out -rf
	

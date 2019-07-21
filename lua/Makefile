
export LD_LIBRARY_PATH=/home/zhangyu/code/hyperloglog/:$LD_LIBRARY_PATH

target = hll.so
CC = gcc
LIBS = -llua-5.1 -lhll

LIB_PATH = -L../
INCLUDE_PATH = -I../


$(target) : lua_hll.c ../hyperloglog.h ../hyperloglog.c
	$(CC) -fPIC -shared $(LIBS) $(LIB_PATH) $(INCLUDE_PATH) lua_hll.c -o $@
	
.PHONY : clean
clean :
	rm -rf *.so *.o $(target)
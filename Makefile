MCBSPLIB=../lib/MulticoreBSP-for-C/lib/libmcbsp1.2.0.a
MCBSPINCLUDE=../lib/MulticoreBSP-for-C
LUAINCLUDE=../lib/lua-5.2.3/src 
LUALIB=../lib/lua-5.2.3/src


CC=gcc -fdiagnostics-color=auto
CFLAGS= -g -I $(MCBSPINCLUDE) -I $(LUAINCLUDE)
LFLAGS=$(MCBSPLIB) -L $(LUALIB) -llua -ldl -pthread -lm -lrt -lhwloc
OBJ= mbspengine.o mbspdiscover.o mbsputil.o
OBJ2= test_with_tree.o mbspdiscover.o mbsputil.o


all: mbspengine

tree: $(OBJ3) 
	$(CC) $(OBJ2) $(LFLAGS) -o tree.exe

test_with_tree.o: test_with_tree.c 
	$(CC) $(CFLAGS) -c test_with_tree.c





mbspengine: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o embsp.exe


mbspengine.o: mbspengine.c 
	$(CC) $(CFLAGS) -c mbspengine.c

mbspdiscover.o: mbspdiscover.c mbspdiscover.h
	$(CC) $(CFLAGS) -c mbspdiscover.c

mbsputil.o: mbsputil.c  mbsputil.h
	$(CC) $(CFLAGS) -c mbsputil.c

clean:
	rm -f *.o embsp	

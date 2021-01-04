# from "Makefile": randmain.mk contains instructions for building
# randmain, randlibhw.so, and randlibsw.so

randmain: randlibhw.so randlibsw.so randmain.c randcpuid.c
	$(CC) $(CFLAGS) -o randmain -ldl -Wl,-rpath=$(PWD) \
	randmain.c randcpuid.c

randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -shared -o $@ $^

randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared -o $@ $^

randlibhw.o: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

randlibsw.o: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

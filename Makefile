OPT=-Os -Wall
CFLAGS=-fno-ident -fno-stack-check -fno-stack-protector
LDFLAGS=-L. -nostartfiles -nostdlib -lucrt -luser32 -lkernel32 -Wl,--exclude-all-symbols,--enable-stdcall-fixup,--dynamicbase,--nxcompat,--subsystem,windows:6.0

PREFIX32=C:/mingw32/bin/
PREFIX64=C:/mingw64/bin/

CC32=gcc -Wl,-e_dll_main -municode
CC64=gcc -Wl,-edll_main -municode

all: sppc32.dll sppc64.dll

sppc32.dll: sppc.c sppc32.res.o libsppcs32.a
	$(PREFIX32)$(CC32) $(OPT) $< sppc.def sppc32.res.o -shared -o $@ $(CFLAGS) $(LDFLAGS) -lsppcs32
	strip -s $@

sppc64.dll: sppc.c sppc64.res.o libsppcs64.a
	$(PREFIX64)$(CC64) $(OPT) $< sppc.def sppc64.res.o -shared -o $@ $(CFLAGS) $(LDFLAGS) -lsppcs64
	strip -s $@

sppc32.res.o: sppc.rc
	$(PREFIX32)windres --codepage=65001 $< $@

sppc64.res.o: sppc.rc
	$(PREFIX64)windres --codepage=65001 $< $@

libsppcs32.a:
	$(PREFIX32)dlltool -k -dsppcs32.def -llibsppcs32.a

libsppcs64.a:
	$(PREFIX64)dlltool -k -dsppcs64.def -llibsppcs64.a

clean:
	rm -f *.dll *.res.o *.a

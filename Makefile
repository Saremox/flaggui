FlagGui : FlagGui.o
	cc -O2 `gtk-config --cflags --libs` FlagGui.o -o FlagGui
FlagGui.o : FlagGui.c
	cc -c `gtk-config --cflags --libs`  FlagGui.c -o FlagGui.o
clean :
	rm FlagGui.o FlagGui

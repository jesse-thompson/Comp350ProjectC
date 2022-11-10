bcc -c -ansi -o shellc.o shell.c
as86 userlib.asm -o userlibasm.o
ld86 -d -o shell shellc.o userlibasm.o

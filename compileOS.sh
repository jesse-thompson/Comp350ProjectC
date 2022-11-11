dd if=/dev/zero of=diskc.img bs=512 count=256
nasm bootload.asm
dd if=bootload of=diskc.img conv=notrunc

bcc -c -ansi -o kernelc.o kernel.c
as86 kernel.asm -o kernelasm.o
ld86 -d -o kernel kernelc.o kernelasm.o

bcc -ansi -c -o tstpr1.o tstpr1.c
as86 -o userlib.o userlib.asm
ld86 -d -o tstpr1 tstpr1.o userlib.o

gcc -o loadFile loadFile.c

./loadFile kernel
./loadFile message.txt
./loadFile tstpr1


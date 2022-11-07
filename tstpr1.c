main()
{
    syscall(0,"tstpr1 is working!\r\n");
    while(1);
}


tstpr2.c

main()
{
    syscall(0,"tstpr2 is working!\r\n");
    syscall(5);
    while(1);
}

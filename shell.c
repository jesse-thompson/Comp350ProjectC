//Jesse Thompson

void takeCommand(char* command);

void main()
{


    while (1)
    {
        char line[80];
        
        syscall(0, "SHELL> ");
        syscall(1, line);

        //Choose command to run

        syscall(0, "\r\n");
    }
    //Use values from the switch statement for syscall


    //syscall(switch statement case, buffer, 30)
}


void takeCommand(int command)
{
    switch (command)
    {
        case 0:
            syscall(0, "type\r\n");
        default:
            syscall(0, "No such command\r\n");
            break;
    }
}

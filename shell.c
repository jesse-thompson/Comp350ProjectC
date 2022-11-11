//Jesse Thompson
// Craig Kimball

void takeCommand(char* command);

void main()
{
    while (1)
    {
        char commandInput[80];

        syscall(0, "SHELL> ",0,0);
        syscall(1, commandInput,0,0);
        syscall(0, commandInput, 0, 0);

        //Choose command to run

        syscall(0, "\r\n");
    }
}
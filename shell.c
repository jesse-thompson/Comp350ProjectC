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

        // checking if the user wants to use the type command
        // this if statement is a little scuffed, but it'll work
        if (commandInput[0] == 't' && commandInput[1] == 'y' && commandInput[2] == 'p' && commandInput[3] == 'e')
        {
            syscall(0, "You want to type out a file");
        }
        else
        {
            syscall(0, "Error: not a valid command.");
        }

        //Choose command to run

        syscall(0, "\r\n");
    }
}
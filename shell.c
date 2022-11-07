//Jesse Thompson

void printString(char*);

main()
{
    switch (expression) {
        case expression:

        default:
            printString("Bad Command!")
    }
}

// Prints out the provided string to the console using interrupts
void printString(char* chars)
{
    int index = 0;
    while(chars[index] != 0x0)
    {
        interrupt(0x10, 0xe * 256 + chars[index], 0, 0, 0);
        index++;
    }
}

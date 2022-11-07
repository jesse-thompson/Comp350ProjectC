//Jesse Thompson

void readString(char*);
void printString(char*);
void handleInput(int, int, int, int);

main()
{
    char userInput[80];
    char fileInput[512];

    makeInterrupt21();
    interrupt(0x21,1,userInput,0,0);
    interrupt(0x21,0,userInput,0,0);
    interrupt(0x21,2,fileInput,30,0);
    interrupt(0x21,0,fileInput,0,0);
}

void handleInput(int ax, int bx, int cx, int dx)
{
    interrupt(0x21, 3, buffer, 30);

    switch (expression)
    {
        case expression:


        default:
            printString("Bad Command!")
    }
}


void readString(char* letter)
{
    int index = 0;
    //getting letter and printing to screen
    letter[index] = interrupt(0x16, 0, 0, 0,0);
    printChar(letter[index]);

    //check if enter key is pressed and under string limit but not zero
    while (letter[index] != 0xd  && 0 < index < 80)
    {
        char inputLetter = interrupt(0x16, 0, 0, 0, 0);
        //if input is backspace
        if (inputLetter == 0x8)
        {
            if(index >= 0)
            {
                index--;
                printChar(0x8);
                printChar(' ');
                printChar(0x8);
            }
        }
        else{
            index++;
            letter[index] = inputLetter;
            printChar(letter[index]);
        }
    }
    //once enter key pressed
    letter[index+1] = 0xa;
    letter[index+2] = 0x0;
    printChar(0xa);
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

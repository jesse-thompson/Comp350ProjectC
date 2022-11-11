// Craig Kimball 11/2/22
// TJ
// Jesse
// Sean


void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
void executeProgram(char* name);
void handleInterrupt21(int,int,int,int);

void main()
{
    // Setting up for readString
    char line[80];

    // Setting up readSector
    char sectorBuffer[512];

    // Setting up readFile
    char fileBuffer [13312];
    char* fileName = "messag";
    int sectorsRead;
    readFile(fileName, fileBuffer, &sectorsRead); // Just a test of the function
    printString("Attempting to print fileBuffer: \n\r");
    printString(fileBuffer);

    // Creating interrupt21
    makeInterrupt21();

    //readFile(fileName, fileBuffer, &sectorsRead); // Just a test of the function

    //handleInterrupt21(3, "messag", fileBuffer, &sectorsRead);

//    if (sectorsRead > 0)
//    {
//        printString(fileBuffer);
//    }
//    else
//    {
//        printString("Error: File not found\n\r");
//    }

    handleInterrupt21(4, "testpr1", 0, 0);



    while(1);
}

void printChar(char c)
{
    interrupt(0x10, 0xe*256+c,0,0,0);
}

void printString(char* chars)
{
    int increment = 0;
    while(chars[increment] != 0x0)
    {
        interrupt(0x10, 0xe*256+chars[increment],0,0,0);
        increment++;
    }
}

void readString(char* chars)
{
    int currIndex = 0;
    chars[currIndex] = interrupt(0x16, 0,0,0,0);
    printChar(chars[currIndex]);

    while(chars[currIndex] != 0xd && currIndex < 80)
    {
        char input = interrupt(0x16, 0,0,0,0);
        if(input == 0x8)
        {
            if(currIndex >= 0)
            {
                currIndex--;
                printChar(0x8);
                printChar(' ');
                printChar(0x8);
            }
        }
        else
        {
            currIndex ++;
            chars[currIndex] = input;
            printChar(input);
        }
    }
    printChar(0xa);

    chars[currIndex+1] = 0xa;
    chars[currIndex+2] = 0x0;
}

void readSector(char* buffer, int sector)
{
    interrupt(0x13, 2*256+1, buffer, sector+1, 0x80);
}

// Class notes 11/3/22
// TO find a file follow these steps:
// 1. Read the directory using readSector(place to put it, what sector you want to read);
// 2. Step through directory, one line at a time, and compare the lines to the file name

void readFile(char* fileName, char* buffer, int* sectorsRead)
{
    //int printIndex; // Index used for printing out the characters of the directory
    int correctCharIndex; // Index used for comparing how many characters in fileName match with directory[fileEntry]
    int correctChars; // The number of matching characters when comparing fileName and directory[fileEntry]

    int sectorIndex; // Index used for reading what sectors a file is stored on

    // The following variables are used for padding out characters in fileName
    int i;
    int pad;

    int fileEntry; // fileEntry acts as an index for accessing the data inside the directory
    char directory[512];

    readSector(directory,2); // Directory is at sector 2

    // Padding out fileName with 0's
    pad = 0; // Pad is false
    for (i = 0; i < 6; ++i)
    {
        if (fileName[i] == '\r' || fileName[i] == '\n')
        {
            pad = 1;
        }
        if (pad == 1)
            fileName[i] = '\0';
    }

    // printing out the contents of directory, just to see what's currently in there
//    printString("Printing directory: \n\r");
//    for (printIndex = 0; printIndex < 512; printIndex++)
//    {
//        printChar(directory[printIndex]);
//    }
//    printString("\n\r");


    for (fileEntry = 0; fileEntry < 512; fileEntry += 32)
    {
        correctChars = 0;

        // fileName has to match identically with the first 6 entries of file stored in the directory
        for (correctCharIndex = 0; correctCharIndex < 6; correctCharIndex++)
        {
            if (fileName[correctCharIndex] == directory[fileEntry + correctCharIndex])
            {
                correctChars++;
            }

            // Seeing if all 6 chars in fileName match with what is in the directory
            if (correctChars == 6)
            {
                // Looks messy but this is a better alternative for testing purposes.
                // This way we can see this print out when testing the shell
                printChar('F');
                printChar('i');
                printChar('l');
                printChar('e');
                printChar(' ');
                printChar('f');
                printChar('o');
                printChar('u');
                printChar('n');
                printChar('d');
                printChar('\r');
                printChar('\n');



                // Now that we've found the file, we need to find what sectors the file is on
                // Starting the index at 6 since the sectors that the file are stored on also start at index 6
                for (sectorIndex = 6; sectorIndex < 32; sectorIndex++)
                {
                    if (directory[fileEntry + sectorIndex] == 0x0)
                    {
                        // This was a test for readFile in kernel.c but this won't be helpful for shell
                        printString("All sectors found. \n\r");
                        break;
                    }
                    else
                    {
                        readSector(buffer, directory[fileEntry + sectorIndex]);
                        buffer += 512;
                        *sectorsRead = *sectorsRead + 1;
                    }
                }
            }
        }
    }
}

void executeProgram(char* name)
{
    int index = 0;
    int numSectorsRead;
    char buffer[13312];

    printChar('e');
    printChar('x');
    printChar('e');
    printChar('c');
    printChar('\r');
    printChar('\n');

    readFile(name, buffer, &numSectorsRead);

    for (index = 0; index < 13312; index++)
    {
        putInMemory(0x2000, index, buffer[index]);
    }
    launchProgram(0x2000);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax)
    {
        case 0:
            printString(bx);
            break;
        case 1:
            readString(bx);
            break;
        case 2:
            readSector(bx, cx);
            break;
        case 3:
            readFile(bx, cx, dx);
        case 4:
            executeProgram(bx);
        default:
            printString("No interrupt function correlated with AX number");

    }
}

// Craig Kimball 11/2/22
// TJ
// Jesse
// Sean


void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, char*, int*);
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
    makeInterrupt21(); // My project B did not give me this error for makeInterrupt21()

    //Project B
    // Calling readString
    //interrupt(0x21, 0, "Enter in some text: \0", 0, 0);
    // Calling printString
    //interrupt(0x21, 1, line, 0, 0);
    // Calling readSector
    //interrupt(0x21, 2, sectorBuffer, 30, 0);

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

void readFile(char* fileName, char* buffer, int sectorsRead)
{
    int i;
    int numSectors; // This is the incorrect way to see the number of sectors read, this is temporary

    int fileEntry;
    char directory[512];

    readSector(directory,2); // Directory is at sector 2
    numSectors = 0;

    // printing out the contents of directory, just to see what's currently in there
    printString("Printing directory: \n\r\0");
    for (i = 0; i < 512; i++)
    {
        printChar(directory[i]);
    }
    printString("\n\r\0");

    for (fileEntry = 0; fileEntry < 512; fileEntry += 32)
    {

        // fileName has to match identically with the first 6 entries of file stored in the directory
        // This approach will not work for anything less than 6 characters
        // Better approach is to replace this if statement with another for loop <-- I'll deal with this later
        if (fileName[fileEntry] == directory[fileEntry] && fileName[fileEntry + 1] == directory[fileEntry + 1] && fileName[fileEntry + 2] == directory[fileEntry + 2] &&
        fileName[fileEntry + 3] == directory[fileEntry + 3] && fileName[fileEntry + 4] == directory[fileEntry + 4] && fileName[fileEntry + 5] == directory[fileEntry + 5])
        {

            printString("File found.\n\r\0");

            for (i = 6; i < 28; i++)
            // 28 is the maximum amount of sectors a file can take up
            {
                if (&directory[fileEntry] == 0)
                {
                    break;
                }
                else
                {
                    readSector(buffer, directory[fileEntry]);
                    buffer += 512;
                    numSectors++;
//                    *sectorsRead = *sectorsRead + 1;
                }
            }
        }
    }

    // if no match, *sectorsRead = 0;
    // The above line means that we loops through the directory and found no matching file name

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
        default:
            printString("No interrupt function correlated with AX number");

    }
}

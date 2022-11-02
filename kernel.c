// Craig Kimball 11/2/22
// TJ
// Jesse
// Sean


void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*, int, int);
void handleInterrupt21(int,int,int,int);

void main()
{


    // Setting up for readString
    char line[80];

    // Setting up readSector
    char sectorBuffer[512];

    // Setting up readFile
    char fileBuffer [13312];
    int sectorsRead;

    // Creating interrupt21
    makeInterrupt21();

    //Project B
    // Calling readString
    interrupt(0x21, 0, "Enter in some text: \0", 0, 0);
    // Calling printString
    interrupt(0x21, 1, line, 0, 0);
    // Calling readSector
    interrupt(0x21, 2, sectorBuffer, 30, 0);

    //Project C
    // Calling readFile
    //interrupt(0x21, 3, "messag", fileBuffer, &sectorsRead);

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

void readFile(char* fileName, int buffer, int sectorsRead)
{
    int fileEntry = 0;
    int fileBuffer = 0;
    int amountOfSectors = 0;
    char directory[512];

    directory = readSector("",2); // Directory is at sector 2, but readSector needs a value for buffer

    for (; fileEntry < 512; iter += 32)
    {
        // fileName has to match identically with the first 6 entries of file stored in the directory
        if (fileName[fileEntry] == directory[fileEntry] and fileName[fileEntry + 1] == directory[fileEntry + 1] and fileName[fileEntry + 2] == directory[fileEntry + 2] and
        fileName[fileEntry + 3] == directory[fileEntry + 3] and fileName[fileEntry + 4] == directory[fileEntry + 4] and fileName[fileEntry + 5] == directory[fileEntry + 5])
        {
            for (int i = 0; i < 28; i++)
            // 28 is the maximum amount of sectors a file can take up
            {
                if (readSector("", fileEntry + 6) == 0)
                {
                    break; // I actually need to return a few things here, but don't know how
                }
                else
                {
                    amountOfSectors++;
                    fileBuffer += 512;
                }
            }
        }
    }



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

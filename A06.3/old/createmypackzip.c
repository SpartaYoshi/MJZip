/**
* @file createmypackzip.c
* @author   ISO-2-02
* @date     20/02/2022
* @brief    Creates a file of type mypackzip containing only data files.
* @details  createmypackzip must create the file fichro_destino with
*	    the mypackzip format with the data content of fich_origne.
*/

//Exits:
	//(1): Not enough arguments; Use: ./createmypackzip fich_origen fich_destino
	//(2): Unable to open the original file.

#include <stdio.h> //Input/output library functions. DO NOT USE fopen, fclose, fwrite, fread!
#include <unistd.h> //Unix standard symbolic constants and types.
#include <sys/stat.h> //Data types returned by the stat() function (for accessing file metadata).
#include <string.h> //For working with strings instead of char[].
#include <fcntl.h> //File control options

#include "s_header.h" //For including the s_header struct type definition.

struct s_mypack_headers headers; //Header container struct, there are 32.
char userMessage[128]; //For printing messages for the user.
char originalFileN[MAX_FILE_NAME]; //Name of the original file name.
char zipFileN[MAX_FILE_NAME]; //Name of the zip file to create.
unsigned long fileSize; //For storing file sizes in bytes.
int oFd; //File descriptor for the original file.
int zipFd; //File descriptor for output (created zip) file.
int i; //Indexes for loops...
char buf[BLOCK_SIZE]; //This program's buffer.

int main(int argc, char *argv[])
{
	if(argc < 3)
        {
                extrae_fichero(argv[1], 0);
		_exit(0);
        }

	//Check the number of arguments. Use: ./createmypackzip fich_origen fich_destino
	if(argc < 3)
	{
		sprintf(userMessage, "Uso: %s fich_origen fich_destino\n", argv[0]);
		write(1, userMessage, strlen(userMessage));
		_exit(1);
	}

	//Check original file opening:
	strcpy(originalFileN, argv[1]);
	oFd = open(originalFileN, O_RDONLY);
	if(oFd < 0)
	{
		sprintf(userMessage, "Error al intentar abrir el fichero \"%s\".\n", originalFileN);
                write(1, userMessage, strlen(userMessage));
                _exit(2);
	}

	//Create/Open the output zip file:
	strcpy(zipFileN, argv[2]);
	zipFd = open(zipFileN, O_CREAT | O_RDWR, S_IRWXU);

	//Introduce first file header information to the first header:
	headers.vHead[0].FileInfo.Type = 'b';
	headers.vHead[0].FileInfo.Compress = 'n';

	fileSize = sizeOfFile(originalFileN);

	headers.vHead[0].FileInfo.DataSize = fileSize;
	headers.vHead[0].FileInfo.CompSize = fileSize;

	headers.vHead[0].FileInfo.NumBlocks = amountOfBlocks(BLOCK_SIZE, fileSize);
	strcpy(headers.vHead[0].FileInfo.DataFileName, originalFileN);
	headers.vHead[0].FileInfo.DatPosition = sizeof(headers);

	//Initialize the rest of the empty file headers:
	for (i = 1; i < NUM_HEADERS; i++)
	{
		headers.vHead[i].FileInfo.Type = '\\';
        	headers.vHead[i].FileInfo.Compress = 'n';

        	fileSize = 0;

        	headers.vHead[i].FileInfo.DataSize = fileSize;
        	headers.vHead[i].FileInfo.CompSize = fileSize;

        	headers.vHead[i].FileInfo.NumBlocks = 0;
		strcpy(headers.vHead[i].FileInfo.DataFileName, "");
	        headers.vHead[i].FileInfo.DatPosition = 0;
	}

	//Write the headers into the zip file:
	write(zipFd, &headers, sizeof(headers));

	//Write the original file into the zip file by blocks:
	int nBlocks = headers.vHead[0].FileInfo.NumBlocks; //Number of blocks of header 0:

	int n; //Read amount of data.
	while((n = read(oFd, buf, BLOCK_SIZE)) > 0)
		write(zipFd, buf, n);

		//Fill with 0s the remaining space of the block:
		int remainingBytes = BLOCK_SIZE * nBlocks - headers.vHead[0].FileInfo.DataSize - 1;
		off_t currentPos = lseek(zipFd, 0, SEEK_CUR);//For knowing the current pointer position in the file.

		bzero(&currentPos, remainingBytes);

	//Closing files:
	close(oFd);
	close(zipFd);

	_exit(0);
}

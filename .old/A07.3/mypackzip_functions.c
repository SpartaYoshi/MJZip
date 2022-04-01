/*
* @file mypackzip_functions.c
* @author   ISO-2-02
* @date     20/02/2022
* @brief    Collection of functions for using in mypackzip project.
*/

#include <stdio.h> //Input/output library functions. DO NOT USE fopen, fclose, fwrite, fread!
#include <unistd.h> //Unix standard symbolic constants and types.
#include <sys/stat.h> //Data types returned by the stat() function (for accessing file metadata).
#include <string.h> //For working with strings instead of char[].
#include <fcntl.h> //File control options
#include <sys/types.h> //Unix file types.
#include <sys/stat.h> //Unix "struct stat" data structure.
#include <dirent.h> //Unix "struct dirent" data structure.
#include <stdlib.h> //For malloc() function.

#include "s_header.h" //For including the s_header struct type definition.

//Functions:
off_t sizeOfFile(char *nameOfFile);
unsigned long amountOfBlocks(unsigned long blockSize, unsigned long fileSize);
int extrae_fichero(char * f_mypackzip, int index);
int inserta_fichero(char * f_mypackzip, char * f_dat, int index);
int es_fichero_regular(char *fileName);
int es_directorio(char *fileName);
int es_enlace_simbolico(char *fileName);
int inserta_fichero_regular(int zipFD, int datFD, char * f_dat, struct s_mypack_headers *headers, int index);
int inserta_directorio(int zipFD, char *f_dat, struct s_mypack_headers *headers, int index);
int inserta_enlace_simbolico(int zipFD, char *f_dat, struct s_mypack_headers *headers, int index);
int crea_fichero(char * f_mypackzip);
char* obtenerNomDir(char * ruta);
char* obtenerNomFich(char * ruta);
int crea_directorio(char * ruta);

//////////////
//sizeOfFile//
//////////////

/**
* @description Given the name of a file returns the size of the aforementioned.
* @param1 nameOfFile The name of the file which to get the size of. The maximum size is 256 bytes.
* @return The size of the file that corresponds to the nameOFile name.
*/
off_t sizeOfFile(char *nameOfFile)
{
	struct stat StVar;
	stat(nameOfFile, &StVar);
	return StVar.st_size;
}

//////////////////
//amountOfBlocks//
//////////////////

/**
* @description Given a file size in bytes and the block size, returns the amount of blocks
*	       needed to store the file.
* @param1 blockSize The block size in bytes.
* @param2 fileSize The size of the file in bytes.
* @return The amount of blocks needed to store the file.
*/
unsigned long amountOfBlocks(unsigned long blockSize, unsigned long fileSize)
{
	int result = fileSize / blockSize;
	if(fileSize % blockSize > 0)
		result++;
	return result;
}

//////////////////
//extrae_fichero//
//////////////////

/**
* @description Extracts file i with 0 <= i < NUM_HEADERS, from zip file f_mypackzip
* 		which has the mypackzip format. If the file is a directory it only creates it.
*		If the file's directory does not exist it creates it.
* @param1 f_mypackzip a pointer to the name of the file to which to extract from.
* @param2 index The index of the file (0 <= i < NUM_HEADERS) to extract from f_mypackzip
* @return If no errors where to occur return(0)
		Errors:
			E_OPEN   (1)
          			 No se puede abrir f_mypackzip.
			E_POS    (2)
				Número de registro erróneo (index).
			E_DIR1	 (3)
				No se puede crear el directorio indicado en (index).
			E_OPEN2  (4)
				El archivo a exportar ya existe.
			E_DESCO  (99)
           			Otro tipo de errores.
*/
int extrae_fichero(char * f_mypackzip, int index)
{
	//Check wrong index:
	if(index < 0 || index >= NUM_HEADERS) return 2;

	int zipFD; // File descriptor for zip file.
	int newFileFD; // To be exported file's file descriptor.
	struct s_mypack_headers headers; //Header container struct, there are 32.
	struct s_Info fileInfo; //Struct containing information of the file to extract.

	//Try opening the file:
	zipFD = open(f_mypackzip, O_RDONLY);

	//Check if errors while opening f_mypackzip:
	if(zipFD < 0) return 1;

	//Getting information needed from the struct header:
       	read(zipFD, &headers, sizeof(struct s_mypack_headers));
        fileInfo = headers.vHead[index].FileInfo;

	if(fileInfo.Type == '\\') return 2;

	//Check file type:
	if(fileInfo.FileType == 'd')
	{
		if(opendir(fileInfo.DataFileName) != NULL)
			return 4;
		else if(crea_directorio(fileInfo.DataFileName) < 0)
			return 3;
	}
	else
	{
		char m[300];
		char * fileName = obtenerNomFich(fileInfo.DataFileName);
		char * fileDir = obtenerNomDir(fileInfo.DataFileName);
                char buf[fileInfo.DataSize]; //Buffer for copying.

		sprintf(m, "fileType => %c\nfileDir => %s\nfileName => %s\n", fileInfo.FileType, fileDir, fileName);
		write(1, m, strlen(m));

		//Check whether the directory of the file exists:
		if(opendir(fileDir) == NULL)
			if(crea_directorio(fileDir) < 0) return 3;

		chdir(fileDir);

		if (fileInfo.FileType == 'r')
                {
			//Check whether the file already exists or create it:
			newFileFD = open(fileName, O_RDWR | O_CREAT, 00600);

			if(newFileFD < 0) return 99;

			if(lseek(newFileFD, 0, SEEK_END) > 0) return 4;
                	lseek(newFileFD, 0, SEEK_SET);

			//Setting the file pointer of the zip to the beginning position of file of index:
			unsigned long filePointer = lseek(zipFD, fileInfo.DatPosition, SEEK_SET);

			//Copying the data:
			read(zipFD, buf, fileInfo.DataSize);
			write(newFileFD, buf, sizeof(buf));

			close(newFileFD);
		}
		else if (fileInfo.FileType == 'l')
       		{
			//Check whether the link already exists:
			if(readlink(fileName, buf, sizeof(buf)) >= 0) return 4;

			//Get original file's path:
			lseek(zipFD, fileInfo.DatPosition, SEEK_SET);
			read(zipFD, buf, fileInfo.DataSize);

			symlink(buf, fileName);
        	}
		else return 99;
	}

	//Extract message:
	char message[256];
	sprintf(message, "\"%s\" extraído con éxito.%d\n", fileInfo.DataFileName,newFileFD);
	write(1, message, strlen(message));

	close(zipFD);

	return 0;
}

///////////////////
//inserta_fichero//
///////////////////

/**
* @description If f_mypackzip does not exist first creates it and then inserts the given
		file or directory (with their corresponding regular files) into the zip in
		the specified index.
		If index is -1 then will find the first empty space.
* @param1 f_mypackzip a pointer to the name of the file to create and/or insert the file in.
* @param2 f_dat a pointer to the file or directory to insert into the zip
* @param3 index The index of the file (0 <= i < NUM_HEADERS) to insert in f_mypackzip.
* @return If no errors where to occur the index where the file has been stored.
                Errors:
                        E_OPEN1		(-1)
                                 f_dat cannot be opened.
			E_OPEN2		(-2)
				f_mypackzip cannot be opened.
                        E_POS1		(-3)
                                Invalid index number.
			E_POS2		(-4)
				Index references s_header containing valid data.
			E_OPENDIR1	(-5)
				f_dat references a directory which cannot be opened.
			E_POS3		(-6)
				All indexes in f_mypackzip contain valid data.
			E_POSDIR	(-7)
				Not enough space to store all regular files inside the given directory.
			E_FILETYPE	(-8)
				Type of file not supported.
                        E_DESCO  	(99)
                    		Other types of errors.
*/
int inserta_fichero(char * f_mypackzip, char * f_dat, int index)
{
	//Check index range:
	if(index < -1 || index >= NUM_HEADERS) return -3;

	int zipFD; // f_mypackzip file descriptor.
	int datFD; // f_dat file descriptor.
	struct s_mypack_headers headers; //Header container struct, there are NUM_HEADERS.
	bzero(&headers, sizeof(struct s_mypack_headers));

	//Check f_dat opening:
        datFD = open(f_dat, O_RDONLY, 00600);
        if(datFD < 0) return -1;

	//Check f_mypackzip opening:
	zipFD = open(f_mypackzip, O_RDWR, 00600);
	if(zipFD < 0)
	{
		zipFD = crea_fichero(f_mypackzip); //Crear e inizializar fichero.
		if(zipFD < 0) return -2;
	}

	//Get headers:
	lseek(zipFD, 0, SEEK_SET); //Reset file pointer.
	read(zipFD, &headers, sizeof(struct s_mypack_headers));

	//Check whether index is -1 for inserting in next empty header, else insert in given index.
	if(index == -1)
	{
		int i = 0;

		while(i < (NUM_HEADERS - 1) && headers.vHead[i].FileInfo.Type == 'b')
			i++;

		//Error: The entire zip is full:
		if(i == (NUM_HEADERS - 1) && headers.vHead[NUM_HEADERS - 1].FileInfo.Type != '\\')
			return -6;

		index = i;
	}

	//Try inseting on index:
	if(headers.vHead[index].FileInfo.Type == 'b') return -4;

	if(es_fichero_regular(f_dat))
	{
		inserta_fichero_regular(zipFD, datFD, f_dat, &headers, index);
		close(datFD);
	}
	else if(es_directorio(f_dat))
	{
		close(datFD);
		int error = inserta_directorio(zipFD, f_dat, &headers, index);
		if(error != 0) return error;
	}
	else if(es_enlace_simbolico(f_dat))
	{
		close(datFD);
		int error = inserta_enlace_simbolico(zipFD, f_dat, &headers, index);
		if(error != 0) return error;
	}
	else return -8;

	//Write the headers:
        lseek(zipFD, 0, SEEK_SET); //Reset to the beginning the file pointer.
        write(zipFD, &headers, sizeof(struct s_mypack_headers));

	close(zipFD);

	return index;
}

//////////////////////
//es_fichero_regular//
//////////////////////

/**
* @description Returns TRUE if the given file name corresponds to
*		a regular file.
* @param1 fileName the name of the file to query.
*/
int es_fichero_regular(char *fileName)
{
	struct stat statVar;

	if(lstat(fileName, &statVar) != 0)
		return 0;
	return S_ISREG(statVar.st_mode);
}

/////////////////
//es_directorio//
/////////////////

/**
* @description Returns TRUE if the given file name corresponds to
*               a directory file.
* @param1 fileName the name of the file to query.
*/
int es_directorio(char *fileName)
{
        struct stat statVar;

        if(lstat(fileName, &statVar) != 0)
                return 0;
        return S_ISDIR(statVar.st_mode);
}

///////////////////////
//es_enlace_simbolico//
///////////////////////

/**
* @description Returns TRUE if the given file name corresponds to
*               a symbolic (by name) linked file.
* @param1 fileName the name of the file to query.
*/
int es_enlace_simbolico(char *fileName)
{
        struct stat statVar;

        if(lstat(fileName, &statVar) != 0)
                return 0;
        return S_ISLNK(statVar.st_mode);
}

///////////////////////////
//inserta_fichero_regular//
///////////////////////////

/**
* @description Auxiliary function of "inserta_fichero" for inserting a regular file.
* @param1 zipFD File Descriptor of the mypackzip to insert the file.
* @param2 dataFD File Descriptor of the data file to be inserted into the mypackzip.
* @param3 f_dat[] Name of the data file to be inserted into the mypackzip.
* @param4 headers Headers to be modified for inserting the data file into the mypackzip.
* @param5 index Index where to be inserted the data file into the mypackzip.
* @return Returns 0 if the execution is correct.
*/

int inserta_fichero_regular(int zipFD, int datFD, char * f_dat, struct s_mypack_headers *headers, int index)
{
	headers->vHead[index].FileInfo.Type = 'b';
        headers->vHead[index].FileInfo.Compress = 'n';

        unsigned long fileSize = sizeOfFile(f_dat);

        headers->vHead[index].FileInfo.DataSize = fileSize;
        headers->vHead[index].FileInfo.CompSize = fileSize;

        unsigned long blockAmount = amountOfBlocks(BLOCK_SIZE, fileSize);

        headers->vHead[index].FileInfo.NumBlocks = blockAmount;
        sprintf(headers->vHead[index].FileInfo.DataFileName, f_dat);
        headers->vHead[index].FileInfo.DatPosition = lseek(zipFD, 0, SEEK_END);
	headers->vHead[index].FileInfo.FileType = 'r';

        //Write the data into the zip:
        int n; //Read amount of data.
        char buf[BLOCK_SIZE]; //Buffer for copying data.
	lseek(zipFD, headers->vHead[index].FileInfo.DatPosition, SEEK_SET); //Set the pointer to the position of the header.

	        while((n = read(datFD, buf, BLOCK_SIZE)) > 0)
                write(zipFD, buf, n);

                        //Fill with 0s the remaining space of the block:
                        int remainingBytes = BLOCK_SIZE * blockAmount - headers->vHead[index].FileInfo.DataSize;

                        if(remainingBytes > 0)
                        {
                                char zeroes[remainingBytes];
                                bzero(zeroes, remainingBytes);
                                write(zipFD, zeroes, remainingBytes);
                        }
	return 0;
}

//////////////////////
//inserta_directorio//
//////////////////////

/**
* @description Auxiliary function of "inserta_fichero" for inserting a directory and its regular files..
* @param1 zipFD File Descriptor of the mypackzip to insert the file.
* @param3 f_dat[] Name of the data file to be inserted into the mypackzip.
* @param4 headers Headers to be modified for inserting the data file into the mypackzip.
* @param5 index Index where to be inserted the data file into the mypackzip.
* @return Returns 0 if the execution is correct.
		Errors:
			E_OPENFILE	(-1)
				f_dat cannot be opened.
                        E_OPENDIR1      (-5)
                                f_dat references a directory which cannot be opened.
                        E_POSDIR        (-7)
                                Not enough space to store all regular files inside the given directory.

*/
int inserta_directorio(int zipFD, char *f_dat, struct s_mypack_headers *headers, int index)
{
	int nRegularFiles = 0; //Amount of regular files inside f_dat directory;
	int nZip = 0; //Amount of available (empty) indexes in mypackzip.
	DIR *fdir; //f_dat directory stream.
	struct dirent *input; //Each of the file information will be stored inside.
	char s_regFiles[NUM_HEADERS][MAX_FILE_NAME]; //Array containing all the regular file names in the directory.
	int iZip[NUM_HEADERS]; //Array containing the available indexes inside mypackzip.
	char fileName[MAX_FILE_NAME];
	int i, datFD;

	//Open directory:
	if((fdir = opendir(f_dat)) == NULL)
		return -5;

	//Count amount of regular files in the directory:
	while((input = readdir(fdir)) != NULL)
	{
		sprintf(fileName, "%s/%s", f_dat, input->d_name); //Set current relative path to file.
		if(es_fichero_regular(fileName))
			sprintf(s_regFiles[nRegularFiles++], fileName);

	}
	//Count amount of available space in mypackzip:
	for(i = 1; i < NUM_HEADERS; i++)
	{
		if(headers->vHead[(i + index) % NUM_HEADERS].FileInfo.Type == '\\')
			iZip[nZip++] = (i + index) % NUM_HEADERS;
	}

	//Not enough available space in zip to store directory:
	if(nRegularFiles > nZip) return -7;

	headers->vHead[index].FileInfo.Type = 'b';
        headers->vHead[index].FileInfo.Compress = 'n';

        headers->vHead[index].FileInfo.DataSize = 0L;
        headers->vHead[index].FileInfo.CompSize = 0L;

        headers->vHead[index].FileInfo.NumBlocks = 0L;
        sprintf(headers->vHead[index].FileInfo.DataFileName, f_dat);
	headers->vHead[index].FileInfo.DatPosition = 0L;

	headers->vHead[index].FileInfo.FileType = 'd';

	//Write the regular files inside the mypackzip:
	for(i = 0; i < nRegularFiles; i++)
	{
		datFD = open(s_regFiles[i], O_RDONLY, 00600);
		if(datFD < 0)
		{
			char mensaje[MAX_FILE_NAME * 2 + 60];
			sprintf(mensaje, "Error al abrir el fichero \"%s\".\n", f_dat);
			write(2, mensaje, strlen(mensaje));
			return -1;
		}
		inserta_fichero_regular(zipFD, datFD, s_regFiles[i], headers, iZip[i]);
		close(datFD);
	}

        return 0;
}

////////////////////////////
//inserta_enlace_simbolico//
////////////////////////////

/**
* @description Auxiliary function of "inserta_fichero" for inserting a symbolic link file.
* @param1 zipFD File Descriptor of the mypackzip to insert the file.
* @param3 f_dat Name of the file to be inserted into the mypackzip.
* @param4 headers Headers to be modified for inserting the data file into the mypackzip.
* @param5 index Index where to be inserted the data file into the mypackzip.
* @return Returns 0 if the execution is correct.
*/
int inserta_enlace_simbolico(int zipFD, char * f_dat, struct s_mypack_headers *headers, int index)
{
        headers->vHead[index].FileInfo.Type = 'b';
        headers->vHead[index].FileInfo.Compress = 'n';

        headers->vHead[index].FileInfo.DataSize = MAX_FILE_NAME;
        headers->vHead[index].FileInfo.CompSize = MAX_FILE_NAME;

        headers->vHead[index].FileInfo.NumBlocks = 1L;
        sprintf(headers->vHead[index].FileInfo.DataFileName, f_dat);
        headers->vHead[index].FileInfo.DatPosition = lseek(zipFD, 0, SEEK_END);
        headers->vHead[index].FileInfo.FileType = 'l';

        //Write sybolic link path into the zip:
        char buf[MAX_FILE_NAME]; //Buffer for copying data.
        lseek(zipFD, headers->vHead[index].FileInfo.DatPosition, SEEK_SET); //Set the pointer to the position of the header.

		if(readlink(f_dat, buf, sizeof(buf)) < 0) return -1;
		else write(zipFD, buf, strlen(buf));

                        //Fill with 0s the remaining space of the block:
                        int remainingBytes = BLOCK_SIZE - strlen(buf);

                        if(remainingBytes > 0)
                        {
                                char zeroes[remainingBytes];
                                bzero(zeroes, remainingBytes);
                                write(zipFD, zeroes, remainingBytes);
                        }
        return 0;
}

////////////////
//crea_fichero//
////////////////

/**
* @description Given a file zip name creates it and initializes empty headers. WARNING!: DOES NOT CLOSE FILE
* @param1 blockSize Name of the zip to be created.
* @return The result of open function: fd or error.
*/
int crea_fichero(char * f_mypackzip)
{
	//Create file:
	int thisFD = open(f_mypackzip, O_CREAT | O_RDWR, 00600);
	//Check open errors:
	if(thisFD < 0) return thisFD;

	//Initialize headers:
	struct s_mypack_headers headers; //Header container struct, there are NUM_HEADERS.
	bzero(&headers, sizeof(struct s_mypack_headers)); //Clear headers memory space.

	int i;
	for (i = 0; i < NUM_HEADERS; i++)
	{
		bzero(headers.vHead[i].Dummy, DUMMY_SIZE);

		headers.vHead[i].FileInfo.Type = '\\';
		headers.vHead[i].FileInfo.Compress = 'n';

		headers.vHead[i].FileInfo.DataSize = 0L;
		headers.vHead[i].FileInfo.CompSize = 0L;

		headers.vHead[i].FileInfo.NumBlocks = 0L;

		bzero(headers.vHead[i].FileInfo.DataFileName, MAX_FILE_NAME);
		headers.vHead[i].FileInfo.DatPosition = 0L;
		headers.vHead[i].FileInfo.FileType = 'o';
	}

	//Clear thisFD's memory space:
	char buf[sizeof(struct s_mypack_headers)];
	bzero(buf, sizeof(struct s_mypack_headers));
	write(thisFD, buf, sizeof(struct s_mypack_headers));
	lseek(thisFD, 0, SEEK_SET);

	//Write the headers into the zip file:
	write(thisFD, &headers, sizeof(struct s_mypack_headers));
	return thisFD;
}

/////////////////
//obtenerNomDir//
/////////////////

/**
* @description Given the path of a file, extracts the path of it's directory.
* @param1 ruta The path of the file where to extract the path of it's directory from.
* @return The path of the directory from the path file of param1.
*/
char* obtenerNomDir(char * ruta)
{
	int i;
	int lastSlashIndex = -1;

	for(i = 0; i < strlen(ruta); i++)
		if(ruta[i] == '/')
			lastSlashIndex = i;

	char * result;

	if(lastSlashIndex != -1)
	{
		result = (char*) malloc(lastSlashIndex * sizeof(char) + 1);
		for(i = 0; i <= lastSlashIndex; i++)
			result[i] = ruta[i];
		result[i] = '\0';
	}
	else
	{
		result = (char*) malloc(2 * sizeof(char) + 1);
		sprintf(result, "./");
	}

	return result;
}

//////////////////
//obtenerNomFich//
//////////////////

/**
* @description Given the path of a file, extracts the name of the file.
* @param1 ruta The path of the file where to extract its name.
* @return The name of the file without the directory path.
*/
char* obtenerNomFich(char * ruta)
{
        int i;
	int lastSlashIndex = -1;

        for(i = 0; i < strlen(ruta); i++)
                if(ruta[i] == '/')
                        lastSlashIndex = i;

        char * result;

        if(lastSlashIndex != -1)
        {
                result = (char*) malloc((strlen(ruta) - lastSlashIndex) * sizeof(char));
                int j = 0;
		for(i = lastSlashIndex + 1; i < strlen(ruta); i++)
                        result[j++] = ruta[i];
		result[j] = '\0';
        }
	else
	{
                result = (char*) malloc(strlen(ruta) * sizeof(char));
                sprintf(result, ruta);
        }

	return result;
}

///////////////////
//crea_directorio//
///////////////////

/**
* @description Given the path of a directory, makes all the subdirectories until the final one is created.
* @param1 ruta The path of directory to make.
* @return 0 If no errors were to occur, -1 elsewhere.
*/

int crea_directorio(char * ruta)
{
	char path_part[strlen(ruta) + 1];
	int i;
	DIR *ddir;
	for(i = 0; i <= strlen(ruta); i++)
	{
		if(i != strlen(ruta))
		{
			path_part[i] = ruta[i];
			path_part[i + 1] = '\0';
		}

		if(ruta[i] == '/' || i == strlen(ruta))
			if((ddir = opendir(path_part)) == NULL)
				if(mkdir(path_part, S_IRWXU) < 0)
					return -1;
			else closedir(ddir);
	}
	return 0;
}

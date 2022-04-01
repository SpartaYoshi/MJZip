#include <stdio.h> //Input/output library functions. DO NOT USE fopen, fclose, fwrite, fread!
#include <unistd.h> //Unix standard symbolic constants and types.
#include <sys/stat.h> //Data types returned by the stat() function (for accessing file metadata).
#include <string.h> //For working with strings instead of char[].
#include <fcntl.h> //File control options
#include <stdlib.h> //atoi function.

#include "s_header.h" //For including the s_header struct type definition.
#include "mypackzip_functions.h"

/**
*lsmypackzip imprime en la salida estandar la información de los ficheros contenidos en f_mypackzip:
*Formato (tipo ls): índice permisos tamaño(bytes) tamaño_comprimido(bytes) nombre_del_fichero(COLOR: azul=directorio, verde=fichero_regular, rojo=enlace_simbólico)
*/
int main(int argc, char *argv[])
{
	if(argc < 2)
        {
		char mensaje[256];
                sprintf(mensaje, "Uso: %s f_mypackzip\n", argv[0]);
                write(1, mensaje, strlen(mensaje));
                _exit(-1);
        }

	return ls_mypackzip(argv[1]);
}

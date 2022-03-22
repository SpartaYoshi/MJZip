/**
* @file extractmypackzip.c
* @author   ISO-2-02
* @date     20/02/2022
* @brief    Testing program for function extrae_fichero.
*/

#include <stdio.h> //Input/output library functions. DO NOT USE fopen, fclose, fwrite, fread!
#include <unistd.h> //Unix standard symbolic constants and types.
#include <sys/stat.h> //Data types returned by the stat() function (for accessing file metadata).
#include <string.h> //For working with strings instead of char[].
#include <fcntl.h> //File control options

#include "s_header.h" //For including the s_header struct type definition.

int main(int argc, char * argv[])
{

	char mensaje[257];

	if(argc < 3)
	{
		sprintf(mensaje, "Uso: %s f_mypackzip index\n", argv[0]);
		write(1, mensaje, strlen(mensaje));
		_exit(-1);
	}

	int resultado = extrae_fichero(argv[1], atoi(argv[2]));
	if( resultado == 0)
		_exit(0);
	else if(resultado == 1)
	{
		sprintf(mensaje, "No se puede abrir %s.\n", argv[1]);
		write(1, mensaje, strlen(mensaje));
		_exit(1);
	}
	else if(resultado == 2)
		sprintf(mensaje, "Número de registro erroneo.\n");
	else if(resultado == 3)
		sprintf(mensaje, "El archivo a exportar ya existe.\n");
	else if(resultado == 4)
		sprintf(mensaje, "El registro (índice) está vacio.\n");
	else
		sprintf(mensaje, "Error al extraer archivo.\n");

	write(2, mensaje, strlen(mensaje));
	_exit(resultado);
}

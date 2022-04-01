/**
* @file insertmypackzip.c
* @author   ISO-2-02
* @date     20/02/2022
* @brief    Testing program for function inserta_fichero.
*/

#include <stdio.h> //Input/output library functions. DO NOT USE fopen, fclose, fwrite, fread!
#include <unistd.h> //Unix standard symbolic constants and types.
#include <sys/stat.h> //Data types returned by the stat() function (for accessing file metadata).
#include <string.h> //For working with strings instead of char[].
#include <fcntl.h> //File control options
#include <stdlib.h> //atoi function.

#include "s_header.h" //For including the s_header struct type definition.
#include "mypackzip_functions.h" //Function descriptions for mypackzip project.

int main(int argc, char * argv[])
{

	char mensaje[257];

	if(argc < 4)
	{
		sprintf(mensaje, "Uso: %s f_mypackzip f_dat, index\n", argv[0]);
		write(1, mensaje, strlen(mensaje));
		_exit(-1);
	}

	int resultado = inserta_fichero(argv[1], argv[2], atoi(argv[3]));

	if(resultado >= 0)
	{
		sprintf(mensaje, "El índice vacio donde se ha guardado el fichero es: %d\n", resultado);
		write(1, mensaje, strlen(mensaje));
		_exit(0);
	}
	else if(resultado == -1)
		sprintf(mensaje, "No se puede abrir %s\n", argv[2]);
	else if(resultado == -2)
		sprintf(mensaje, "No se puede abrir o crear %s\n", argv[1]);
	else if(resultado == -3)
		sprintf(mensaje, "Numero de índice erroneo\n");
	else if(resultado == -4)
		sprintf(mensaje, "Index hace referencia a un s_header que ya contiene datos válidos.\n");
	else if(resultado == -5)
		sprintf(mensaje, "\"%s\" referencia un directorio que no se puede abrir.\n", argv[2]);
	else if(resultado == -6)
		sprintf(mensaje, "Todos los índices de \"%s\" están ocupados.\n", argv[1]);
	else if(resultado == -7)
		sprintf(mensaje, "No hay suficientes índices libres en \"%s\" para albergar todos los ficheros regulares dentro de \"%s\".\n", argv[1], argv[2]);
	else if(resultado == -8)
		sprintf(mensaje, "Fichero con formato no soportado o que contiene ficheros no soportados.\n");
	else if(resultado == -9)
		sprintf(mensaje, "Error al intentar comprimir fichero regular.\n");
	else
		sprintf(mensaje, "Error al insertar fichero en zip.\n");

	write(2, mensaje, strlen(mensaje));
	_exit(resultado);
}

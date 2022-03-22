#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	printf("Es un fichero regular = %d\nEs un directorio = %d\n",es_fichero_regular(argv[1]), es_directorio(argv[1]));
}

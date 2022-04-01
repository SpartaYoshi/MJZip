//Colours for bash:
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KBLU "\x1B[34m"
#define KGRN "\x1B[32m"
#define KMAG "\x1B[35m"


off_t sizeOfFile(char *nameOfFile);

unsigned long amountOfBlocks(unsigned long blockSize, unsigned long fileSize);

int extrae_fichero(char * f_mypackzip, int index);

int extraer_fichero_recursivo(int *zipFD, struct s_mypack_headers *headers, int *index);

int inserta_fichero(char * f_mypackzip, char * f_dat, int index);

int es_fichero_regular(char *fileName);

int es_directorio(char *fileName);

int es_enlace_simbolico(char *fileName);

mode_t obtener_permisos(char *filename);

int inserta_fichero_regular(int zipFD, int datFD, char * f_dat, struct s_mypack_headers *headers, int index);

int inserta_directorio(int zipFD, char *f_dat, struct s_mypack_headers *headers, int index);

int obtener_ficheros_recursivo(char * dirPath, struct s_mypack_headers * tempHeaders, int * tempI, int * nZip);

int inserta_enlace_simbolico(int zipFD, char *f_dat, struct s_mypack_headers *headers, int index);

int crea_fichero(char * f_mypackzip);

char* obtenerNomDir(char * ruta);

char* obtenerNomFich(char * ruta);

int crea_directorio(char * ruta, mode_t mode);

int ls_mypackzip(char * f_mypackzip);

void permToString(char * permS, mode_t perm);

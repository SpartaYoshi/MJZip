gcc -Wall insertmypackzip.c -c
gcc -Wall mypackzip_functions.c -c
gcc ./zpipe_sis/zpipe_sis.c -c
gcc -Wall -lz insertmypackzip.o zpipe_sis.o mypackzip_functions.o -o insertmypackzip
gcc -Wall extractmypackzip.c -c
gcc -Wall -lz extractmypackzip.o zpipe_sis.o mypackzip_functions.o -o extractmypackzip
gcc -Wall lsmypackzip.c -c
gcc -Wall -lz lsmypackzip.o zpipe_sis.o mypackzip_functions.o -o lsmypackzip

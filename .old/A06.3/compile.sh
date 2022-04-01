gcc insertmypackzip.c -c
gcc mypackzip_functions.c -c
gcc insertmypackzip.o mypackzip_functions.o -o insertmypackzip
gcc extractmypackzip.c -c
gcc extractmypackzip.o mypackzip_functions.o -o extractmypackzip
gcc createmypackzip.c -c
gcc createmypackzip.o mypackzip_functions.o -o createmypackzip

Para compilar:
--------------

 gcc zpipe_sis.c -lz -o zpipe_sis


Para Comprimir:
--------------


 ./zpipe_sis < fich1.dat > fich1.dat.zip

 ls -al

Para descomprimir:
------------------

 ./zpipe_sis -d <fich1.dat.zip > fich2.dat
 ls -al


Para validar:
-------------

 ls -al
 diff fich1.dat fich2.dat


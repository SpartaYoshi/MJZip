bash compile.sh
./insertmypackzip toDelete pruebas 0
mv pruebas pruebas_old
./extractmypackzip toDelete 0
./extractmypackzip toDelete 1
./extractmypackzip toDelete 2
./extractmypackzip toDelete 3
ls -ali pruebas
rm -r pruebas
rm toDelete
mv pruebas_old pruebas


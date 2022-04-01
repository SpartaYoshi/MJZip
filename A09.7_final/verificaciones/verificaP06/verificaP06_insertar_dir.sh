#####
echo $(tput setaf 6)---------------------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de insertar directorio -$(tput sgr 0)
echo $(tput setaf 6)---------------------------------------$(tput sgr 0)
#Casos de prueba
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dir1/ 0
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dir1/ -1
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dir1/ 2
rm bottle.mj

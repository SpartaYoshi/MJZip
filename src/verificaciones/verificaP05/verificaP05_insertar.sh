echo $(tput setaf 6)----------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de insertar -$(tput sgr 0)
echo $(tput setaf 6)----------------------------$(tput sgr 0)
#Casos de prueba
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/infinight.txt 2
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt 2
#4
echo $(tput setaf 3)Caso 4: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/vacio.txt 3
#5
echo $(tput setaf 3)Caso 5: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 3
#6
echo $(tput setaf 3)Caso 6: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 31
#7
echo $(tput setaf 3)Caso 7: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 33
#8
echo $(tput setaf 3)Caso 8: $(tput sgr 0)
for i in {0..31}
do
../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1
done
../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1

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
#4
echo $(tput setaf 3)Caso 4: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/vacio.txt 16
../../insertmypackzip bottle.mj ../testfiles/dir1/ 16
#5
echo $(tput setaf 3)Caso 5: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dir1/ 30
#6
echo $(tput setaf 3)Caso 6: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dir2/ 35
#7
echo $(tput setaf 3)Caso 7: $(tput sgr 0)
for i in {0..15}
do
../../insertmypackzip fullbottle.mj ../testfiles/dir2/ -1
done
../../insertmypackzip fullbottle.mj ../testfiles/dir1/ -1
#8
echo $(tput setaf 3)Caso 8: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/dirrec/dirrec2/ 10
rm bottle.mj fullbottle.mj

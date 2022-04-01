#####
echo $(tput setaf 6)------------------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de insertar enlaces -$(tput sgr 0)
echo $(tput setaf 6)------------------------------------$(tput sgr 0)
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/enlaces/infi 0
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/enlaces/ 4
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/enlaces/enlace_de_enlace/ 20
#hexdump -C bottle.mj
rm bottle.mj



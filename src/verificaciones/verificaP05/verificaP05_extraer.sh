#####
echo $(tput setaf 6)---------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de extraer -$(tput sgr 0)
echo $(tput setaf 6)---------------------------$(tput sgr 0)
#Casos de prueba
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../extractmypackzip bottle.mj 0
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
rm ../testfiles/infinight.txt
../../extractmypackzip bottle.mj 2
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
../../extractmypackzip bottle.mj 35
#4
echo $(tput setaf 3)Caso 4: $(tput sgr 0)
rm bottle.mj
for i in {0..15}
do
../../insertmypackzip bottle.mj ../testfiles/infinight.txt -1
done
../../extractmypackzip bottle.mj 16
# hexdump -C bottle.mj
rm bottle.mj

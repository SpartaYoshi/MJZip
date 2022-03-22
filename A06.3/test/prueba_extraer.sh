#extraer
#1
../extractmypackzip bottle.mj 0
#2
../extractmypackzip bottle.mj 2
#3
../extractmypackzip bottle.mj 35
#4
rm bottle.mj
for i in {0..15}
do
../insertmypackzip bottle.mj ../testfiles/infinight.txt -1
done
../extractmypackzip bottle.mj 16 
hexdump -C bottle.mj

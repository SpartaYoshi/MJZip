rm bottle.mj
#insertar
#1
../../insertmypackzip bottle.mj ../testfiles/infinight.txt 2
#2
../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1
#3
../../insertmypackzip bottle.mj ../testfiles/corto.txt 2
#4
../../insertmypackzip bottle.mj ../testfiles/vacio.txt 3
#5
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 3
#6
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 31
#7
../../insertmypackzip bottle.mj ../testfiles/testfile.txt 33
#8
for i in {0..31}
do
	../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1
done

../../insertmypackzip bottle.mj ../testfiles/ciberataque.txt -1

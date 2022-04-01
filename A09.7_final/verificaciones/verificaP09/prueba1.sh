../../compile.sh
rm ../../bottle.mj
../../insertmypackzip bottle.mj ../testfiles/upv.txt 0
../../insertmypackzip bottle.mj ../testfiles/dir1/ 14
#./extractmypackzip bottle.mj 0
#./extractmypackzip bottle.mj 1
#./extractmypackzip bottle.mj 2
#./extractmypackzip bottle.mj 3
../../lsmypackzip bottle.mj


echo $(tput setaf 6)---------------------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de permisos de fichero -$(tput sgr 0)
echo $(tput setaf 6)---------------------------------------$(tput sgr 0)
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../insertmypackzip bottle09.mj ../testfiles/sinperm.txt 0
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
../../insertmypackzip bottle09.mj ../testfiles/perm-rusr.txt 0
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
rm -f ../testfiles/perm-rusr.txt
../../extractmypackzip bottle09.mj 0
#4
echo $(tput setaf 3)Caso 4: $(tput sgr 0)
../../insertmypackzip bottle09.mj ../testfiles/programita 1
#5
echo $(tput setaf 3)Caso 5: $(tput sgr 0)
rm ../testfiles/programita
../../extractmypackzip bottle09.mj 1
#6
echo $(tput setaf 3)Caso 6: $(tput sgr 0)
../../insertmypackzip bottle09.mj ../testfiles/perm-rnotusr.txt 2
#7
echo $(tput setaf 3)Caso 7: $(tput sgr 0)
../../insertmypackzip bottle09.mj ../testfiles/dirp/ 5
#8
echo $(tput setaf 3)Caso 8: $(tput sgr 0)
rm -r -f ../testfiles/dirp/
../../extractmypackzip bottle09.mj 5
#9
echo $(tput setaf 3)Caso 9: $(tput sgr 0)
mv ../testfiles/sinperm.txt ../testfiles/perm-rnotusr.txt ../testfiles/dirp/
../../insertmypackzip bottle09v2.mj ../testfiles/dirp/ 0
mv ../testfiles/dirp/sinperm.txt ../testfiles/dirp/perm-rnotusr.txt ../testfiles/
#10
echo $(tput setaf 3)Caso 10: $(tput sgr 0)
../../extractmypackzip bottle09.mj 1
#11
echo $(tput setaf 3)Caso 11: $(tput sgr 0)
cp ../testfiles/infinight.txt ../testfiles/dirp2/
../../insertmypackzip bottle09v2.mj ../testfiles/dirp2/ 0
rm ../testfiles/dirp2/infinight.txt
../../extractmypackzip bottle09v2.mj 0
######
echo $(tput setaf 3)----------------------------------$(tput sgr 0)
../../lsmypackzip bottle09.mj
echo $(tput setaf 3)----------------------------------$(tput sgr 0)
../../lsmypackzip bottle09v2.mj
######
rm bottle09.mj bottle09v2.mj

echo $(tput setaf 6)----------------------------------------------$(tput sgr 0)
echo $(tput setaf 6)"- Verificación de propiedad de terminal \"ls\" -"$(tput sgr 0)
echo $(tput setaf 6)----------------------------------------------$(tput sgr 0)
../../insertmypackzip bottle.mj ../testfiles/upv.txt 3
../../insertmypackzip bottle.mj ../testfiles/infinight.txt -1
../../insertmypackzip bottle.mj ../testfiles/dir1/ 14
../../insertmypackzip bottle.mj ../testfiles/programita 10
../../insertmypackzip bottle.mj ../testfiles/link_programita 29
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
../../lsmypackzip bottle.mj
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
../../lsmypackzip
#3
echo $(tput setaf 3)Caso 3: $(tput sgr 0)
../../lsmypackzip nonexistent.mj
#####
rm bottle.mj

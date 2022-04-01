#####
echo $(tput setaf 6)--------------------------------$(tput sgr 0)
echo $(tput setaf 6)- Verificación de recursividad -$(tput sgr 0)
echo $(tput setaf 6)--------------------------------$(tput sgr 0)
#1
echo $(tput setaf 3)Caso 1: $(tput sgr 0)
mkdir p
cd p
touch text
chmod 700 text
ln -s text text_link
mkdir p2
cd p2
echo MJ loves Spider-Man > text
ln -s text text_link
chmod 770 text_link
mkdir p3
cd p3
for i in {1..25}
do
echo Esto es una prueba, espero que funcione! > text_$i
done
cd ../../..
tree p > tree_original
../../insertmypackzip toDelete p 3
#../../lsmypackzip toDelete
rm -r p
../../extractmypackzip toDelete 3
tree p > tree_exported
#2
echo $(tput setaf 3)Caso 2: $(tput sgr 0)
touch pato.txt
../../insertmypackzip toDelete pato.txt -1
#####
rm toDelete
rm pato.txt
rm -r p
diff tree_original tree_exported

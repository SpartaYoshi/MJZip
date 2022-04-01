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
rm toDelete
rm -r p
diff tree_original tree_exported
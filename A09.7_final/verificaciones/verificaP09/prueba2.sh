rm toDelete
rm -r p
mkdir p
cd p
touch text
chmod 700 text
ln -s text text_link
mkdir p2
cd p2
touch text
ln -s text text_link
chmod 770 text_link
mkdir p3
cd p3
for i in {1..25}
do
 touch text_$i
done
cd ../../..
../../insertmypackzip toDelete p 3
../../lsmypackzip toDelete
rm -r p
#./extractmypackzip toDelete 3

#!/bin/bash
filename="main.c"
touch $filename
echo '#include<stdio.h>
#include<stdlib.h>
#include<assert.h>'>$filename

for file in test_*.h; do
	echo "#include \"$file\"">>$filename
done

echo '
int main (void)
{'>>$filename

for file in test_*.h; do
	grep test_ "$file"| cut -d' ' -f2|cut -d'(' -f1 |sed -e "s/$/();/g" >>$filename
	nom=$(echo "$file"|cut -d'_' -f2|cut -d'.' -f1)
	echo "printf(\"Tests fonctions $nom réussis\n\");" >>$filename
done
echo 'return 0;
}'>> $filename

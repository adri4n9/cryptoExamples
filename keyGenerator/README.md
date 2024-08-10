this is a AES128 key generator

you can compile it using the following command

gcc -o keygen wbaes128_unprotected_tables_generator.c

the generator takes a ASCII string as input and converts the values in a whitebox key
usage:
./keygen "keyvalue" 

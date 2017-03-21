 gcc -o server2.o server2.c
 gcc -o client2.o client2.c
 ./server2.o 12345 &
./client2.o "127.0.0.1" 12345 "hello world!"
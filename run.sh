pkill server
gcc server.c -o server
./server &
gcc client.c -o client
./client

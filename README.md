Decode MD5 password using MPI parallel programing
------------------------------
# How to run?
In Linux there is a file: run.sh

Just do: ./run.sh and input your md5 string.

In windows: 
mpicc main.c -o md5run -lcrypto

mpirun -np 4 ./md5run
------------------------------
Make sure that you are already install openssl library and mpi.h
Video demo: https://youtu.be/9wMHJqPdnrQ
------------------------------

Copyright : LinhPhan
Contact me: fb.com/deluxe.psk
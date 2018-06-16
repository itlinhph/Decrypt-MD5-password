Decode MD5 password using MPI parallel programing
------------------------------
# How to run?
In Linux there is a file: run.sh <br/>
Just do: ./run.sh and input your md5 string. <br/>

In windows: <br/>
mpicc main.c -o md5run -lcrypto <br/>
mpirun -np 4 ./md5run

------------------------------
Make sure that you are already install openssl library and mpi.h <br/>
Video demo: https://youtu.be/9wMHJqPdnrQ

------------------------------
#########
Copyright : LinhPhan <br/>
Contact me: fb.com/deluxe.psk

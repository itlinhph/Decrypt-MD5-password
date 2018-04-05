# mpicc md5.c -o MPIrun
# mpirun -np 4 ./MPIrun
# gcc md5.c -o md4

mpicc md5.c -o md5run -lcrypto
mpirun -np 6 ./md5run

# gcc exmd5.c -o md5 -lcrypto
# ./md5
# tic-tac-toe-window-ubuntu:
using the bridged network of Oracle virtual machine
python (judge) runs on a real machine (windows), server and clients run on a Ubuntu 20.04 (Oracle virtual machine) and communicate to each other through TCP/IP.

python (on a real machine) communicates to the server (on a virtual machine) through zmq

if you don't use my configuration (virtual machine) but 2 separate PCs, it also should work but make sure they are connected without any obstacle (i.e can ping IP or send through a socket)

also, ensure the ports (for both server clients and judge-server) are not locked by any anti-virus

make sure you installed GCC (I use version 9.3) on the Ubuntu-based virtual machine and Python (3.7) on the real machine

install package of zmq on the virtual machine: sudo apt-get install libzmq3-de

install zmq for Python (from pip or whatever)

compile server: gcc -pthread -o server server.c -lzmq

compile client: gcc -pthread -o client client.c

how to run:
1. on real machine, run python (tic_tac_toe.py)
2. on the virtual machine, cd to the directory of the server, run the server
3.  run 1st client
4. run 2nd client
5. play the game !!

see detail how to run at https://www.youtube.com/watch?v=J5eWpaGk2Sc

The judge in python is adopted and modified from https://medium.com/byte-tales/the-classic-tic-tac-toe-game-in-python-3-1427c68b8874


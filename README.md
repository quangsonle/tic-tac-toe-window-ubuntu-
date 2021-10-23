# tic-tac-toe-window-ubuntu:
using the bridged network of oracle virtual machine
python (judge) runs on real machine (windows), server and clients run on ubuntu 20.04 (Oracle virtual machine) and communicate to each other through TCP/IP.

python (on real machine) communicates to server (on virtual machine) thorugh zmq

make sure you install gcc (I use version 9.3) on virtual machine and python (3.7) on real machine

install package of zmq on virtual machine: sudo apt-get install libzmq3-de

install zmq for python (from pip or whatever)

compile server: gcc -pthread -o server server.c -lzmq

compile client: gcc -pthread -o client client.c

how to run:
1. on real machine, run python (tic_tac_toe.py)
2. on virtual machine, cd to the directory of server, run server
3.  run 1st client
4. run 2nd client
5. play the game !!

see detail how to run at https://www.youtube.com/watch?v=J5eWpaGk2Sc


#Implementation of Two Player Tic-Tac-Toe game in Python.
import time
import zmq

turn = 'X'


''' We will make the board using dictionary 
    in which keys will be the location(i.e : top-left,mid-right,etc.)
    and initialliy it's values will be empty space and then after every move 
    we will change the value according to player's choice of move. '''
context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://192.168.0.175:5555")# window-based real machine's ip


board_keys = []
list_board=[1,0,0,0,0,0,0,0,0,0]  # this is the game's status: the first element respresents the status, the next 9 elements are the statuses of 9 cells (88 (X) or 79(O))

'''
for the first element:
if it is:
0: invalid
1: the 1 st player (X) just sent
2: the 2 nd player (O) just sent
3: 1 st player (X) won
4: 2 nd player (O) won
6: 2 nd player (O) won since timer of X player is out  
5: 1 nd player (X) won since timer of O player is out

N0te that 5 and 6 are done in server, not here
'''

#for key in theBoard:
    #board_keys.append(key)

''' We will have to print the updated board after every move in the game and 
    thus we will make a function in which we'll define the printBoard function
    so that we can easily print the board everytime by calling this function. '''

def printBoard(board):
    print( chr(board[7]) + '|' +  chr(board[8]) + '|' +  chr(board[9]))
    print('-+-+-')
    print( chr(board[4]) + '|' +  chr(board[5]) + '|' +  chr(board[6]))
    print('-+-+-')
    print( chr(board[1]) + '|' +  chr(board[2]) + '|' +  chr(board[3]))

# Now we'll write the main function which has all the gameplay functionality.
def game():

    global turn
    list_board[0]=0
    count = 0
    
    print('sent time out\n')
    printBoard(list_board)
    while(True):
        
        print("It's your turn," + turn + ".Move to which place?")

        
              
        message = socket.recv() # get zmq message
        
        #print(message
        #print("Received request: %d, %s"%( int(message[0]),str(int(message[1])-48)))
        if (int(message[1])==10):   # this is the "new feed" character, it will be ignored
         socket.send(bytearray((10)))#no respond 
         #print('sent no reposnse\n')
         continue
        if ((turn == 'X' and int(message[0])==48 )or(turn == 'O' and int(message[0])==49)):  # check if the right player is responded 
            
            move=int(message[1])-48
            if turn =='X': # update status
             
             list_board[0]=2
            else:
             
             list_board[0]=1
            
     
            #  Send reply back to client
            
            if list_board[move] == 0:
                #list_board[move] = turn
                if (turn=='X'):
                 list_board[int(message[1])-48]=88#ascii of X
                if (turn=='O'):
                 list_board[int(message[1])-48]=79#ascii of O
                count += 1
            else:
                print("That place is already filled.\nMove to which place?")
                socket.send(bytearray((10)))#no respond
                #print('sent no reposnse, the place is filled\n')
                continue
            
            # Now we will check if player X or O has won,for every move after 5 moves. 
            #print('count is %d'%(count))
            
            if count >= 5:
                if (turn=='X'):
                 list_board[0]=3#X wins
                if (turn=='O'):
                 list_board[0]=4#O wins
                if list_board[7] == list_board[8] == list_board[9] != 0: # across the top
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")       
                    socket.send(bytearray(list_board))                    
                    break
                elif list_board[4] == list_board[5] == list_board[6] != 0: # across the middle
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
                elif list_board[1] == list_board[2] == list_board[3] != 0: # across the bottom
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
                elif list_board[1] == list_board[4] == list_board[7] != 0: # down the left side
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
                elif list_board[2] == list_board[5] == list_board[8] != 0: # down the middle
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
                elif list_board[3] == list_board[6] == list_board[9] != 0: # down the right side
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break 
                elif list_board[7] == list_board[5] == list_board[3] != 0: # diagonal
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
                elif list_board[1] == list_board[5] == list_board[9] != 0: # diagonal
                    printBoard(list_board)
                    print("\nGame Over.")                
                    print(" **** " +turn + " won. ****")
                    socket.send(bytearray(list_board))
                    break
            else:
             socket.send(bytearray(list_board))  #update the status
             print('sent normal\n')
            # the next if-else is to swictch the turn to another player
            if turn =='X':
             turn = 'O'
             list_board[0]=2
            else:
             turn = 'X' 
             list_board[0]=1
            printBoard(list_board)
        else:# wrong order 
            print('somebody tries to press not his turn: %s'%(message))
            list_board[0]=119 # it is"w", a sign that someone plays when it is not his turn

            socket.send(bytearray(list_board))
        # If neither X nor O wins and the board is full, we'll declare the result as 'tie'.
        if count == 9:
            print("\nGame Over.\n")                
            print("It's a Tie!!")

        # Now we have to change the player after every move.
             
    
    # Now we will ask if player wants to restart the game or not.
    restart = input("Do want to play Again?(y/n)")
    if restart == "y" or restart == "Y":  
        for key in list_board:
            list_board[key] = 0

        game()

if __name__ == "__main__":
    game()

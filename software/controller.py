import socket
import keyboard
import time

##create tcp socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
##address found from the ip seen on phone hotspot
server_address = ('192.168.17.151', 10000)
print('starting up on ', server_address)
sock.bind(server_address)
sock.listen(10)

while True:
    command = b's'
    print('waiting for connection')
    connection, client_address = sock.accept()
    print('connection from', client_address)  
    try:  
        while True:
                ##wasd bind is used for movement command that is sent to the rover
                if keyboard.is_pressed('a'):
                    command = b'l'
                elif keyboard.is_pressed('s'):
                    command = b'b'
                elif keyboard.is_pressed('d'):
                    command = b'r'
                elif keyboard.is_pressed('w'): #foward/backward direction doesn't matter
                    command = b'f'             
                elif keyboard.is_pressed('space'): #stop
                     command=b's'
                elif keyboard.is_pressed('q'): ##quits the program
                     quit()
                print(command)
                connection.send(command)
                time.sleep(0.2)
    finally:
        connection.close()        


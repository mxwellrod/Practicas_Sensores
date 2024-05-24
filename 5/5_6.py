# -*- coding: utf-8 -*-
"""
Created on Wed May  1 23:09:13 2024

@author: Maxwell
"""

import socket

# Set up a TCP/IP server
tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to server address and port 1500
server_address = ('192.168.1.111', 2000)

try:
    # Bind the socket to the server address and port
    tcp_socket.bind(server_address)

    # Listen for incoming connections (allow 1 connection in the queue)
    tcp_socket.listen(1)

    print("Server is listening on {}:{}".format(server_address[0], server_address[1]))

    while True:
        print("Waiting for connection...")
        connection, client_address = tcp_socket.accept()

        try:
            print("Connected to client IP: {}".format(client_address))

            # Receive and print data in chunks of 32 bytes as long as the client sends data
            while True:
                data = connection.recv(32)
                if not data:
                    break
                print("Received data: {}".format(data.decode()))

        finally:
            # Close the connection with the client
            connection.close()

except OSError as e:
    print("Error: {}".format(e))

finally:
    # Close the server socket
    tcp_socket.close()


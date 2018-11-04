Secure Chat is an encrypted peer to peer chat program that uses PGP and AES encryption to secure communications. Each user generates a PGP public and private key. Your PGP key is then used to exchange a new AES key for each chat session when a client connects to your server.

Primary code files:
main.cpp
app.cpp
chat.cpp
Client.cpp
Server.cpp


![Secure Chat](https://i.imgur.com/h4aUQ3u.png)


Dependencies:
cryptlib.lib
Eigen
boost
openGL
WinSock

# Reliable_UDP_Appliaction
•This project has two programs: sever and client.
•The server should listen on a UDP socket, then begin sending chunks of a specific file over UDP when client initiates a download. The client should then save a copy of the file when it is done receiving the chunks.
•Because UDP is connectionless and unreliable, I implement parallel programming to allow multiple clients to download simultaneously. And I use stop and wait protocol to rearranged chunks. Besides, I set timer for each packet to recover from lost.
------------------------------------------------------------------------------------------------------------------------------
I have designed an application that is based on UDP model. But at application layer, I add packet check to ensure the reliability of UDP. 

I use stop and wait protocol to make sure each packet has been sent and received successfully. That is, the next packet should be sent until the server receive the former packet. The server will send back the message to tell client that it receive the former packet. Thus the client will send the next packet.

If there is packet loss, I set the timer(4s) to resend the loss packet.

This application support handling multiple clients on server. I use multiple process to handle the parallel programming. 

Besides, I also check more client can use same port to transfer data.

Usage: There is a test file named: test(503KB),that you can use.

For example:
use make to compile

$make

Server terminal:

$./Server test 11114
Waiting for data...

===>Received requested message successfully!!: Please send me a file

==>Send successfully!!!!!

Waiting for data...

===>Received requested message successfully!!: Please send me a file

==>Send successfully!!!!!

Waiting for data...

===>Received requested message successfully!!: Please send me a file

==>Send successfully!!!!!

Waiting for data...

Client terminal:

$./Client 127.0.0.1 11114 result0

=>Send requested message  successfully

===>Received completely and successfully!!!

$./Client 127.0.0.1 11114 result1

=>Send requested message  successfully

===>Received completely and successfully!!!

$./Client 127.0.0.1 11114 result2

=>Send requested message  successfully

===>Received completely and successfully!!!

$diff result0 result1

$diff result1 result2



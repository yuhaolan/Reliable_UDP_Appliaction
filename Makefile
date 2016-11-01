all :
	g++ -g -o Client Client.cc Functions.cc 
	g++  -g -o Server Server.cc Functions.cc 

clean : 
	rm Server
	rm Client
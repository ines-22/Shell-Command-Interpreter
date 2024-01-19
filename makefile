interactive: shelll
batch: myshell 

shelll: shell.o 
	gcc shell.o -o shelll
	./shelll
		
myshell: shell.o 
	gcc shell.o  -o myshell
	./myshell batchfile.sh
	
shell.o: shell.c 
	gcc -c shell.c
		
cleanI: 
	rm -rf *o shelll

cleanB: 
	rm -rf *o myshell	    
Test:DEMO
			
# Replace the .c and .o file names which you wish to run
 
DEMO:Bc.o
#	gcc -o DEMO BcMt.o ../libApoorva31Rts-4NodeDDCDll.a
	gcc -o DEMO Bc.o ../libApoorva31Rts_4NodeDll.a
##-L. -lPci1553Dll
Bc.o:      Bc.c
	gcc -g -c Bc.c -o Bc.o
clean:
	rm -rf *.o DEMO	
	echo "This is 1553 Single Node Sample Program"




	

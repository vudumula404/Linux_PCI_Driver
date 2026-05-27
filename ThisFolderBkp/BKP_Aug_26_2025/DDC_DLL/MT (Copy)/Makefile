Test:DEMO
			
# Replace the .c and .o file names which you wish to run
 
DEMO:MT.o
#	gcc -o DEMO MT.o ../libApoorva31Rts-4NodeDDCDll.a
	gcc -o DEMO MT.o ../libApoorva31Rts_4NodeDll.a
##-L. -lPci1553Dll
MT.o:      MT.c
	gcc -g -c MT.c -o MT.o
clean:
	rm -rf *.o DEMO	
	echo "This is 1553 Single Node Sample Program"




	

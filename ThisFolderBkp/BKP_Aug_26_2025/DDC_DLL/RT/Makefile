Test:DEMO
			
# Replace the .c and .o file names which you wish to run
 
DEMO:RT.o
#	gcc -o DEMO RT.o ../libApoorva31Rts-4NodeDDCDll.a
	gcc -o DEMO RT.o ../libApoorva31Rts_4NodeDll.a
##-L. -lPci1553Dll
RT.o:      RT.c
	gcc -g -c RT.c -o RT.o
clean:
	rm -rf *.o DEMO	
	echo "This is 1553 Single Node Sample Program"




	

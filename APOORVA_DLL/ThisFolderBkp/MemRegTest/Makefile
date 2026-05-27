Test:DEMO
			
# Replace the .c and .o file names which you wish to run
 
DEMO:MemRegTest.o
#	gcc -o DEMO MemRegTest.o ../libApoorva31Rts-4NodeDDCDll.a
	gcc -o DEMO MemRegTest.o ../libApoorva31Rts_4NodeDll.a
##-L. -lPci1553Dll
MemRegTest.o:      MemRegTest.c
	gcc -g -c MemRegTest.c -o MemRegTest.o
clean:
	rm -rf *.o DEMO	
	echo "This is 1553 Single Node Sample Program"




	

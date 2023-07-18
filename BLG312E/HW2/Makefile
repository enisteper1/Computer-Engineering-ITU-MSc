output: multiprocessing.o multithreading.o

multiprocessing.o: multiprocessing.c
	gcc multiprocessing.c -o multiprocessing -pthread -lrt

multithreading.o: multithreading.c
	gcc multithreading.c -o multithreading -pthread -lrt

clean:
	rm multiprocessing multithreading

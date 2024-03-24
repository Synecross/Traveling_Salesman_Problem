main : site.o lectureFichiers.o main.o haversine.o itineraire.o
	gcc main.o site.o lectureFichiers.o haversine.o itineraire.o -o main -Wall -g -lm

site.o : site.c site.h
	gcc -c site.c

itineraire.o : itineraire.c itineraire.h site.h
	gcc -c itineraire.c

lectureFichiers.o : lectureFichiers.c lectureFichiers.h
	gcc -c lectureFichiers.c

haversine.o : haversine.c haversine.h
	gcc -c haversine.c

main.o : main.c site.h lectureFichiers.h itineraire.h
	gcc -c main.c

clean :
	rm -f *.o main
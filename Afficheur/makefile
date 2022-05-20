all: afficheur SNAfficheur.o SNLigne.o PortSerieAfficheur.o

SNAfficheur.o: SNAfficheur.h SNAfficheur.cpp
    g++ -Wall -c SNAfficheur.cpp

SNLigne.o: SNLigne.h SNLigne.cpp
    g++ -Wall -c SNLigne.cpp

PortSerieAfficheur.o: PortSerieAfficheur.cpp PortSerieAfficheur.h
    g++ -Wall -c PortSerieAfficheur.cpp

main.o: main.cpp
    g++ -Wall -c main.cpp

afficheur: SNAfficheur.o SNLigne.o PortSerieAfficheur.o main.o
    g++ -Wall SNAfficheur.o SNLigne.o PortSerieAfficheur.o main.o -o afficheur

clean:
    rm -f *.o
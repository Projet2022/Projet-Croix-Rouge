all: afficheur SNAfficheur.o SNLigne.o PortSerieAfficheur.o SNServeurTcpMonoClient.o SNClientTCP.o

SNAfficheur.o: SNAfficheur.h SNAfficheur.cpp
	g++ -Wall -c SNAfficheur.cpp

SNLigne.o: SNLigne.h SNLigne.cpp
	g++ -Wall -c SNLigne.cpp

PortSerieAfficheur.o: PortSerieAfficheur.cpp PortSerieAfficheur.h
	g++ -Wall -c PortSerieAfficheur.cpp

SNServeurTcpMonoClient.o: SNServeurTcpMonoClient.cpp SNServeurTcpMonoClient.h
	g++ -Wall -c SNServeurTcpMonoClient.cpp

SNClientTCP.o: SNClientTCP.h SNClientTCP.cpp
	g++ -Wall -c SNClientTCP.cpp

main.o: main.cpp
	g++ -Wall -c main.cpp

afficheur: SNAfficheur.o SNLigne.o PortSerieAfficheur.o main.o SNServeurTcpMonoClient.o SNClientTCP.o
	g++ -Wall SNAfficheur.o SNLigne.o PortSerieAfficheur.o main.o SNServeurTcpMonoClient.o SNClientTCP.o -o afficheur

clean:
	rm -f *.o

#include "SNServeurTcpMonoClient.h"

#ifdef _WIN32
#include <winsock2.h>
#include <conio.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <iostream>
#include <string>


using namespace std;

SNServeurTcpMonoClient::SNServeurTcpMonoClient()
{
}

SNServeurTcpMonoClient::~SNServeurTcpMonoClient()
{
	FermerLaCommunicationAvecLeClient();
	ArreterLeServeur();
}

void SNServeurTcpMonoClient::Initialisation()
{
	#ifdef _WIN32
	WSADATA wsaData ;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0)
	{
		cout << "Echec lors de l'initialisation de la DLL Winsock" << endl;
	}
	#endif
	socketEcoute = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( socketEcoute == -1 )
	{
		cout << "Creation de la socket d'ecoute : ECHEC." << endl;
		return ;
	}
	cout << "Creation de la socket d'ecoute : OK." << endl;

}
bool SNServeurTcpMonoClient::MettreEnEcouteSurLePort(unsigned short portEcoute)
{
	if(socketEcoute == -1)
	{
		return false;
	}
	portEcoute = portEcoute;

	struct sockaddr_in addrMonServeur;
	addrMonServeur.sin_family = AF_INET;
	// Ecoute sur le port 4222
	addrMonServeur.sin_port = htons(portEcoute);
	// Ecoute sur toutes les adresses IP de ma machine
	addrMonServeur.sin_addr.s_addr = INADDR_ANY;
	if ( bind ( socketEcoute, ( struct sockaddr * ) &addrMonServeur, sizeof ( addrMonServeur )  ) < 0 )
	{
		cout << "Association a un port : ECHEC." << endl;
		return false;
	}

	cout << "Association a un port : OK." << endl;

	// Mise en ecoute du serveur
	if (listen(socketEcoute,1) == -1) {
		cout << "Mise en ecoute : ECHEC." << endl;
		return false;
	}
	cout << "Mise en ecoute : OK." << endl;
	return true;
}

bool SNServeurTcpMonoClient::AttendreUnNouveauClient()
{
	if(socketEcoute == -1)
	{
		return false;
	}
	struct sockaddr_in addrClient;
	unsigned int addrClientLen = sizeof(addrClient);
	socketDeCommunication = accept(socketEcoute,(struct sockaddr*)&addrClient, (unsigned int *) &addrClientLen);
	if(socketDeCommunication <= 0)
	{
		cout << "Client accepte : ECHEC." << endl;
		return false;
	}

	cout << "Client accepte : OK." << endl;
	return true;
}

int SNServeurTcpMonoClient::RecevoirUnMessage(char message[], int longueurMaxDuMessage)
{
	int nbOctets = recv(socketDeCommunication, message, longueurMaxDuMessage, 0);
	//string str(message, nbOctets);
	//cout << "Message du client (" << nbOctets << " octets) : " << str << endl;
	return nbOctets;
}

int SNServeurTcpMonoClient::EnvoyerUnMessage(char message[], int longueurDuMessage)
{
	int nbOctets = send(socketDeCommunication, message, longueurDuMessage, 0);
	return nbOctets;

}

void SNServeurTcpMonoClient::FermerLaCommunicationAvecLeClient()
{
	// Fermeture de la connexion avec le client
	#ifdef _WIN32
	closesocket(socketDeCommunication);
	#else
	close(socketDeCommunication);
	#endif
	cout << "Fermeture de la communication avec le client : OK." << endl;
}

void SNServeurTcpMonoClient::ArreterLeServeur()
{
	// Fermeture de la socket d'ecoute
	#ifdef _WIN32
	closesocket(socketEcoute);
	#else
	close(socketEcoute);
	#endif
	cout << "Arret du serveur : OK." << endl;

}


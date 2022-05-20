#include "SNAfficheur.h"

#ifdef __unix__
#include <arpa/inet.h>
#include <unistd.h>
#elif _WIN32 || _WIN64
#include <winsock2.h>
//#pragma comment(lib, "Ws2_32.dll")

#endif

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

int SNAfficheur::nbTrameUDP = 0;

SNAfficheur::SNAfficheur()
{
  m_identifiant = 0;
  m_adresseIPServeurUDP = "172.20.182.10";
  m_portServeurUDP = 4321;
}

SNAfficheur::~SNAfficheur()
{
  // On s'assure que le port série se ferme correctement
  m_portSerie.fermer();
}


bool SNAfficheur::ChangerLIdentifiantDeLAfficheur( unsigned int identifiant)
{
    m_erreur = AUCUNE;
    char _reponse[255];

    stringstream strame;
    strame << "<ID>" << setfill('0') << setw(2) << setiosflags(ios_base::uppercase) << identifiant << "<E>";

    cout << "Trame de configuration : " << strame.str() << endl;

    // Envoi de la trame de configuration
    if( m_portSerie.envoyerTrame(strame.str()) == false)
    {
        m_erreur = ID1;
        return false;	// Echec de l'envoi de la trame de configuration de l'identifiant
    }

    // Lecture de la réponse
    int nb = m_portSerie.lireMessage(_reponse, 255);
#ifdef DEBUG
    cout << "Nb caracteres lus : " << nb << endl;
    cout << "Message recu : " <<  _reponse << endl;
#endif
    if(strlen(_reponse) == 2)
    {
        if(identifiant == (unsigned int)atoi(_reponse))
        {
#ifdef DEBUG
            cout << "Le changement d'identifiant est reussi." << nb << endl;
#endif
            m_identifiant = identifiant;
            return true;
        }
        m_erreur = ID2;
        return false;      // Réponse de bonne longueur mais identifiant reçu incorrect
    }
    m_erreur = ID3;
    return false;          // Réponse de mauvaise longueur.
}


bool SNAfficheur::OuvrirPortSerie(string nomPortSerie)
{
    // Ouverture du port série
    if(m_portSerie.ouvrir(nomPortSerie) != 0)
    {
        cerr << "Erreur lors de l'ouverture du port sÃ©rie" << endl;
        return false;
    }
    return true;
}


bool SNAfficheur::FermerPortSerie()
{
    return m_portSerie.fermer();
}

bool SNAfficheur::ModifierNombrePagesAffichees(int nombrePages)
{
    // On dit Ã  l'afficheur le nombre de page à afficher : 1 page dans cette version
    char _trameConfig[200];
    char debutTrame[] = "<TA>00010100009912302359A";
    char _reponse[10];
    for(int i=1 ; i<=nombrePages ; i++)
    {
        sprintf(_trameConfig,"%s%c", debutTrame, (char)('A'-1 + nombrePages) );
    }
    char _message[300];
    sprintf(_message, "<ID%.2d>%s%.2X<E>", m_identifiant, _trameConfig, calculerChecksum(_trameConfig));
    //char reponse[10];
    //ajouterIdCsE(trameConfig);
#ifdef DEBUG
    cout << "Trame de configuration : " << _message << endl;
#endif
    // Envoi de la trame de configuration
    if( m_portSerie.envoyerTrame(_message) == false)
    {
        return false;
    }

    // Lecture de la rÃ©ponse
    int nb = m_portSerie.lireMessage(_reponse, 255);
#ifdef DEBUG
    cout << "Nb caracteres lus : " << nb << endl;
    cout << "Message recu : " <<  _reponse << endl;
#endif
    if( (strlen(_reponse) == 3) && (strcmp(_reponse,"ACK")==0))
    {
#ifdef DEBUG
        cout << "Configuration du nombre de pages reussie." << endl;
#endif
        return true;
    }
    return false;
}



bool SNAfficheur::EnvoyerLigne(SNLigne ligne)
{
    stringstream strame;
    strame << "<ID" << setfill('0') << setw(2) << setiosflags(ios_base::uppercase) << m_identifiant << ">" << ligne.PaquetDonnees() << setfill('0') << setw(2) << setiosflags(ios_base::uppercase) << hex << (unsigned short)calculerChecksum(ligne.PaquetDonnees()) << "<E>";
    return EnvoyerTrame(strame.str());
}


bool SNAfficheur::EnvoyerTrame(string trame)
{
    // Envoi de la trame sur le port série
    if( m_portSerie.envoyerTrame(trame.c_str()) == false)
    {
        return false;
    }
#ifdef DEBUG
    cout << "Trame envoyée : " << trame << endl;
#endif
    // Lecture de la réponse si ID != 0
    /*if(m_identifiant != 0)
    {
        m_portSerie.lireMessage(_reponse, 10);
        // Si la réponse de l'afficheur n'est pas ACK
        if(strcmp(_reponse,"ACK") != 0)
        {
            return true;
        }
    }*/
    return true;
}


bool SNAfficheur::EnvoyerTrame(const char * trame, int longueurTrame)
{
    string str(trame,longueurTrame);
    return EnvoyerTrame(str);
}



bool SNAfficheur::EnvoyerTrameEnUDP(string trame)
{
    if(nbTrameUDP == 0)
    {
		#if defined(_WIN32) || defined(_WIN64)
		WSADATA wsaData ;
		int iResult;
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if(iResult != 0)
		{
			cout << "Echec lors de l'initialisation de la DLL Winsock" << endl;
		}
		#endif
    }
	nbTrameUDP++;
	int socketClientUDP;


    socketClientUDP = socket ( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketClientUDP == -1 )
    {
        cout << "Erreur de création de socket." << endl;
        return false;
    }

    struct sockaddr_in addrServeur;
    addrServeur.sin_family = AF_INET;
    addrServeur.sin_port = htons ( m_portServeurUDP );
    addrServeur.sin_addr.s_addr = inet_addr(m_adresseIPServeurUDP.c_str());


    int nbCaractEmi;

    nbCaractEmi = sendto ( socketClientUDP, trame.c_str(), trame.length()+1 , 0, (struct sockaddr *) &addrServeur, sizeof(addrServeur));
    // On vérifie qu'un message a bien été reçu
    if(nbCaractEmi == -1)
    {
        return false;
    }

    #ifdef __unix__
    close(socketClientUDP);
    #elif defined(_WIN32) || defined(_WIN64)
    closesocket(socketClientUDP);
    #endif

    return true;
}


bool SNAfficheur::EnvoyerTrameEnUDP(const char * trame, int longueurTrame)
{
    string str(trame,longueurTrame);
    return EnvoyerTrameEnUDP(str);
}


bool SNAfficheur::EnvoyerLigneEnUDP(SNLigne ligne)
{
    stringstream strame;
    strame << "<ID" << setfill('0') << setw(2) << setiosflags(ios_base::uppercase) << m_identifiant << ">" << ligne.PaquetDonnees() << setfill('0') << setw(2) << setiosflags(ios_base::uppercase) << hex << (unsigned short)calculerChecksum(ligne.PaquetDonnees()) << "<E>";

    cout << strame.str() << endl;
	//return true;
	return EnvoyerTrameEnUDP(strame.str());
}



/*unsigned char SNAfficheur::calculerChecksum(const char * message) const
{
    // Calcul du CS
    unsigned char checksum = 0;
    for (unsigned int i=0 ; i < strlen(message) ; i++)
    {
        checksum = checksum ^ message[i];
    }
    return checksum;
}*/

unsigned char SNAfficheur::calculerChecksum(string message) const
{
    // Calcul du CS
    unsigned char checksum = 0;
    for (unsigned int i=0 ; i < message.length() ; i++)
    {
        checksum = checksum ^ message[i];
    }
    return checksum;
}



string SNAfficheur::DescriptionErreur() const
{
    string descriptionErreur;
    switch(m_erreur)
    {
                case AUCUNE : descriptionErreur = "Aucune erreur.";
                    break;
                case ID1 : descriptionErreur = "Impossible d'envoyer la trame de changement d'identifiant.";
                    break;
                case ID2 : descriptionErreur = "L'identifiant envoyé dans le message de confirmation est incorrect.";
                    break;
                case ID3 : descriptionErreur = "Le message de confirmation de changement d'identifiant n'est pas de la bonne longueur (2 caractères attendus).";
                    break;
                }
    return descriptionErreur;
}

#include "PortSerieAfficheur.h"

#ifdef __unix__
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#elif _WIN32 || _WIN64

#endif

#include <iostream>
using namespace std;

/********************************************************************************
*	SOUS-PROGRAMME PortSerieAfficheur DE LA CLASSE PortSerieAfficheur	*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE :						*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Le constructeur fourni des valeurs par défaut au attributs de la 	*
* 	classe. Par défaut, le port est fermé, le descripteur de fichier vaut 	*
* 	-1 et le nom du port est une chaine de caractère vide.			*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
********************************************************************************/
PortSerieAfficheur::PortSerieAfficheur()
{
    // Valeur par défaut des attributs
    m_estOuvert = false;
    m_nomDuPortSerie =  "";
    m_descripteurFichier = -1;
}


/********************************************************************************
*	SOUS-PROGRAMME ~PortSerieAfficheur DE LA CLASSE PortSerieAfficheur	*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE :						*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Le destructeur doit vérifier que le port série est bien fermé avant que	*
* 	l'objet soit détruit. S'il n'est pas fermé, il le ferme.		*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
********************************************************************************/
PortSerieAfficheur::~PortSerieAfficheur()
{
    if(m_estOuvert)
        this->fermer();
}

/********************************************************************************
*	SOUS-PROGRAMME ouvrir DE LA CLASSE PortSerieAfficheur			*
*		DONNEES EN ENTREE : nomDuPortSerie[] : CARACTERE		*
* 		DONNEES EN SORTIE : erreur : ENTIER				*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Cette méthode doit ouvrir le port série dont le nom est passé en 	*
* 	argument : 'nomDuPortSerie'. Si cet objet a déjà ouvert un port série	*
* 	ou que l'ouverture du port série spécifié est impossible (port ouvert	*
* 	par quelqu'un d'autre, droits insuffisants...) cette méthode retourne	*
* 	une valeur négative. Sinon, elle renvoie 0.				*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
*	Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.	*
********************************************************************************/
int PortSerieAfficheur::ouvrir(string nomDuPortSerie)
{  
    // Copie du nom dans l'attribut m_nomDuPortSerie si le nom n'est pas trop long
    if(nomDuPortSerie.length() > 50) return -1;
    m_nomDuPortSerie = nomDuPortSerie;

    // On vérifie qu'un port de comunication série n'est pas déjà ouvert...
    if(m_estOuvert) return -2;

	// On ouvre le port de communication série demandé
	#ifdef __unix__
	//m_descripteurFichier = open(nomDuPortSerie, O_RDWR | O_NOCTTY | O_NDELAY);
	m_descripteurFichier = open(nomDuPortSerie.c_str(), O_RDWR | O_NDELAY);
	//m_descripteurFichier = open(nomDuPortSerie, O_RDWR );
	#elif _WIN32 || _WIN64

	#endif

	if (m_descripteurFichier == -1) return -3;
	m_estOuvert = true;

	#ifdef __unix__
	// Déclaration d'une structure contenant les paramètres du port série
    struct termios options;
    // On récupère les paramètres actuels du port série
    tcgetattr(m_descripteurFichier, &options);
    // On impose une vitesse à 9600 baud en lecture et en écriture
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    // On impose 8N1  : 8 bits de données, pas de bit de parité, 1 bit stop.
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    // On modifie les paramètres du port série avec nos nouvelles valeurs
	tcsetattr(m_descripteurFichier, TCSANOW, &options);
	// La lecture sera bloquante
	fcntl(m_descripteurFichier, F_SETFL, 0);	// Lecture bloquante
	#elif _WIN32 || _WIN64

	#endif
    return 0;
}


/********************************************************************************
*	SOUS-PROGRAMME fermer DE LA CLASSE PortSerieAfficheur			*
*		DONNEES EN ENTREE :						*
* 		DONNEES EN SORTIE : erreur : ENTIER				*
* 		DONNEES LOCALES : 						*
********************************************************************************* 
* 	DESCRIPTION :								*
* 	Cette méthode doit fermer le port série précédemment ouvert. Si le port *
* 	série n'était pas ouvert ou que la fermeture du port série provoque une *
* 	erreur, cette méthode renvoie une valeur négative. Sinon, elle renvoie 	*
* 	0.									*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie 0 en cas de succès, une valeur négative sinon.	*
********************************************************************************/
int PortSerieAfficheur::fermer()
{
    // Si le port série n'a pas été ouvert, on renvoie -1
    if(m_descripteurFichier == -1)
        return -1;
    // Si la fermeture du port série échoue, on renvoie -2
    #ifdef __unix__
    if(close(m_descripteurFichier) == -1)
        return -2;
    #elif _WIN32 || _WIN64

	#endif
    // Sinon, on indique que la fermeture s'est bien passé en mettant le booléen m_estouvert à false et en renvoyant la valeur 0
    m_estOuvert = false;
    return 0;
}


/********************************************************************************
*	SOUS-PROGRAMME envoyerCaractere DE LA CLASSE PortSerieAfficheur		*
*		DONNEES EN ENTREE : c : CARACTERE       			*
* 		DONNEES EN SORTIE : resultat : BOOLEEN				*
* 		DONNEES LOCALES : 						*
*********************************************************************************
* 	DESCRIPTION :								*
* 	Cette méthode doit envoyer sur le port série le caractère 'c' passé en  *
*       argument. Le port série doit être préalalement ouvert, sinon 'resultat' *
*       vaut 'FAUX'.                                                            *
* 	Si le nombre d'octet réellement envoyé est différent de 1, 'resultat'   *
*       vaut 'FAUX'.                						*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie FAUX en cas d'erreur, VRAI sinon.                 *
********************************************************************************/
bool PortSerieAfficheur::envoyerCaractere(const char c) const
{
    // Si le port série n'est pas ouvert, renvoie de 'FAUX'
    if (!m_estOuvert)
        return false;
    // Écriture du caractère 'c' sur le port série. On récupère le nombre d'octet vraiment écrit dans nbOctet.
    int nbOctet;
    #ifdef __unix__
    nbOctet = write(m_descripteurFichier, &c, 1);
    #elif _WIN32 || _WIN64

	#endif
    // Si nombre d'octet vraiment écrit est différent de 1, renvoie de la valeur 'FAUX', 'VRAI sinon.
    if(nbOctet != 1)
        return false;
    return true;
}


/********************************************************************************
*	SOUS-PROGRAMME envoyerTrame DE LA CLASSE PortSerieAfficheur		*
*		DONNEES EN ENTREE : trame[] : CARACTERE       			*
* 		DONNEES EN SORTIE : resultat : BOOLEEN				*
* 		DONNEES LOCALES : 						*
*********************************************************************************
* 	DESCRIPTION :								*
* 	Cette méthode doit envoyer sur le port série la chaîne de caractères    *
*       'trame' passé en argument. Le port série doit être préalalement ouvert  *
*       sinon 'resultat' vaut 'FAUX'.                                           *
* 	Si le nombre d'octet réellement envoyé est différent de la longueur de  *
*       la trame à envoyer, 'resultat' vaut 'FAUX'.                             *                						*
*********************************************************************************
*	VALEUR DE RETOUR : 							*
* 	Cette méthode renvoie FAUX en cas d'erreur, VRAI sinon.                 *
********************************************************************************/
bool PortSerieAfficheur::envoyerTrame(const char trame[]) const
{
	int tailleDeLaTrame = strlen(trame);
	int nbOctet;
	
	if (!m_estOuvert)
		return false;
	#ifdef __unix__
	nbOctet = write(m_descripteurFichier, trame, tailleDeLaTrame);
	usleep(100000);
	#elif _WIN32 || _WIN64
	
	#endif
	
	if(nbOctet != tailleDeLaTrame)
		return false;

	return true;
}


// Surcharge de la méthode précédente
bool PortSerieAfficheur::envoyerTrame(string trame) const
{
	int tailleDeLaTrame = trame.length();
	int nbOctet;
	
	if (!m_estOuvert)
		return false;
	#ifdef __unix__
	nbOctet = write(m_descripteurFichier, trame.c_str(), tailleDeLaTrame);
	usleep(100000);
	#elif _WIN32 || _WIN64
	
	#endif

	if(nbOctet != tailleDeLaTrame)
		return false;

	return true;}

int PortSerieAfficheur::lireMessage(char message[], int tailleMaximaleDuMessage) const
{
	int nbOctet=0;
	cout << "Attente d'un message" << endl;
	#ifdef __unix__
	nbOctet = read(m_descripteurFichier, message, tailleMaximaleDuMessage-1);
	#elif _WIN32 || _WIN64
	
	#endif

	cout << "Message lu" << endl;
	// Rajout du caractère \0 si nécessaire
	if(message[nbOctet-1] != '\0')
		message[nbOctet] = '\0';
	return nbOctet;
}

string PortSerieAfficheur::lireMessage() const
{
	char msg[1024];
	int nbOctet=0;
	cout << "Attente d'un message" << endl;
	#ifdef __unix__
	nbOctet = read(m_descripteurFichier, msg, 1023);
	#elif _WIN32 || _WIN64
	
	#endif

	cout << "Message lu" << endl;
	// Rajout du caractère \0 si nécessaire
	if(msg[nbOctet-1] != '\0')
		msg[nbOctet] = '\0';
	//return nbOctet;
	return string(msg);
}

/*int PortSerieAfficheur::lireMessageMieux(char message[], int tailleMaximaleDuMessage) const
{
    int nbOctetsLus=0;
    int res;
    int tmp = 0;
    do{
      res = read(m_descripteurFichier, message+nbOctetsLus, 1);
      //cout << "Lire - res : " << res << " - nbOctetsLus : " << nbOctetsLus << endl;
      if(res > 0 ) nbOctetsLus += res;
      tmp ++;
      usleep(10);
    } while(tmp<8000);
    // Rajout du caractère \0 si nécessaire
    if(message[nbOctetsLus-1] != '\0')
        message[nbOctetsLus] = '\0';
    return nbOctetsLus;
}


int PortSerieAfficheur::lireMessageACK(char message[], int tailleMaximaleDuMessage) const
{
    int nbOctetsLus=0;
    int nbOctetsALire = 3;
    do
    {
      nbOctetsLus += read(m_descripteurFichier, message + nbOctetsLus, 1);
      if((nbOctetsLus == 1) && (message[0] == 'N'))
      {
	nbOctetsALire = 4;
      }
    }while (nbOctetsLus < nbOctetsALire );
    // Rajout du caractère \0 si nécessaire
    if(message[nbOctetsLus-1] != '\0')
        message[nbOctetsLus] = '\0';
    return nbOctetsLus;
}*/


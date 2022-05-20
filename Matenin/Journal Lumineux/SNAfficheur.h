/*! \mainpage Documentation de l'API Afficheur Météo
 *
 * \section intro_sec Introduction
 *
 * L'API (application programming interface) Afficheur Météo fournie des classes facilitant le développement d'application utilisant un journal lumineux dont voici les références : <a href="http://www.conrad.fr/ce/fr/product/590998/Journal-lumineux-LED-rouge">journal lumineux LED rouge</a> et <a href="http://www.conrad.fr/ce/fr/product/590996/Journal-lumineux-LED-3-couleurs">journal lumineux LED 3 couleurs</a>. <br />
 * Dans notre cas, nous utiliserons l'afficheur monochrome. Voici une photo de cette afficheur :
 *
 * \image html JournalLumineux-BTSSN2014-mini.png
 *
 * Dans la suite de cette page de présentation, vous trouverez de l'aide pour installer le matériel et l'utiliser aussi bien pour Windows que pour Linux.
 *
 * Ensuite, vous trouverez un diagramme de classe de l'API afficheur météo.
 *
 * \section install_sec Installation
 *
 * \subsection install_subsec Pour Windows
 * Vous trouverez la documentation d'installation dans <a href="../documentsConstructeur/590998-an-01-ml-LED_LAUFSCHRIFT_ROT_de_en_fr_nl.pdf">ce document officiel</a> à partir de la page 55.
 * \subsection install_subsec Pour Linux
 * La procédure d'installation ci-dessous s'utilise lors de la première utilisation du journal lumineux. Il faut alors indiqué à l'OS Linux que le journal lumineux doit être vu comme un port série virtuel. Voici la procédure à suivre :
 * \li Récupérer le numéro du vendeur et le numéro du produit du journal lumineux par la commande :
 * \code
 * lsusb
 * \endcode
 * \li Indiquer qu'il s'agit d'un matériel utilisant un port série virtuel, en indiquant les numéros de vendeur et de produit précédemment trouvés :
 * \code
 * sudo modprobe usbserial vendor=0x.... product=0x....
 * \endcode
 * \li Trouver le nom du port série associé au journal lumineux par la commande suivante :
 * \code
 * dmesg | grep usb
 * \endcode
 *
 * Le port série généralement attribué est <i>/dev/ttyUSB0</i>.
 *
 * \section port_sec Quel port série ?
 * Une fois l'installation faîte, il est nécessaire de connaître le port série virtuel utilsé pour l'afficheur. La procédure est différente suivant les OS.
 * \subsection portWindows_subsec Pour Windows
 * Pour connaître le port série de l'afficheur, ouvrez le gestionnaire de périphérique. Allez ensuite dans la partie 'Ports Séries / Port COM'. Vous trouverez ensuite le nom du port série associé à votre matériel (cp210x).
 * \subsection portLinux_subsec Pour Linux
 * Pour trouver le nom du port série associé au journal lumineux, tapez la commande suivante :
 * \code
 * dmesg | grep usb
 * \endcode
 * Vous trouverez alors quel port série virtuel est utilisé pour votre matériel (cp210x). Généralement, le port série attribué est /dev/ttyUSB0.
 *
 * \section UML Diagramme de classe de l'API
 * L'API afficheur météo est constituée de 4 classes : PortSerieAfficheur, Afficheur, Ligne et DateHeure. La classe Afficheur est une classe outil gérant la communication avec l'afficheur. Voici un diagramme de classe :
 *
 * A faire : image du diagramme de classe
 *
 * Vous trouverez une description détaillée de chacune de ces classes dans l'onglet Classes de cette page.
 *
 */


/*!
 * \file Afficheur.h
 * \brief Paramétrage et envoi de données d'un journal lumineux
 * \author Gaëtan GEORGES
 * \version 1.3
 */

#ifndef SNAFFICHEUR_H
#define SNAFFICHEUR_H

#include "SNLigne.h"
#include "PortSerieAfficheur.h"
#include <string>

using namespace std;

#define DEBUG


//! La classe Afficheur est une classe outil de gestion d'un afficheur à LED appeler plus  communément un journal lumineux. Cette classe outil gère l'envoi de données vers l'afficheur ainsi que la réception des données au besoin. La communication avec l'afficheur s'effectue sur un port série virtuel.
/*!
Abordons maintenant l'utilisation de la classe Afficheur. Commençons avant toute chose par créer un objet de la classe Afficheur. Nous le nommerons 'aff'. Voici la création de cet objet en C++ :
\code
Afficheur aff;
\endcode
Une fois l'objet 'aff' créé, la première chose à faire est de définir le port série virtuel utilisé pour communiquer avec l'afficheur. Lorsqu'il est connecté à l'ordinateur par la câble USB, un afficheur est vu comme un port série virtuel. Ce port série virtuel possède un nom (COM1, COM2, etc. sous windows ; /dev/ttyUSB0, etc. pour linux). Ce nom est à préciser à l'aide de la méthode ouvrirPortSerie().
\code
aff.ouvrirPortSerie("/dev/ttyUSB0");
\endcode
Un afficheur est également caractérisé par un identifiant (voir les méthodes setIdentifiant() et getIdentifiant()). La valeur 0 indique que les messages seront envoyés en broadcast (diffusion) à tous les afficheurs présents. Supposons que nous communiquons avec l'afficheur dont l'identifiant est 1, nous écrirons alors le code suivant :
\code
aff.setIdentifiant(1);
\endcode
Une fois correctement paramétrée (numéro d'identifiant et nom du port série), il est possible d'envoyer une trame de données à l'afficheur. Cette trame doit suivre le protocole établi par le constructeur. Par exemple :
\code
aff.envoyerTrame("<ID01><L1><PA><FE><MA><WC><FE>Hello36<E>");
\endcode
La trame précédente affichera "Hello" sur la première ligne de la première page de l'afficheur. Le texte arrivera de la droite vers la gauche, il restera affiché 2 secondes puis sortira de la droite vers la gauche. Voir les méthodes envoyerTrame() et envoyerLigne() à ce sujet. Pour plus d'information concernant le protocole de communication avec l'afficheur, consulter la <a href="../documentsConstructeur/590996-da-01-en-Communication_protocol_LED_Displ_Board.pdf">documentation constructeur</a>.
Le message à afficher peut être décrit par un objet de la classe Ligne (voir la description de cette classe pour plus d'information).
*/

class SNAfficheur
{
public:
    //! Un constructeur.
    /*!
      Le constructeur sans argument. Il initialise les attributs avec des valeurs par défaut. L'identifiant prend alors la valeur 0, c'est-à-dire en mode de diffusion (broadcast).
      \sa setIdentifiant(), getIdentifiant(), ~Afficheur()
    */
    SNAfficheur();
    //! Le destructeur.
    /*!
      Le destructeur. Il ferme le port série virtuel.
      \sa Afficheur()
    */
    ~SNAfficheur();

	/* Méthodes d'accès de l'attribut 'm_identifiant' */
    //! Méthode d'accès permettant de récupérer la valeur de l'identifiant de l'afficheur.
    /*!
      \return Cette méthode renvoie l'identifiant de l'afficheur.
      \sa ModifierIdentifiant(unsigned int identifiant)
    */
	inline unsigned int Identifiant() const { return m_identifiant;}

    //! Méthode d'accès permettant d'indiquer l'identifiant de l'afficheur que vous utilisez.
    /*!
      \param identifiant l'identifiant de l'afficheur que vous utilisez.
      \sa Identifiant() changerIdentifiant()
    */
	inline void ModifierIdentifiant( unsigned int identifiant) {m_identifiant = identifiant;}

	//! Méthode d'accès permettant de changer l'identifiant de l'afficheur.
    /*!
      \param identifiant le nouvel identifiant de l'afficheur.
      \return Cette méthode renvoie true si le changement d'identifiant s'est correctement passé, false sinon.
      \sa Identifiant() ModifierIdentifiant()
    */
	bool ChangerLIdentifiantDeLAfficheur( unsigned int identifiant);


	//! Méthode permettant d'ouvrir la communication avec l'afficheur sur le port série virtuel.
	/*!
	  \param nomPortSerie le nom du port série virtuel à ouvrir.
	  \return Cette méthode renvoie 0 si le port de communication est correctement ouvert, une valeur négative sinon.
	  \sa FermerPortSerie()
	*/
	bool OuvrirPortSerie(string nomPortSerie);

	//! Méthode permettant de fermer la communication avec l'afficheur sur le port série virtuel.
	/*!
	  \return Cette méthode renvoie true si le port de communication est correctement fermé, false sinon.
	  \sa ouvrirPortSerie()
	*/
	bool FermerPortSerie();

	//! Méthode d'accès permettant de modifier le nombre de pages à afficher.
	/*!
	  \param nombrePages le nombre de page à afficher.
	  \return Cette méthode renvoie 0 si le nombre de pages a été correctement modifié, une valeur négative sinon.
	  \sa UuvrirPortSerie(const char * nomPortSerie)
	*/
	bool ModifierNombrePagesAffichees(int nombrePages);

	//! Méthode d'accès permettant de modifier l'adresse IP du serveur UDP.
	/*!
	  \param adresseIPServeurUDP l'adresse IP du serveur UDP.
	  \sa ModifierPortDuServeurUDP(unsigned short portServeurUDP), envoyerLigneEnUDP(Ligne l)
	*/
	void ModifierAdresseIPDuServeurUDP(string adresseIPServeurUDP) {m_adresseIPServeurUDP = adresseIPServeurUDP;}

	void ModifierPortDuServeurUDP(unsigned short portServeurUDP) {m_portServeurUDP = portServeurUDP;}


	/* Méthode : configurer (nomPortSerie : const char *, identifiant : unsigned char) : void */
	//bool configurer (const char * nomPortSerie, unsigned int identifiant = 0);

	//! Méthode permettant d'envoyer une ligne à l'afficheur.
	/*!
	  \param trame la trame à afficher. Cette trame est une chaîne de caractères dont le format est : "<LX><PX><FX><MX><WX><FX>LeMessageCS". Consulter la documentation constructeur pour de plus amples explications.
	  \return Cette méthode renvoie 0 si le message a été correctement envoyé, une valeur négative sinon.
	  \sa EnvoyerLigne(const Ligne ligne), ModifierAdresseIPDuServeurUDP(), et ModifierPortDuServeurUDP()
	*/
	bool EnvoyerTrame(const char * trame, int longueurTrame);
	bool EnvoyerTrame(string trame);

		//! Méthode permettant d'envoyer une ligne à l'afficheur.
	/*!
	  \param ligne la ligne à afficher. Voir la classe Ligne à ce sujet.
	  \return Cette méthode renvoie 0 si le message a été correctement envoyé, une valeur négative sinon.
	  \sa envoyerTrame()
	*/
	bool EnvoyerLigne(SNLigne ligne);


	//! Méthode permettant d'envoyer une ligne à un serveur UDP.
	/*!
	  \param trame la trame à envoyer. Cette trame est une chaîne de caractères dont le format est : "<LX><PX><FX><MX><WX><FX>LeMessageCS". Consulter la documentation constructeur pour de plus amples explications.
	  \return Cette méthode renvoie true si le message a été correctement envoyé, false sinon.
	  \sa EnvoyerLigneEnUDP(const Ligne ligne)
	*/
	bool EnvoyerTrameEnUDP(const char * trame, int longueurTrame);
	bool EnvoyerTrameEnUDP(string trame);

	//! Méthode permettant d'envoyer une ligne au serveur UDP.
	/*!
	  \param ligne la ligne à envoyer. Voir la classe Ligne à ce sujet.
	  \return Cette méthode renvoie true si le message a été correctement envoyé, false sinon.
	  \sa EnvoyerTrameEnUDP()
	*/
	bool EnvoyerLigneEnUDP(SNLigne ligne);


	//! Méthode permettant de mettre à jour la date et l'heure de l'afficheur avec celles de l'ordinateur.
	/*!
	  \return Cette méthode renvoie 0 si la mise à jour de la date et de l'heure s'est correctement passée, une valeur négative sinon.
	*/
	// bool actualiserDateHeure();

	string DescriptionErreur() const;

private:
	typedef enum {AUCUNE, ID1, ID2, ID3 }Erreur;
	static int nbTrameUDP;
	PortSerieAfficheur m_portSerie;			/*!< Le port série pour la communnication RS232*/
	unsigned int m_identifiant;	/*!< L'identifiant de l'afficheur (son numéro) : la valeur 0 est utilisée pour la diffusion (broadcast)*/
	Erreur m_erreur;
	//unsigned char calculerChecksum(const char * message) const; /*!< Méthode renvoyant le checksum d'un message passé en argument.*/
	unsigned char calculerChecksum(string message) const;
	string m_adresseIPServeurUDP;
	unsigned short m_portServeurUDP;
};

#endif

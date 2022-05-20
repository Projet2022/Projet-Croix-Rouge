/*
Pour se connecter à la raspberry en SSH en ligne de commande :
	ssh pi@172.20.21.191
*/


#include <iostream>
#include "SNAfficheur.h"
#include "SNLigne.h"
using namespace std;


int main(int argc, char * argv[])
{
	cout << "Bonjour" << endl; // Affiche "bonjour" dans le terminal

	SNAfficheur aff;// Création d'un objet 'aff' de la classe SNAfficheur

	aff.OuvrirPortSerie("/dev/ttyUSB0");// Appel de la méthode OuvrirPortSerie() avec aff pour ouvrir le port série

	SNLigne ligne;
	ligne.ModifierMessage("joel 10 pompe");

	aff.EnvoyerLigne(ligne);// Appel de la méthode EnvoyerTrame() avec aff pour envoyer une trame

	aff.FermerPortSerie();// Fermer le port série avec la méthode FermerPortSerie()

	return 0;
}

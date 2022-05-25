#include <iostream>
#include "SNAfficheur.h"
#include "SNLigne.h"
#include "SNClientTCP.h"
using namespace std;


int main(int argc, char * argv[])
{
    SNAfficheur aff;// Création d'un objet 'aff' de la classe SNAfficheur

    aff.OuvrirPortSerie("/dev/ttyUSB0");// Appel de la méthode OuvrirPortSerie() avec aff pour ouvrir le port série

    SNLigne ligne;
    ligne.ModifierMessage("Projet"); // Envoi d'un message sur le journal lumineux

    aff.EnvoyerLigne(ligne);// Appel de la méthode EnvoyerTrame() avec aff pour envoyer une trame

    aff.FermerPortSerie();// Fermer le port série avec la méthode FermerPortSerie()

    // SNClientTCP

    SNClientTCP monClient;

    char reponseHTTP[500];

  //  while(true) {
        monClient.SeConnecterAUnServeur("172.20.21.195",80);
        char requeteHTTP[] = "GET Matenin//APIRest/rest.php HTTP/1.1\r\nHost: 172.20.21.195\r\nConnection: keep-alive\r\n\r\n";
        monClient.Envoyer(requeteHTTP, strlen(requeteHTTP));
        monClient.Recevoir(reponseHTTP,500);
    
    cout<<reponseHTTP<<endl;
//     //}

    return 0;
}
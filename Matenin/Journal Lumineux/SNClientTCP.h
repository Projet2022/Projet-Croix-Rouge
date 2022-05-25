#ifndef SNCLIENTTCP_H
#define SNCLIENTTCP_H
#include <string>

using namespace std;

class SNClientTCP
{

public:
    SNClientTCP();
    bool SeConnecterAUnServeur(string adresseIPServeur, unsigned short portServeur);
    bool SeDeconnecter();
    bool Envoyer(const char * requete,int longueur);
    int Recevoir(char * reponse, int tailleMax);



private :
    static int nbClientTCP;
    int m_maSocket;
    string m_adresseIPServeur;
    unsigned short m_portServeur;

};

#endif // CLIENTHTTP_H

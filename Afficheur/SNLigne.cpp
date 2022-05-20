#include "SNLigne.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace std;


SNLigne::SNLigne( )
{
    //strcpy(m_message, "Hello");
    m_message = "Hello";
    m_numeroLigne = 1;
    m_numeroPage = 'A';
    m_effetChargement = LEADING_SCROLL_LEFT;
    m_effetAffichage = NORMAL;
    m_temporisationAffichage = W_2_SEC;
    m_effetSortie = LAGGING_SCROLL_LEFT;

    PreparerPaquetDonnees();
//    sprintf(m_paquetDonnees, "<L%d><P%c><F%c><M%c><W%c><F%c>%s", m_numeroLigne, m_numeroPage, 'A' + m_effetChargement, 'A' + m_effetAffichage, 'A' + m_temporisationAffichage, 'A' + m_effetSortie, m_message);
}


SNLigne::SNLigne( const char * message, unsigned int longueurMessage )
{
    // A faire : tester que la longueur de message soit inféreure ou égale à celle de m_message
    m_message = string(message, longueurMessage);
    m_numeroLigne = 1;
    m_numeroPage = 'A';
    m_effetChargement = LEADING_SCROLL_LEFT;
    m_effetAffichage = NORMAL;
    m_temporisationAffichage = W_2_SEC;
    m_effetSortie = LAGGING_SCROLL_LEFT;

    PreparerPaquetDonnees();
    //sprintf(m_paquetDonnees, "<L%d><P%c><F%c><M%c><W%c><F%c>%s", m_numeroLigne, m_numeroPage, 'A' + m_effetChargement, 'A' + m_effetAffichage, 'A' + m_temporisationAffichage, 'A' + m_effetSortie, m_message);
}

SNLigne::SNLigne( string message)
{
    m_message = message;
    m_numeroLigne = 1;
    m_numeroPage = 'A';
    m_effetChargement = LEADING_SCROLL_LEFT;
    m_effetAffichage = NORMAL;
    m_temporisationAffichage = W_2_SEC;
    m_effetSortie = LAGGING_SCROLL_LEFT;

    PreparerPaquetDonnees();
//    sprintf(m_paquetDonnees, "<L%d><P%c><F%c><M%c><W%c><F%c>%s", m_numeroLigne, m_numeroPage, 'A' + m_effetChargement, 'A' + m_effetAffichage, 'A' + m_temporisationAffichage, 'A' + m_effetSortie, m_message);
}

SNLigne::~SNLigne()
{
}

void SNLigne::PreparerPaquetDonnees()
{
    stringstream ss;
    ss << "<L" << m_numeroLigne << "><P" << m_numeroPage << "><F" << (char)('A' + m_effetChargement) << "><M" << (char)('A' + m_effetAffichage) << "><W" << (char)('A' + m_temporisationAffichage) << "><F" << (char)('A' + m_effetSortie) <<">" << m_message ;
    m_paquetDonnees = ss.str();
}


/*void Ligne::ajouterChecksum()
{
	char cs = 0x00;
	int lg = strlen(m_paquetDonnees);
	for(int i=0 ; i<lg ; i++)
	{
		cs = cs ^ m_paquetDonnees[i];
	}
	sprintf(m_paquetDonnees,"%s%.2X",m_paquetDonnees, cs);
}*/

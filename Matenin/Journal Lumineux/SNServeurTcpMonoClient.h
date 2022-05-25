#ifndef SNSERVEURTCPMONOCLIENT_H
#define SNSERVEURTCPMONOCLIENT_H

class SNServeurTcpMonoClient
{
private:
	int socketEcoute;
	unsigned short portEcoute;
	int socketDeCommunication; // la socket permettant de communiquer avec le 'mono' client connecté

public:
	SNServeurTcpMonoClient();
	~SNServeurTcpMonoClient();
	void Initialisation();
	bool MettreEnEcouteSurLePort(unsigned short portEcoute);
	bool AttendreUnNouveauClient();
	int EnvoyerUnMessage(char message[], int longueurDuMessage);
	int RecevoirUnMessage(char message[], int longueurMaxDuMessage);
	void FermerLaCommunicationAvecLeClient();
	void ArreterLeServeur();


};

#endif // SNSERVEURTCPMONOCLIENT_H

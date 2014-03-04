/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : Mere
    copyright            : (C) Mere par Mere
    e-mail               : Mere
*************************************************************************/

//---------- Réalisation de la tâche <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <iostream>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Clavier.h"
#include "Entree.h"
#include "/public/tp/tp-multitache/Outils.h"
#include "/public/tp/tp-multitache/Heure.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main (void)
//Algorithme :
{
	//Declaration des differents pids des processus déclarés
	pid_t noClavier;
	pid_t noHeure;
	pid_t noEntreeUn;
	pid_t noEntreeDeux;
	pid_t noEntreeTrois;


	//Mise en place du canal de communication entre la mere et le clavier
	//Creation des differents canaux de communication nommes
	if(mkfifo(canalProfBP,DROITSCANAL) == -1){
		cerr<< "erreur creation du canal entre entree et clavier" << endl;
		return -1 ;
	}




	InitialiserApplication(TERMINALUTILISE);


	noHeure = ActiverHeure();


	if( (noClavier = fork()) == 0)
	{
		/*Code du fils */
		Clavier();
	}else if((noEntreeUn = fork()) ==0){
		/*Code du fils */
		Entree(PROF_BLAISE_PASCAL);

	}else{
		/*Code du pere */



		//On attend la reception de Q du clavier
		//Synchro avec le clavier
		waitpid(noClavier, NULL, 0);


		//Demande de fin avec envoi de SIGUSR2
		kill(noHeure, SIGUSR2);
		kill(noEntreeUn,SIGUSR2);


		//attente de fin des processus fils
		waitpid(noHeure, NULL,0);
		waitpid(noEntreeUn,NULL,0);


		//fermeture des canaux de communication
		unlink(canalProfBP);


		TerminerApplication();
		exit(0);

	}


	return 0;
}

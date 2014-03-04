/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                : Sortie
    copyright            : (C) Sortie par Sortie
    e-mail               : Sortie
*************************************************************************/

//---------- Réalisation de la tâche <Sortie> (fichier Sortie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <signal.h>
//exit
#include <stdlib.h>
//pipe
#include <fcntl.h>
//lecture pipe
#include <sys/types.h>
#include <unistd.h>
//wait
#include <sys/wait.h>
//vector
#include <vector>
#include <algorithm>
//------------------------------------------------------ Include personnel
#include "/public/tp/tp-multitache/Outils.h"
#include "ConfigParking.h"
///////////////////////////////////////////////////////////////////  PRIVE

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
//Vector plutot que map car pas besoin de stocker d'autres infos
static vector<pid_t> voituriersEnSortie;
static int descR;
//------------------------------------------------------ Fonctions privées
static void handlerSortie(int noSignal){
	if(noSignal == SIGUSR2){
		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			kill(*itLE, SIGUSR2);
		}
		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			waitpid(*itLE,NULL,0);
		}
		close(descR);
		exit(0);
	}

	if(noSignal == SIGCHLD){
		int status;
		//Recuperer le fils qui a envoye le SIGCHLD
		pid_t filsFini = wait(&status);

		//Efface la bonne voiture sur lecran
		switch(WEXITSTATUS(status))
		{
			case(1):
				Effacer(ETAT_P1);
				break;
			case(2):
				Effacer(ETAT_P2);
				break;
			case(3):
				Effacer(ETAT_P3);
				break;
			case(4):
				Effacer(ETAT_P4);
				break;
			case(5):
				Effacer(ETAT_P5);
				break;
			case(6):
				Effacer(ETAT_P6);
				break;
			case(7):
				Effacer(ETAT_P7);
				break;
			case(8):
				Effacer(ETAT_P8);
				break;
			default:
				break;
		}

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti);
	}
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques


void Sortie(){
	//Installation du handler
	struct sigaction action;

	action.sa_handler = handlerSortie ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;


	//armer sigusr2 sur handlerSortie;
	sigaction(SIGUSR2,&action,NULL);
	sigaction(SIGCHLD,&action,NULL);

	//Ouverture du canal de la sortie
	descR = open(canalSortie,O_RDONLY);


	int numeroPlace;
	for(;;){
		//Lecture sur le canal du numero de la place
		if(read(descR,&numeroPlace,sizeof(int)) > 0){
			pid_t voiturierSortie = SortirVoiture(numeroPlace);

			//on stocke les voituriers en sortie pour pouvoir les supprimer
			//si on appuie sur Q
			voituriersEnSortie.push_back(voiturierSortie);
		}
	}

}



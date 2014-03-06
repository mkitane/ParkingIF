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


#include <iostream>

//memoire
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
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
static int memID;
static int semID;
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
		Effacer((TypeZone)WEXITSTATUS(status));



		//Recuperer la voiture sur la mémoire partagée
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
		Voiture v = a->voituresPartagee[WEXITSTATUS(status)-1] ;
		shmdt(a);

		AfficherSortie(v.TypeUsager,v.numeroVoiture,v.heureArrivee, time(NULL));

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti);


		struct sembuf vOp = {SemaphoreCompteurPlaces,1,0};
		semop(semID,&vOp,1);


		//Si une requete est en attente, on la satisfait!
		//Recuperer la voiture sur la mémoire partagée
		if( semctl(semID,SemaphoreCompteurPlaces,GETVAL,0) == 1){

			cerr<<"on libere une voiture" << endl;
			memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
			shmdt(a);


			struct sembuf pOp2 = {MutexPorteBPPROF,1,0};
			semop(semID,&pOp2,1);
		}

	}
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques


void Sortie(int pmemID , int psemID){
	memID = pmemID;
	semID = psemID;

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



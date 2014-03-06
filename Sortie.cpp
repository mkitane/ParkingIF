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

#include <errno.h>
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
static bool operator < (enum TypeUsager lhs, enum TypeUsager rhs)
{
    //On inverse l'ordre car ici on veux
    //AUCUN<AUTRE<PROF
    //Or notre enum nous donne AUCUN<PROF<AUTRE
    if(lhs == 2 && rhs == 1) return true;
    if(lhs == 1 && rhs == 2) return false;
    return (int)lhs<(int)rhs;
}

static int getMax(Voiture const &a, Voiture const &b, Voiture const &c){
	Voiture requetePrio = a;
	//Ordre Type Usager = AUCUN,PROF,AUTRE

	if(requetePrio.TypeUsager < b.TypeUsager){
		requetePrio= b;
	}else if(b.TypeUsager == requetePrio.TypeUsager){
		if(b.heureArrivee < requetePrio.heureArrivee){
			requetePrio= b;
		}
	}


	if(requetePrio.TypeUsager < c.TypeUsager){
        requetePrio= c;
	}else if(c.TypeUsager == requetePrio.TypeUsager){
		if(c.heureArrivee < requetePrio.heureArrivee){
			requetePrio= c;
		}
	}



    if(requetePrio.TypeUsager == AUCUN){
        return 0;
    }
    if(requetePrio.TypeUsager == a.TypeUsager && requetePrio.heureArrivee == a.heureArrivee){
        return 1;
    }
    if(requetePrio.TypeUsager == b.TypeUsager && requetePrio.heureArrivee == b.heureArrivee){
        return 2;
    }
    if(requetePrio.TypeUsager == c.TypeUsager && requetePrio.heureArrivee == c.heureArrivee){
        return 3;
    }

    return 0;
}
static int gererPriorite(int memID){
	Voiture requetePorteBPPROF;
	Voiture requetePorteBPAUTRE;
	Voiture requetePorteGB;

	//Baisser Mutex Pour lecture sur memoire
	{
	//On met en place le mutex
	struct sembuf pOp = {MutexMP,-1,0};
	while(semop(semID,&pOp,1)==-1 && errno==EINTR);
	}

	memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
	requetePorteBPPROF = a->requetes[(int)PROF_BLAISE_PASCAL -1];
	requetePorteBPAUTRE = a->requetes[(int)AUTRE_BLAISE_PASCAL -1];
	requetePorteGB = a->requetes[(int)ENTREE_GASTON_BERGER -1];
	shmdt(a);
	//Lever mutex
	{
	//On relache le mutex
	struct sembuf vOp = {MutexMP,1,0};
	semop(semID,&vOp,1);
	}

	//Gestion Priorite
	//Rappel Prof tjrs Prioritaire Ensuite Temps
	return getMax(requetePorteBPPROF, requetePorteBPAUTRE, requetePorteGB);
}
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
		{
		//On met en place le mutex
		struct sembuf pOp = {MutexMP,-1,0};
		while(semop(semID,&pOp,1)==-1 && errno==EINTR);
		}
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
		Voiture v = a->voituresPartagee[WEXITSTATUS(status)-1] ;
		shmdt(a);
		{
		//On relache le mutex
		struct sembuf vOp = {MutexMP,1,0};
		semop(semID,&vOp,1);
		}


		AfficherSortie(v.TypeUsager,v.numeroVoiture,v.heureArrivee, time(NULL));

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti);


		struct sembuf vOp = {SemaphoreCompteurPlaces,1,0};
		semop(semID,&vOp,1);


		unsigned short int prio = gererPriorite(memID);
		if(prio!=0){
			//Si une requete est en attente, on la satisfait!


			{
			//On met en place le mutex
			struct sembuf pOp = {MutexMP,-1,0};
			while(semop(semID,&pOp,1)==-1 && errno==EINTR);
			}
			memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
			a->requetes[prio-1] = {AUCUN, 0,0};
			shmdt(a);

			{
			//On relache le mutex
			struct sembuf vOp = {MutexMP,1,0};
			semop(semID,&vOp,1);
			}


			cerr<<"On gere une priorite" << endl;
			struct sembuf pOp2 = {prio,1,0};
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



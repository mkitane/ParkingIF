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
static int gererPriorite(Voiture const &a, Voiture const &b, Voiture const &c){
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


static void init();
static void moteur();
static void destruction(int noSignal);
static void receptionMortVoiturier(int noSignal);


static void init()
{
	//Installation du handler
	struct sigaction action;
	action.sa_handler = destruction ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	sigaction(SIGUSR2,&action,NULL); //armer sigusr2 sur destruction;


	struct sigaction actionMortVoiturier;
	actionMortVoiturier.sa_handler = receptionMortVoiturier ;
	sigemptyset(&actionMortVoiturier.sa_mask);
	actionMortVoiturier.sa_flags = 0 ;
	sigaction(SIGCHLD,&actionMortVoiturier,NULL); //armer SigCHLD sur receptionMortVoiturier;


	//Ouverture du canal de la sortie
	descR = open(CANAL_SORTIE,O_RDONLY);
}

static void moteur()
{
	int numeroPlace;

	//Lecture sur le canal du numero de la place
	if(read(descR,&numeroPlace,sizeof(int)) > 0){
		pid_t voiturierSortie = SortirVoiture(numeroPlace);

		//on stocke les voituriers en sortie pour pouvoir les supprimer
		//si on appuie sur Q
		voituriersEnSortie.push_back(voiturierSortie);
	}
}

static void destruction(int noSignal)
{
	if(noSignal == SIGUSR2){
		//On masque SIGCHLD avant de killer !
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		//armer sigusr2 sur handlerEntree;
		sigaction(SIGCHLD,&action,NULL);


		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			kill(*itLE, SIGUSR2);
		}
		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			waitpid(*itLE,NULL,0);
		}


		close(descR);
		exit(0);
	}
}

static void receptionMortVoiturier(int noSignal)
{

	if(noSignal == SIGCHLD){
		struct sembuf reserver = {MutexMP, -1,0};	//p Operation --> Reservation
		struct sembuf liberer = {MutexMP, 1, 0};	//v Operation --> liberation
		struct sembuf vOp = {SemaphoreCompteurPlaces,1,0};

		int status;
		Voiture requetePorteBPPROF;
		Voiture requetePorteBPAUTRE;
		Voiture requetePorteGB;




		pid_t filsFini = wait(&status); //Recuperer le fils qui a envoye le SIGCHLD


		Effacer((TypeZone)WEXITSTATUS(status)); //Efface la bonne place sur l'ecran





		while(semop(semID,&reserver,1)==-1 && errno==EINTR); //Reservation de la memoire


		//Recuperer la voiture et les demandes d'entree sur la mémoire partagée
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
		Voiture v = a->voituresPartagee[WEXITSTATUS(status)-1] ;
		requetePorteBPPROF = a->requetes[(int)PROF_BLAISE_PASCAL -1];
		requetePorteBPAUTRE = a->requetes[(int)AUTRE_BLAISE_PASCAL -1];
		requetePorteGB = a->requetes[(int)ENTREE_GASTON_BERGER -1];
		shmdt(a);

		semop(semID,&liberer,1); //Liberation de la memoire



		AfficherSortie(v.TypeUsager,v.numeroVoiture,v.heureArrivee, time(NULL));

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti); //On efface le voiturier car plus besoin de le stocker


		semop(semID,&vOp,1); //on effectue l'operation v pour le semaphore a compte i.e On incremente le nombre de places


		unsigned short int prio = gererPriorite(requetePorteBPPROF, requetePorteBPAUTRE, requetePorteGB);

		if(prio!=0){
			//Si une requete est en attente, on la satisfait!


			while(semop(semID,&reserver,1)==-1 && errno==EINTR); //Reservation de la memoire

			memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
			a->requetes[prio-1] = {AUCUN, 0,0};	 //On efface la requete de la memoire
			shmdt(a);

			semop(semID,&liberer,1); //Liberation de la memoire


			struct sembuf pOp = {prio,1,0};
			semop(semID,&pOp,1); //On relache le bon semaphore de synchronisation i.e on liberer la bonne porte
		}

	}
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques


void Sortie(int pmemID , int psemID){
	memID = pmemID;
	semID = psemID;


	init();

	for(;;){
		moteur();
	}

}



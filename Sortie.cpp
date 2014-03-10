/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                :
    copyright            : (C) 2014 par Mehdi Kitane
*************************************************************************/

//---------- Réalisation de la tâche <Sortie> (fichier Sortie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <errno.h>
//------------------------------------------------------ Include personnel
#include "ConfigParking.h"
///////////////////////////////////////////////////////////////////  PRIVE

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static vector<pid_t> voituriersEnSortie; //Vecteur qui stocke les pid des
//voituriers toujours en train de se garer
static int descR;
static int memID;
static int semID;
//------------------------------------------------------ Fonctions privées
static bool operator < (enum TypeUsager lhs, enum TypeUsager rhs)
//Mode d'emploi :
//	Redefinition de l'opérateur < pour le type TypesUsager
{
    //On inverse l'ordre car ici on veux
    //AUCUN<AUTRE<PROF
    //Or notre enum nous donne AUCUN<PROF<AUTRE
    if(lhs == 2 && rhs == 1) return true;
    if(lhs == 1 && rhs == 2) return false;
    return (int)lhs<(int)rhs;
}
static int gererPriorite(Voiture const &a, Voiture const &b, Voiture const &c)
//Mode d'emploi :
//	Permet de gerer les prioritées entre prof et autre
//	Un prof est toujours prioritaire
//	Entre deux usagers de meme type, celui qui est arrivé en premier est
//	servi en premier
{
	Voiture requetePrio = a;

	if(requetePrio.typeUsager < b.typeUsager){
		requetePrio= b;
	}else if(b.typeUsager == requetePrio.typeUsager){
		if(b.instantArrivee < requetePrio.instantArrivee){
			requetePrio= b;
		}
	}


	if(requetePrio.typeUsager < c.typeUsager){
        requetePrio= c;
	}else if(c.typeUsager == requetePrio.typeUsager){
		if(c.instantArrivee < requetePrio.instantArrivee){
			requetePrio= c;
		}
	}



    if(requetePrio.typeUsager == AUCUN){
        return 0;
    }
    if(requetePrio.typeUsager == a.typeUsager && requetePrio.instantArrivee == a.instantArrivee){
        return 1;
    }
    if(requetePrio.typeUsager == b.typeUsager && requetePrio.instantArrivee == b.instantArrivee){
        return 2;
    }
    if(requetePrio.typeUsager == c.typeUsager && requetePrio.instantArrivee == c.instantArrivee){
        return 3;
    }

    return 0;
}


static void destruction(int noSignal);
static void receptionMortVoiturier(int noSignal);


static void initialisation()
//Mode d'emploi :
//	Phase d'initialisation du processus Sortie
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
//Mode d'emploi
//	Phase moteur du processus Sortie
{
	int numeroPlace;

	//Lecture sur le canal du numero de la place
	if(read(descR,&numeroPlace,sizeof(int)) > 0){
		pid_t voiturierSortie = SortirVoiture(numeroPlace);

		//on stocke les voituriers en sortie pour pouvoir les supprimer si on appuie sur Q
		voituriersEnSortie.push_back(voiturierSortie);
	}
}

static void destruction(int noSignal)
//Mode d'emploi :
//	Phase de destruction du processus Sortie
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
			kill(*itLE, SIGUSR2);//Envoi du signal SIGUSR2 aux voitures en train de se garer
		}
		for(vector<pid_t>::iterator itLE = voituriersEnSortie.begin(); itLE != voituriersEnSortie.end(); itLE++){
			waitpid(*itLE,NULL,0);  //Attente de la fin des voitures a laquelles on a envoyé un signal
		}


		close(descR); //Fermeture du canal
		exit(0);
	}
}

static void receptionMortVoiturier(int noSignal)
//Mode d'emploi
//	--Handler pour le signal SIGCHLD
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
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ; //Attachement
		Voiture v = a->placesParking[WEXITSTATUS(status)-1] ;
		requetePorteBPPROF = a->requetes[(int)PROF_BLAISE_PASCAL -1];
		requetePorteBPAUTRE = a->requetes[(int)AUTRE_BLAISE_PASCAL -1];
		requetePorteGB = a->requetes[(int)ENTREE_GASTON_BERGER -1];
		shmdt(a); //Detachement

		semop(semID,&liberer,1); //Liberation de la memoire



		AfficherSortie(v.typeUsager,v.numeroPlaque,v.instantArrivee, time(NULL));

		vector<pid_t>::iterator itSorti = std::find(voituriersEnSortie.begin(),voituriersEnSortie.end(),filsFini);
		voituriersEnSortie.erase(itSorti); //On efface le voiturier car plus besoin de le stocker


		semop(semID,&vOp,1); //on effectue l'operation v pour le semaphore à compte i.e On incremente le nombre de places


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


void Sortie(int pmemID , int psemID)
//Mode d'emploi
//	Processus fils Sortie
{
	memID = pmemID;
	semID = psemID;


	initialisation();

	for(;;){
		moteur();
	}

}



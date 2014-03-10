/*************************************************************************
                           Entree  -  description
                             -------------------
    début                : Entree
    copyright            : (C) Entree par Entree
    e-mail               : Entree
*************************************************************************/

//---------- Réalisation de la tâche <Entree> (fichier Entree.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Entree.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string.h>


#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int descR; //descripteur du canal qui relie le clavier a l'entree
static map<pid_t,Voiture> mapVoiture; //Map qui stocke les voituriers entrain de se garer
static int memID;
static int semID;
//------------------------------------------------------ Fonctions privées
static void destruction(int noSignal);
static void receptionMortVoiturier(int noSignal);


static void initialisation(TypeBarriere parametrage)
//Mode d'emploi :
//	Phase d'initialisation du processus Entree
{
	//Installation du handler destruction
	struct sigaction action;
	action.sa_handler = destruction ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;
	sigaction(SIGUSR2,&action,NULL); //armer SIGUSR2 sur destruction;



	//Installation du handler actionFinVoiturier
	struct sigaction actionFinVoiturier;
	actionFinVoiturier.sa_handler = receptionMortVoiturier ;
	sigemptyset(&actionFinVoiturier.sa_mask);
	actionFinVoiturier.sa_flags = 0 ;
	sigaction(SIGCHLD,&actionFinVoiturier,NULL);  //armer SIGCHLD sur actionFinVoiturier;

	switch(parametrage)
	{
		case(PROF_BLAISE_PASCAL):
			descR = open(CANAL_PROF_BP,O_RDONLY); 	//Ouverture Canal
			break;
		case(AUTRE_BLAISE_PASCAL):
			descR = open(CANAL_AUTRE_BP,O_RDONLY);	//Ouverture Canal
			break;
		case(ENTREE_GASTON_BERGER):
			descR = open(CANAL_GB,O_RDONLY);	//Ouverture Canal
			break;
		default:
			break;
	}
}

static void moteur(TypeBarriere parametrage)
//Mode d'emploi :
//	Phase moteur du processus Entree
{
	Voiture voiture;
	struct sembuf reserver = {MutexMP, -1,0};	//p Operation --> Reservation
	struct sembuf liberer = {MutexMP, 1, 0};	//v Operation --> liberation

		if(read(descR,&voiture,sizeof(Voiture)) > 0){ //lecture canal

			DessinerVoitureBarriere(parametrage,voiture.typeUsager);

			if( semctl(semID,SemaphoreCompteurPlaces,GETVAL,0) <= 0){ //Recuperation valeur du semaphore a compte
				//Si il ne reste plus de place
				//On place en liste d'attente !
				AfficherRequete(parametrage, voiture.typeUsager, voiture.instantArrivee);


				//On ecrit dans la mémoire partagée que l'on a une requete !


				while(semop(semID,&reserver,1)==-1 && errno==EINTR); //Reservation de la memoire partagee

				//Ecrire la voiture sur la mémoire partagée
				memStruct *a = (memStruct *) shmat(memID, NULL, 0) ; //attachement
				a->requetes[parametrage-1] =  voiture ;
				shmdt(a); //detachement


				semop(semID,&liberer,1); //Liberation de la memoire partagee




				struct sembuf pOp = {parametrage,-1,0};  //p Operation sur le mutex de synchronisation
				while(semop(semID,&pOp,1)==-1 && errno==EINTR);

				Effacer((TypeZone)(ETAT_P8+parametrage));
			}


			//On met a jour le Semaphore compteur de place
			struct sembuf pOp = {SemaphoreCompteurPlaces,-1,0};
			semop(semID,&pOp,1);




			// garage voiture ajout du pid voiturier dans la list
			pid_t voiturier=GarerVoiture(parametrage);
			mapVoiture.insert(pair<pid_t,Voiture>(voiturier,voiture));

			//sleep 1s
			sleep(TEMPO);

		}
}

static void destruction(int noSignal)
//Mode d'emploi :
//	Phase de destruction du processus Entree
{
	if(noSignal == SIGUSR2){
		//On masque SIGCHLD avant de killer !
		struct sigaction action;
		action.sa_handler = SIG_IGN ;
		sigemptyset(&action.sa_mask);
		action.sa_flags = 0 ;
		sigaction(SIGCHLD,&action,NULL);




		for(map<pid_t,Voiture>::iterator it=mapVoiture.begin(); it!= mapVoiture.end() ; it++){
			kill(it->first,SIGUSR2); //Envoi du signal SIGUSR2 aux voitures en train de se garer
		}
		for(map<pid_t,Voiture>::iterator it=mapVoiture.begin(); it!= mapVoiture.end() ; it++){
			waitpid(it->first,NULL,0); //Attente de la fin des voitures a laquelles on a envoyé un signal
		}

		close(descR); //fermeture canal
		exit(0);
	}
}

static void receptionMortVoiturier(int noSignal)
//Mode d'emploi
//	Handler pour le signal SIGCHLD
{

	if(noSignal == SIGCHLD){
		struct sembuf reserver = {MutexMP, -1,0};	//p Operation --> Reservation
		struct sembuf liberer = {MutexMP, 1, 0};	//v Operation --> liberation


		int status;
		//Recuperer le fils qui a envoye le SIGCHLD
		pid_t filsFini = wait(&status);


		//Recuperer la bonne voiture qui a lancé le signal
		map<pid_t,Voiture>::iterator itLE = mapVoiture.find(filsFini);
		Voiture v = itLE ->second ;

		//Afficher ses caractéristiques dans l'endroit indique
		AfficherPlace(WEXITSTATUS(status),v.typeUsager,v.numeroPlaque,v.instantArrivee);




		while(semop(semID,&reserver,1)==-1 && errno==EINTR); //Reservation de la memoire


		//Ecrire la voiture sur la mémoire partagée
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ; //attachment
		a->placesParking[WEXITSTATUS(status)-1] = v ;
		shmdt(a); //detachement



		semop(semID,&liberer,1); //Liberation de la memoire




		//Supprimer la bonne voiture de la map des voitures en train de stationner
		mapVoiture.erase(itLE);

	}
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques


void Entree(TypeBarriere parametrage,int pmemID, int psemID)
//Mode d'emploi :
//Processus Fils Entree
{
	memID = pmemID; //Récuperation de l'identifiant de la mémoire partagée
	semID = psemID; //Récuperation de l'identifiant du sémaphore général

	initialisation(parametrage);

	for(;;){
		moteur(parametrage);
	}
}



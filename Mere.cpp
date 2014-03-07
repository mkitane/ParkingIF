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


#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/sem.h>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Clavier.h"
#include "Entree.h"
#include "Sortie.h"
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

	int memID;
	int semID;

	//Declaration des differents pids des processus déclarés
	pid_t noClavier;
	pid_t noHeure;
	pid_t noEntreeUn;
	pid_t noEntreeDeux;
	pid_t noEntreeTrois;
	pid_t noSortie;



	//Mise en place du canal de communication entre la mere et le clavier
	//Creation des differents canaux de communication nommes
	if(mkfifo(CANAL_PROF_BP,DROITS_CANAL) == -1){
		cerr<< "erreur creation du canal entre entree et clavier" << endl;
		return -1 ;
	}
	if(mkfifo(CANAL_AUTRE_BP,DROITS_CANAL) == -1){
		cerr<< "erreur creation du canal entre entree et clavier" << endl;
		return -1 ;
	}
	if(mkfifo(CANAL_GB,DROITS_CANAL) == -1){
		cerr<< "erreur creation du canal entre entree et clavier" << endl;
		return -1 ;
	}
	if(mkfifo(CANAL_SORTIE,DROITS_CANAL) == -1){
		cerr<< "erreur creation du canal entre sortie et clavier" << endl;
		return -1 ;
	}


	//Creation de la memoire partagee
	memID = shmget(ftok(CHEMIN_EXE,1), sizeof(memStruct), IPC_CREAT | IPC_EXCL | DROITS_MEMOIRE);

	if(memID == -1){
		cerr << "erreur creation memoire Partagee" << endl;
		return -1;
	}

	//Initialisation de la memoire partagee
	memStruct *a = (memStruct *) shmat(memID, NULL, 0);
	for(int i=0; i<(int)NB_PLACES ; i++){
		a->voituresPartagee[i] = {AUCUN, 0,0};
	}
	for(int i=0; i<(int)NB_BARRIERES_ENTREE ; i++){
		a->requetes[i] = {AUCUN, 0,0};
	}
	shmdt(a);

	//Creation des semaphores
	semID = semget(ftok(CHEMIN_EXE,2), NB_SEM , IPC_CREAT | IPC_EXCL | DROITS_SEMAPHORE);

	if(semID == -1){
		cerr << "erreur creation semaphore" << endl;
		return -1;
	}

	//Initialisation des semaphores
		//Initialisation du semaphore compteur de places
	semctl(semID,SemaphoreCompteurPlaces,SETVAL,NB_PLACES);
		//Initialisation des mutexs et sem synchro
	semctl(semID,SynchroPorteBPPROF,SETVAL,0);
	semctl(semID,SynchroPorteBPAUTRE,SETVAL,0);
	semctl(semID,SynchroPorteGB,SETVAL,0);
	semctl(semID,MutexMP,SETVAL,1);



	InitialiserApplication(TERMINALUTILISE);


	noHeure = ActiverHeure();

	if( (noClavier = fork() ) == 0 ){
		/*Code du fils */
		Clavier();
	}else if( (noEntreeUn = fork() ) == 0 ){
		/*Code du fils */
		Entree(PROF_BLAISE_PASCAL,memID,semID);

	}else if( (noEntreeDeux = fork() ) == 0 ){
		/*Code du fils */
		Entree(AUTRE_BLAISE_PASCAL,memID,semID);

	}else if( (noEntreeTrois = fork() ) == 0 ){
		/*Code du fils */
		Entree(ENTREE_GASTON_BERGER,memID,semID);

	}else if( (noSortie = fork()) == 0 ){
		/*Code du fils*/
		Sortie(memID,semID);

	}else{
		/*Code du pere */



		//On attend la reception de Q du clavier
		//Synchro avec le clavier
		waitpid(noClavier, NULL, 0);


		//Demande de fin avec envoi de SIGUSR2
		kill(noSortie,SIGUSR2);
		kill(noEntreeTrois,SIGUSR2);
		kill(noEntreeDeux,SIGUSR2);
		kill(noEntreeUn,SIGUSR2);
		kill(noHeure, SIGUSR2);


		//attente de fin des processus fils
		waitpid(noSortie,NULL,0);
		waitpid(noEntreeTrois,NULL,0);
		waitpid(noEntreeDeux,NULL,0);
		waitpid(noEntreeUn,NULL,0);
		waitpid(noHeure, NULL,0);


		//fermeture des canaux de communication
		unlink(CANAL_SORTIE);
		unlink(CANAL_GB);
		unlink(CANAL_AUTRE_BP);
		unlink(CANAL_PROF_BP);


		//Suppression memoire partagee
		shmctl(memID, IPC_RMID,0);

		//Suppression du semaphore
		semctl(semID, 0, IPC_RMID, 0);

		TerminerApplication();
		exit(0);

	}


	return 0;
}

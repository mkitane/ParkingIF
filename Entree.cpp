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
static int descR;
static map<pid_t,Voiture> mapVoiture;
static int memID;

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
static void handlerEntree(int noSignal){
	if(noSignal == SIGUSR2){

		//TODO: A REGLER : Voiturier ne quitte pas directement
		map<pid_t,Voiture>::iterator it;
		for(it=mapVoiture.begin(); it!= mapVoiture.end() ; it++){
			kill(it->first,SIGUSR2);
		}
		for(it=mapVoiture.begin(); it!= mapVoiture.end() ; it++){
			waitpid(it->first,NULL,0);
		}

		close(descR);
		exit(0);
	}




	if(noSignal == SIGCHLD){

		int status;
		//Recuperer le fils qui a envoye le SIGCHLD
		pid_t filsFini = wait(&status);


		//Recuperer la bonne voiture qui a lancé le signal
		map<pid_t,Voiture>::iterator itLE = mapVoiture.find(filsFini);
		Voiture v = itLE ->second ;

		//Afficher ses caractéristiques dans l'endroit indique
		AfficherPlace(WEXITSTATUS(status),v.TypeUsager,v.numeroVoiture,v.heureArrivee);


		//Ecrire la voiture sur la mémoire partagée
		memStruct *a = (memStruct *) shmat(memID, NULL, 0) ;
		a->voituresPartagee[WEXITSTATUS(status)-1] = v ;
		shmdt(a);

		//Supprimer la bonne voiture de la map des voitures en train de stationner
		mapVoiture.erase(itLE);

	}
}

void Entree(TypeBarriere Parametrage,int pmemID){
	memID = pmemID;

	//Installation du handler
	struct sigaction action;

	action.sa_handler = handlerEntree ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;


	//armer sigusr2 sur handlerEntree;
	sigaction(SIGUSR2,&action,NULL);
	sigaction(SIGCHLD,&action,NULL);

	switch(Parametrage)
	{
		case(PROF_BLAISE_PASCAL):
			//Prof Blaise Pascal
			descR = open(canalProfBP,O_RDONLY);
			break;
		case(AUTRE_BLAISE_PASCAL):
			//descR = open(canalAutreBP,O_RDONLY);
			break;
		case(ENTREE_GASTON_BERGER):
			//descR = open(canalGB,O_RDONLY);
			break;
		default:
			break;
	}

	for(;;){
		Voiture voiture;

		if(read(descR,&voiture,sizeof(Voiture)) > 0){

			// garage voiture ajout du pid voiturier dans la list
			DessinerVoitureBarriere(PROF_BLAISE_PASCAL,voiture.TypeUsager);
			pid_t voiturier=GarerVoiture(PROF_BLAISE_PASCAL);

			if(voiturier != -1){
				mapVoiture.insert(pair<pid_t,Voiture>(voiturier,voiture));
				//sleep 1s
				sleep(TEMPO);
			}else{
				//Place indisponible
			}
		}
	}

}



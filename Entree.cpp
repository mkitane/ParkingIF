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
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void handlerEntree(int noSignal){
	if(noSignal == SIGUSR2){
		map<pid_t,Voiture>::iterator it;
		for(it==mapVoiture.begin(); it!= mapVoiture.end() ; it++){
			kill(it->first,SIGUSR2);
		}
		for(it==mapVoiture.begin(); it!= mapVoiture.end() ; it++){
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


	}
}

void Entree(TypeBarriere Parametrage){
	//Installation du handler
	struct sigaction action;

	action.sa_handler = handlerEntree ;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0 ;


	//armer sigusr2 sur handlerEntree;
	sigaction(SIGUSR2,&action,NULL);
	sigaction(SIGCHLD,&action,NULL);
	//Prof Blaise Pascal
	descR = open(canalProfBP,O_RDONLY);

	Voiture voiture;

	for(;;){
		if(read(descR,&voiture,sizeof(Voiture)) > 0){

			// garage voiture ajout du pid voiturier dans la list
			DessinerVoitureBarriere(PROF_BLAISE_PASCAL,voiture.TypeUsager);
			pid_t voiturier=GarerVoiture(PROF_BLAISE_PASCAL);

			mapVoiture.insert(pair<pid_t,Voiture>(voiturier,voiture));
			//sleep 1s
			sleep(TEMPO);
		}
	}

}



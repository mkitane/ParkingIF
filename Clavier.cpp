/*************************************************************************
                           Clavier  -  description
                             -------------------
    début                : Clavier
    copyright            : (C) Clavier par Clavier
    e-mail               : Clavier
*************************************************************************/

//---------- Réalisation de la tâche <Clavier> (fichier Clavier.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Clavier.h"
#include "/public/tp/tp-multitache/Outils.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int descProfBPW ;
static int descSortieW;
static int descGBW ;
static int descAutreBPW ;
static int compteurVoiture = 0;

//------------------------------------------------------ Fonctions privées
static void init()
{
	//Ouverture Canal
	descProfBPW = open(CANAL_PROF_BP,O_WRONLY);
	descAutreBPW  = open(CANAL_AUTRE_BP,O_WRONLY);
	descGBW = open(CANAL_GB,O_WRONLY);
	descSortieW = open(CANAL_SORTIE, O_WRONLY);
}

static void destruction()
{
	close(descSortieW);
	close(descGBW);
	close(descAutreBPW);
	close(descProfBPW);

	exit(0);
}
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Clavier()
// Algorithme :
//
{
	init();

	for(;;){
		Menu();
	}
} //----- fin de Nom

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	if(code == 'Q'){
		destruction();
	}else if(code == 'P'){
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0;
		}

		Voiture voiture;
		voiture.numeroVoiture = compteurVoiture;
		voiture.heureArrivee = time(NULL);
		voiture.TypeUsager = PROF;

		if(valeur == 1){
			//Prof Blaise Pascal
			write(descProfBPW,&voiture,sizeof(voiture));
		}
		if(valeur == 2){
			//Prof Gaston Berger
			write(descGBW,&voiture,sizeof(voiture));
		}
	}else if(code == 'A'){
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0;
		}


		Voiture voiture;
		voiture.numeroVoiture = compteurVoiture;
		voiture.heureArrivee = time(NULL);
		voiture.TypeUsager = AUTRE;


		if(valeur ==1){
			//Autre Blaise Pascal
			write(descAutreBPW,&voiture,sizeof(voiture));
		}
		if(valeur == 2){
			//Autre Gaston Berger
			write(descGBW,&voiture,sizeof(voiture));
		}
	}else if(code == 'S'){
		write(descSortieW,&valeur,sizeof(int));
	}
}

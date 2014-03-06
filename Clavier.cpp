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
static int descProfBPW ;
static int descSortieW;
static int descGBW ;
static int descAutreBPW ;

static int compteurVoiture = 0;
void Clavier()
// Algorithme :
//
{
	//Ouverture Canal
	descProfBPW = open(canalProfBP,O_WRONLY);
	descAutreBPW  = open(canalAutreBP,O_WRONLY);
	descGBW = open(canalGB,O_WRONLY);
	descSortieW = open(canalSortie, O_WRONLY);

	for(;;){
		Menu();
	}
} //----- fin de Nom

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	if(code == 'Q'){
		close(descProfBPW);
		close(descAutreBPW);
		close(descGBW);
		close(descSortieW);

		exit(0);
	}else if(code == 'P'){
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0;
		}
		Afficher(MESSAGE,"P pressed");

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
		Afficher(MESSAGE,"A pressed");
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
		Afficher(MESSAGE,"S pressed");
		write(descSortieW,&valeur,sizeof(int));
	}
}

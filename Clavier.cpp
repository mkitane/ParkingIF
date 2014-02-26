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
static int descW ;
static int compteurVoiture = 0;
void Clavier()
// Algorithme :
//
{
	descW = open("fifo1",O_WRONLY);

	for(;;){
		Menu();
	}
} //----- fin de Nom

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	if(code == 'Q'){
		close(descW);
		exit(0);
	}else if(code == 'P'){
		char c = 'c';
		Afficher(MESSAGE,"P pressed");
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0;
		}
		if(valeur == 1){
			write(descW,&c,sizeof(c));
		}
		if(valeur == 2){

		}
	}else if(code == 'A'){
		Afficher(MESSAGE,"A pressed");
	}else if(code == 'S'){
		Afficher(MESSAGE,"S pressed");
	}
}

/*************************************************************************
                        Clavier  -  description
                          -------------------
 début                :
 copyright            : (C) 2014 par Mehdi Kitane
 *************************************************************************/

//---------- Réalisation de la tâche <Clavier> (fichier Clavier.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Clavier.h"
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
static void initialisation()
//Mode d'emploi :
//	Phase d'initialisation du processus Clavier
{
	//Ouverture des canaux
	descProfBPW = open(CANAL_PROF_BP,O_WRONLY);
	descAutreBPW  = open(CANAL_AUTRE_BP,O_WRONLY);
	descGBW = open(CANAL_GB,O_WRONLY);
	descSortieW = open(CANAL_SORTIE, O_WRONLY);
}//---Fin de initialisation

static void destruction()
//Mode d'emploi :
//	Phase de destruction du processus Clavier
{
	//Fermeture des canaux
	close(descSortieW);
	close(descGBW);
	close(descAutreBPW);
	close(descProfBPW);

	exit(0);
}//---Fin de destruction


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Clavier()
//Mode d'emploi:
//	 Processus Fils Clavier
//
{
	initialisation();

	for(;;){
		Menu();
	}
} //----- fin de Clavier

void Commande(char code, unsigned int valeur)
// Mode d'emploi :
//
{
	if(code == 'Q'){
		destruction();
	}else if(code == 'P'){
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0;  //Reinitialisation du compteur
		}


		Voiture voiture;
		voiture.numeroPlaque = compteurVoiture;
		voiture.instantArrivee = time(NULL);
		voiture.typeUsager = PROF;

		if(valeur == 1){
			//Prof Blaise Pascal
			write(descProfBPW,&voiture,sizeof(voiture));//Ecriture de la voiture arrivée dans le canal
		}
		if(valeur == 2){
			//Prof Gaston Berger
			write(descGBW,&voiture,sizeof(voiture)); //Ecriture de la voiture arrivée dans le canal
		}
	}else if(code == 'A'){
		compteurVoiture++;
		if(compteurVoiture > 999){
			compteurVoiture=0; //Reinitialisation du compteur
		}


		Voiture voiture;
		voiture.numeroPlaque = compteurVoiture;
		voiture.instantArrivee = time(NULL);
		voiture.typeUsager = AUTRE;


		if(valeur ==1){
			//Autre Blaise Pascal
			write(descAutreBPW,&voiture,sizeof(voiture));//Ecriture de la voiture arrivée dans le canal
		}
		if(valeur == 2){
			//Autre Gaston Berger
			write(descGBW,&voiture,sizeof(voiture));//Ecriture de la voiture arrivée dans le canal
		}
	}else if(code == 'S'){
		write(descSortieW,&valeur,sizeof(int));//Ecriture du numero de la place a sortir
	}
}//--Fin de commande

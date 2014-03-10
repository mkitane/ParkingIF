/*************************************************************************
                           ConfigParking  -  description
                             -------------------
    début                :
    copyright            : (C) 2014 par Mehdi Kitane
*************************************************************************/

//---------- Interface de la tâche <Clavier> (fichier Clavier.h) -------
#if ! defined ( ConfigParking_H )
#define ConfigParking_H

//------------------------------------------------------------------------
// Rôle du module ConfigParking
//	Ce module regroupe les différentes configurations utilisées par
//	l'application
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include </public/tp/tp-multitache/Outils.h>


//------------------------------------------------------------- Constantes
#define TERMINALUTILISE XTERM
#define DROITS_CANAL 0660
#define DROITS_MEMOIRE 0660
#define DROITS_SEMAPHORE 0660

//Nom des differents canaux
#define CANAL_PROF_BP  "canalProfBP"
#define CANAL_AUTRE_BP "canalAutreBP"
#define CANAL_GB  "canalGB"
#define CANAL_SORTIE "canalSortie"

const int NB_SEM = 5; //Nombre de sémaphores elementaires
const int TEMPO = 1 ; //Temporisation d'attente avant l'arrivée d'un nouveau véhicule


#define CHEMIN_EXE  "./Parking" //Fichier utilisé pour batir la clé publique
//------------------------------------------------------------------ Types

//définition de la structure utilisée pour modéliser une voiture
// Une voiture est représentée par la personne la conduisant, son numero d'immatriculation
//et son instant d'arrivée
typedef struct Voiture {
	enum TypeUsager typeUsager;
	int numeroPlaque;
	time_t instantArrivee;
} Voiture;


//définition de la structure utilisée par la mémoire partagée
//Elle contient les 8 places de parking et les 3 requetes
typedef struct memStruct{
	Voiture placesParking[NB_PLACES];

	Voiture requetes[NB_BARRIERES_ENTREE];
} memStruct;

//definition des différents sémaphores élémentaires utilisés par l'application
typedef enum Semaphore
{
	SemaphoreCompteurPlaces = 0,
	SynchroPorteBPPROF = 1,
	SynchroPorteBPAUTRE = 2,
	SynchroPorteGB = 3,
	MutexMP = 4
}Semaphore;
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

#endif

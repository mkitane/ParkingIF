/*************************************************************************
                           Clavier  -  description
                             -------------------
    début                : Clavier
    copyright            : (C) Clavier par Clavier
    e-mail               : Clavier
*************************************************************************/

//---------- Interface de la tâche <Clavier> (fichier Clavier.h) -------
#if ! defined ( ConfigParking_H )
#define ConfigParking_H

//------------------------------------------------------------------------
// Rôle de la tâche <Clavier>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include </public/tp/tp-multitache/Outils.h>


//------------------------------------------------------------- Constantes
#define TERMINALUTILISE XTERM
#define DROITS_CANAL 0660
#define DROITS_MEMOIRE 0660
#define DROITS_SEMAPHORE 0666

#define CANAL_PROF_BP  "canalProfBP"
#define CANAL_AUTRE_BP "canalAutreBP"
#define CANAL_GB  "canalGB"
#define CANAL_SORTIE "canalSortie"

const int NB_SEM = 5;
const int TEMPO = 1 ;


#define CHEMIN_EXE  "Parking"
//------------------------------------------------------------------ Types

typedef struct Voiture {
	enum TypeUsager TypeUsager;
	int numeroVoiture;
	time_t heureArrivee;
} Voiture;

typedef struct memStruct{
	Voiture voituresPartagee[NB_PLACES];

	Voiture requetes[NB_BARRIERES_ENTREE];
} memStruct;


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

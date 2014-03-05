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
#define DROITSCANAL 0660
#define DROITSMEM 0660
#define TEMPO 1

static const char* canalProfBP = "fifo1" ;
static const char* canalSortie = "fifo4" ;
//static const char* canalAutreBP = "fifo2" ;
//static const char* canalGB = "fifo3" ;

static const char* memoirePartagee = "Parking";
//------------------------------------------------------------------ Types

typedef struct Voiture {
	enum TypeUsager TypeUsager;
	int numeroVoiture;
	time_t heureArrivee;
} Voiture;
typedef struct memStruct{
	Voiture voituresPartagee[NB_PLACES];
} memStruct;
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

#endif

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

//------------------------------------------------------------------ Types

struct Voiture {
	TypeUsager TypeUsager;
	int numeroVoiture;
	time_t heureArrivee;
};

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

#endif

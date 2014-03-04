/*************************************************************************
                           Entree  -  description
                             -------------------
    début                : Entree
    copyright            : (C) Entree par Entree
    e-mail               : Entree
*************************************************************************/

//---------- Interface de la tâche <Entree> (fichier Entree.h) -------
#if ! defined ( Entree_H )
#define Entree_H

//------------------------------------------------------------------------
// Rôle de la tâche <Entree>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "/public/tp/tp-multitache/Outils.h"
#include "ConfigParking.h"
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Entree(TypeBarriere Parametrage, int pmemID);
//TODO : CHANGER LE INT PARAMETRAGE PLUTARD
// Mode d'emploi :
//
// Contrat :
//

#endif // Entree_H


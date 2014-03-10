/*************************************************************************
                           Entree  -  description
                             -------------------
    début                :
    copyright            : (C) 2014 par Mehdi Kitane
*************************************************************************/

//---------- Interface de la tâche <Entree> (fichier Entree.h) -------
#if ! defined ( Entree_H )
#define Entree_H

//------------------------------------------------------------------------
// Rôle de la tâche <Entree>
//	La tache Entrée permet de gérer les voituriers qui rentrent dans le
//	Parking
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "ConfigParking.h"
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Entree(TypeBarriere parametrage, int pmemID, int psemID);
// Mode d'emploi :
//	Processus fils Entree
// Contrat :
//

#endif // Entree_H


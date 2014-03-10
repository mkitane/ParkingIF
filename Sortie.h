/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                :
    copyright            : (C) 2014 par Mehdi Kitane
*************************************************************************/

//---------- Interface de la tâche <Sortie> (fichier Sortie.h) -------
#if ! defined ( Sortie_H )
#define Sortie_H

//------------------------------------------------------------------------
// Rôle de la tâche <Sortie>
//	La tache Sortie permet de gérer les voituriers qui sortent du Parking
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Sortie(int pmemID, int psemID);
// Mode d'emploi :
//	Processus fils Sortie
// Contrat :
//

#endif // Sortie_H


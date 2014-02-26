/*************************************************************************
                           Clavier  -  description
                             -------------------
    début                : Clavier
    copyright            : (C) Clavier par Clavier
    e-mail               : Clavier
*************************************************************************/

//---------- Interface de la tâche <Clavier> (fichier Clavier.h) -------
#if ! defined ( Clavier_H )
#define Clavier_H

//------------------------------------------------------------------------
// Rôle de la tâche <Clavier>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "/public/tp/tp-multitache/Menu.h"
#include "ConfigParking.h"
#include <stdlib.h>
#include <time.h>
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Clavier();
// Mode d'emploi :
//
// Contrat :
//

void Commande(char code, unsigned int valeur);
// Mode d'emploi :
//
// Contrat :
//


#endif // Clavier_H


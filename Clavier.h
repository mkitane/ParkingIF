/*************************************************************************
                        Clavier  -  description
                          -------------------
 début                :
 copyright            : (C) 2014 par Mehdi Kitane
 *************************************************************************/

//---------- Interface de la tâche <Clavier> (fichier Clavier.h) -------
#if ! defined ( Clavier_H )
#define Clavier_H

//------------------------------------------------------------------------
// Rôle de la tâche <Clavier>
//Cette tache permet de prendre en charge les entrées clavier de
//l'utilisateur et de les traiter et effectuer les actions adéquates.
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
//		Processus fils Clavier
// Contrat :
//

void Commande(char code, unsigned int valeur);
// Mode d'emploi :
//		Cette procedure est appelé par Menu() et permet de gerer les entrées clavier
//de l'utilisateur
// Contrat :
//


#endif // Clavier_H


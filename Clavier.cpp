/*************************************************************************
                           Clavier  -  description
                             -------------------
    début                : Clavier
    copyright            : (C) Clavier par Clavier
    e-mail               : Clavier
*************************************************************************/

//---------- Réalisation de la tâche <Clavier> (fichier Clavier.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Clavier.h"
#include "/public/tp/tp-multitache/Outils.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Clavier()
// Algorithme :
//
{
	for(;;){
		Menu();
	}
} //----- fin de Nom

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case 'Q' :
			exit(0);
			break;
		case 'P' :
			Afficher(MESSAGE,"P pressed");
			break;
		case 'A' :
			Afficher(MESSAGE,"A pressed");
			break;
		case 'S' :
			Afficher(MESSAGE,"S pressed");
			break;
		default :
			break;
	}
}

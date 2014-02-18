/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : Mere
    copyright            : (C) Mere par Mere
    e-mail               : Mere
*************************************************************************/

//---------- Réalisation de la tâche <Mere> (fichier Mere.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "Clavier.h"
#include "/public/tp/tp-multitache/Outils.h"
#include "/public/tp/tp-multitache/Heure.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
#define TERMINALUTILISE XTERM
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
static pid_t noClavier;
int main (void)
//Algorithme :
{
	InitialiserApplication(TERMINALUTILISE);

	if( (noClavier = fork()) == 0)
	{
		/*Code du fils */
		Clavier();
	}else{
		/*Code du pere */

		waitpid(noClavier, NULL, 0); //On attends la reception de Q du clavier



		TerminerApplication();
		exit(0);

	}

	return 0;
}

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
#include "Entree.h"
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
static pid_t noHeure;
static pid_t noEntreeUn;
static pid_t noEntreeDeux;
static pid_t noEntreeTrois;
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
		noHeure = ActiverHeure();

		if( (noEntreeUn = fork()) ==0){
			/*Code du fils */
			Entree(PROF_BLAISE_PASCAL);

		}else{

		/*Code du pere */
		waitpid(noClavier, NULL, 0); //On attend la reception de Q du clavier

		//Fin de tache heure apres reception de SIGUSR2
		kill(noHeure, SIGUSR2);

		//Demande Fin vers Entree1
		kill(noEntreeUn,SIGUSR2);
		waitpid(noEntreeUn,NULL,0);

		TerminerApplication();
		exit(0);

		}

	}

	return 0;
}

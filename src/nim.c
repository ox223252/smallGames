#include <stdio.h>
#include <stdint.h>

int nim ( void )
{
	int16_t nObjets;
	uint16_t choix;
	uint8_t i;
	uint8_t joueurId = 0;
	uint16_t nbJoueurs = 1;

	int valid = 0;
 
	printf ( "Nombre de joueurs (1 ou 2 ) :\n" );
	do
	{
		valid = scanf ( "%hu", &nbJoueurs );
		while ( getchar ( ) != '\n' );

		if ( valid &&
			( nbJoueurs >= 1 ) &&
			( nbJoueurs <= 2 ) )
		{
			break;
		}
		printf ( "\e[A\e[2K" );
	}
	while ( 1 );

	printf ( "Nombre d'allumettes total? (entre 1 et 20) :\n" );
	do
	{
		valid = scanf ( "%hu", &nObjets );
		while ( getchar ( ) != '\n' );

		if ( valid &&
			( nObjets >= 1 ) &&
			( nObjets <= 20 ) )
		{
			break;
		}
		printf ( "\e[A\e[2K" );
	}
	while ( 1 );

	do
	{
		for ( i = 0; i < nObjets; i++ )
		{
			printf ( "| " );
		}
		printf ( "\n" );

		if ( !joueurId || 
			( nbJoueurs > 1 ) )
		{ // human palyers
			printf ( "Retirer 1, 2 ou 3 alumette(s)? :\n" );
			do
			{
				valid = scanf ( "%hu", &choix );
				while ( getchar ( ) != '\n' );

				if ( valid &&
					( choix >= 1 ) &&
					( choix <= 3 ) )
				{
					break;
				}
				printf ( "\e[A\e[2K" );
			}
			while ( 1 );
		}
		else
		{ // if no second human
			if ( ( nObjets % 4 ) != 0 )
			{
				if ( nObjets > 4 )
				{
					choix = ( nObjets ) % 4;
				}
				else
				{
					choix = nObjets - 1;
				}
			}
			else if ( nObjets == 4 )
			{
				choix = 3;
			}
			else
			{
				choix = rand ( ) % ( ( nObjets > 4 )? 4 : nObjets );
			}
		}

		printf ( "player \e[1;33m%s\e[0m pick %d\n", (!joueurId)?"0":(nbJoueurs > 1)?"1":"IA", choix );

		nObjets -= choix;

		if ( nObjets <= 1 )
		{
			printf ( "Joueur \e[1;33m%s\e[0m %s\n", 
				(!joueurId)?"0":(nbJoueurs > 1)?"1":"IA",
				(nObjets > 0 )?"\e[1;32mgagne\e[0m":"\e[1;31mperd\e[0m" );
			break;
		}
		else
		{
			printf ("Il reste %d allumette(s) !\n", nObjets );
		}
		joueurId = ( joueurId + 1 ) % 2 ;
	}
	while ( 1 );

	return ( 0 );
}
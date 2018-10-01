#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "lib/termRequest/request.h"


enum
{
	ONE,
	TWO
};

static int printPendu ( int id, int x, int y )
{
	setPosition ( x, y );
	printf ( "                 " );
	setPosition ( x + 1, y );
	printf ( "                 " );
	setPosition ( x + 2, y );
	printf ( "                 " );
	setPosition ( x + 3, y );
	printf ( "                 " );
	setPosition ( x + 4, y );
	printf ( "                 " );
	setPosition ( x + 5, y );
	printf ( "                 " );
	setPosition ( x + 6, y );
	printf ( "________________\n" );

	if ( id > 1 )
	{
		setPosition ( x + 6, y + 3 );
		printf ( "/|\\\n" );
	}

	if ( id > 2 )
	{
		setPosition ( x + 1, y + 4 );
		printf ( "|" );
		setPosition ( x + 2, y + 4 );
		printf ( "|" );
		setPosition ( x + 3, y + 4 );
		printf ( "|" );
		setPosition ( x + 4, y + 4 );
		printf ( "|" );
		setPosition ( x + 5, y + 4 );
		printf ( "|" );
	}

	if ( id > 3 )
	{
		setPosition ( x + 1, y + 4 );
		printf ( "+-------+" );
	}

	if ( id > 4 )
	{
		setPosition ( x + 2, y + 12 );
		printf ( "|" );
	}

	if ( id > 5 )
	{
		setPosition ( x + 3, y + 12 );
		printf ( "O" );
	}

	if ( id > 6 )
	{
		setPosition ( x + 4, y + 12 );
		printf ( "|" );
	}

	if ( id > 7 )
	{
		setPosition ( x + 4, y + 11 );
		printf ( "/" );
	}

	if ( id > 8 )
	{
		setPosition ( x + 4, y + 13 );
		printf ( "\\" );
	}

	if ( id > 9 )
	{
		setPosition ( x + 5, y + 12 );
		printf ( "'" );
	}

	if ( id > 10 )
	{
		setPosition ( x + 5, y + 11 );
		printf ( "/" );
	}

	if ( id > 11 )
	{
		setPosition ( x + 5, y + 13 );
		printf ( "\\" );
		return ( 1 );
	}

	return ( 0 );
}

int pendu ( void )
{
	FILE *f = NULL;
	int nbLines = 0;
	char c = 0;
	int i = 0;
	int result = 0;

	int nbCoups = 1;

	void * mask = NULL;

	char word[ 64 ] = { 0 };
	char cmd[ 256 ] = { 0 };
	uint64_t found = 0;

	srand ( time ( NULL ) );

	switch ( menu ( 2, "un joueur", "deux joueurs", NULL ) )
	{
		case ONE:
		{
			sprintf ( cmd, "NUM=$(( %d %% $(wc res/french.txt -l | cut -d ' ' -f1) )) ; sed -n ${NUM}p < res/french.txt", rand ( ) );
			f = popen ( cmd, "r" );
			if ( !f )
			{
				return ( __LINE__ );
			}

			if ( !fscanf ( f, "%63s", word ) )
			{
				fclose ( f );
				return ( __LINE__ );
			}

			fclose ( f );

			break;
		}
		case TWO:
		{
			printf ( "saisir le mot : " );
			getPassword ( word, 63, '*' );
			break;
		}
	}
	setPosition ( -2, 2 );
	printf ( "\e[2K\n\e[2K" );

	setBlockMode ( &mask, true );

	setPosition ( -2, 3 );
	printf ( "____________ \n" );
	
	setPosition ( -1, 3 );
	for ( i = 0; i < strlen ( word ); i++ )
	{
		printf ( "_" );
	}

	do
	{
		c = getchar ( );

		for ( i = 0; i < strlen ( word ); i++ )
		{
			if ( c == word[ i ] )
			{
				found |= 1 << i;
			}
		}

		if ( found == ( ( 1 << i ) - 1 ) )
		{
			setPosition ( -2, 3 );
			printf ( "mot trouvé en %d fois \n", nbCoups );

			setPosition ( -1, 3 );
			printf ( "%s \n", word );
			break;
		}

		setPosition ( -2, nbCoups + 2 );
		printf ( "%c", c );
	
		result = printPendu ( nbCoups++, -9, 2 );
		
		setPosition ( -1, 3 );

		for ( i = 0; i < strlen ( word ); i++ )
		{
			if ( found & ( 1 << i ) )
			{
				printf ( "%c", word[ i ] );
			}
			else
			{
				printf ( "_" );
			}
		}
		
		setPosition ( -2, 16 );
		printf ( "%d\n", nbCoups );
	}
	while ( !result );

	if ( result )
	{
		setPosition ( -2, 2 );
		printf ( " le mot était %s\n", word );
	}
	getchar ( );

	resetBlockMode ( mask );

	return ( result );
}
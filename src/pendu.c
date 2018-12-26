#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "lib/termRequest/request.h"

enum
{
	ONE,
	TWO
};

#ifdef _WIN32
#include <conio.h>
#endif


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
	int nbErrors = 0;

	void * mask = NULL;

	char word[ 64 ] = { 0 };
	char fileName[ 64 ] = { 0 };
	uint64_t found = 0;

	srand ( time ( NULL ) );


	// get one word
	switch ( menu ( 2, "un joueur", "deux joueurs", NULL ) )
	{
		case ONE:
		{
			printf ( "path du dictionnaire :\n" );
			scanf ( "%63s", fileName );
			while ( getchar ( ) != '\n' );
			
			f = fopen ( fileName, "r" );
			if ( !f )
			{
				printf ( "%s\n", strerror ( errno ) );
				return ( __LINE__ );
			}

			nbLines = 0;
			while ( fgets ( word, 63, f ) != NULL )
			{
				nbLines++;
			}

			if ( nbLines > 65535 )
			{
				nbLines = rand ( ) * rand ( ) % nbLines;
			}
			else
			{
				nbLines = rand ( ) % nbLines;
			}

			fseek ( f, 0, SEEK_SET );

			for ( i = 0; i < nbLines; i++ )
			{
				if ( !fgets ( word, 63, f ) )
				{
					fclose ( f );
					return ( __LINE__ );
				}
			}

			// remove \n and \r if they are existing
			while ( ( word[ strlen ( word ) - 1 ] == '\n' ) ||
				( word[ strlen ( word ) - 1 ] == '\r' ) )
			{
				word[ strlen ( word ) - 1 ] = '\0';
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

	// clean part of screen
	setPosition ( -2, 2 );
	#ifdef __linux__
	printf ( "\e[2K\n\e[2K" );
	#endif

	setBlockMode ( &mask, true );

	// print error line
	setPosition ( -2, 3 );
	printf ( "____________ \n" );
	
	// print word line
	setPosition ( -1, 3 );
	for ( i = 0; i < strlen ( word ); i++ )
	{
		if ( ( word[ i ] == ' ' ) ||
			( word[ i ] == '-' ) )
		{
			printf ( "%c", word[ i ] );
			found |= 1 << i;
		}
		else
		{
			printf ( "_" );
		}
	}

	do
	{
		#ifdef __linux__
		c = getchar ( );
		#elif _WIN32
		c = _getch ( );
		#endif

		result = 1;
		for ( i = 0; i < strlen ( word ); i++ )
		{
			if ( c == word[ i ] )
			{
				found |= 1 << i;
				result = 0;
			}
		}

		if ( result )
		{
			nbErrors++;
			setPosition ( -2, nbErrors + 2 );
			printf ( "%c", c );
		}

		if ( found == ( ( 1 << i ) - 1 ) )
		{
			setPosition ( -2, 3 );
			printf ( "mot trouve en %d fois \n", nbCoups );

			setPosition ( -1, 3 );
			printf ( "%s \n", word );
			break;
		}


		nbCoups++;

		result = printPendu ( nbErrors, -9, 2 );
		
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
		printf ( " le mot etait %s\n", word );
	}
	getchar ( );

	resetBlockMode ( mask );

	return ( result );
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib/termRequest/request.h"
 
#define ROWS 6
#define COLUMNS 7
 
static int addInGrid ( char Grid[ ROWS ][ COLUMNS ], int colonne, char x )
{
	for ( int i = 0; i < ROWS; i++ )
	{
		if ( Grid[ i ][ colonne ] == ' ' )
		{
			Grid[ i ][ colonne ] = x;
			return ( 0 );
		}
	}
	return ( __LINE__ );
}

static void printGrid ( char Grid[ ROWS ][ COLUMNS ] )
{
	printf ( "\e[%dA\r-1-2-3-4-5-6-7- \n", ROWS + 3 );
	for ( int i = ROWS - 1; i >= 0; i-- )
	{
		printf ( "|" );
		for ( int j = 0; j < COLUMNS; j++ )
		{
			printf ( "%c|",  Grid[ i ][ j ] );
		}
		printf ( " \n" );
	}
	printf ( "--------------- \n\e[2K\r" );
}

static char isWon ( char Grid[ ROWS ][ COLUMNS ] )
{
	for ( int i = 0; i < ROWS; i++ )
	{
		for ( int j = 0; j < COLUMNS; j++ )
		{
			if ( Grid[ i ][ j ] == ' ' )
			{
				continue;
			}

			if ( ( j < COLUMNS - 3 ) &&
				( Grid[ i ][ j ] == Grid[ i ][ j + 1 ] ) &&
				( Grid[ i ][ j ] == Grid[ i ][ j + 2 ] ) &&
				( Grid[ i ][ j ] == Grid[ i ][ j + 3 ] ) )
			{
				return ( Grid[ i ][ j ] );
			}

			if ( ( i < ROWS - 3 ) &&
				( Grid[ i ][ j ] == Grid[ i + 1 ][ j ] ) &&
				( Grid[ i ][ j ] == Grid[ i + 2 ][ j ] ) &&
				( Grid[ i ][ j ] == Grid[ i + 3 ][ j ] ) )
			{
				return ( Grid[ i ][ j ] );
			}

			if ( ( i < ROWS - 3 ) &&
				( j < COLUMNS - 3 ) &&
				( Grid[ i ][ j ] == Grid[ i + 1 ][ j + 1 ] ) &&
				( Grid[ i ][ j ] == Grid[ i + 2 ][ j + 2 ] ) &&
				( Grid[ i ][ j ] == Grid[ i + 3 ][ j + 3 ] ) )
			{
				return ( Grid[ i ][ j ] );
			}
		}
	}
	return ( 0 );
}

int puissance4 ( )
{
	char letter[2] = { 'o', 'x' };
	int i = 0;

	char Grid [ROWS][COLUMNS] = {
		{ ' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ' }
	};

	int position;
	int x, y;
	void *ptr = NULL;;

	getSize ( &x, &y );

	if ( ( x < ( ROWS + 3 ) ) ||
		( y < COLUMNS ) )
	{
		printf ( "screen too small\n" );
		return ( __LINE__ );
	}

	do
	{
		printGrid ( Grid );
		printf ( "Position jeton : " );
		do
		{
			scanf ( "%d", &position );
			while ( getchar ( ) != '\n' );
		}
		while ( ( position > COLUMNS ) &&
			( position < 1 ) );

		position-=1;

		if ( addInGrid ( Grid, position % COLUMNS, letter[ i++ & 0x01 ]  ) )
		{
			printf ( "colonne pleinne\n" );
			return ( __LINE__ );
		}

		position = isWon ( Grid );
		if ( position )
		{
			printGrid ( Grid );
			printf ( "\e[5A\e[1;32m game won by %c \e[0m\e[5B\r", position );
			return ( 0 );
		}
	}
	while ( 1 );
}

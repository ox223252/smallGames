#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "arena.h"

#include "lib/termRequest/request.h"

#define VERT "\e[42m"
#define GRIS "\e[47m"
#define JAUNE "\e[43m"
#define RED "\e[41m"
#define CYAN "\e[46m"
#define NORMAL "\e[0m"

typedef struct
{
	uint8_t discovered:1,
		wood:1,	
		stone:1,
		gold:1,
		food:1,
		water:1;
}
arenaCell;

typedef struct
{
	int line;
	int col;
	uint8_t wood;
	uint8_t stone;
	uint8_t gold;
	uint8_t food;
	uint8_t water;
}
arenaUser;

static void clearScreen ( int sizeX )
{
	printf ( "\e[1;1H" );
	while ( sizeX-- )
	{
		printf ( "\e[2K\n" );
	}
}

static void printArena ( const arenaCell ** const table, int lines, int cols, const arenaUser user[] )
{
	int height = 0;
	int width = 0;

	printf ( "\e[1;1H" );
	for ( height = 0; height < lines; height++ )
	{
		for ( width = 0; width < cols; width++ )
		{
			if ( ( height == user[ 0 ].line ) &&
				( width == user[ 0 ].col  ) )
			{
				printf ( "@" );
			}
			else switch ( *(uint8_t *)&table[ height ][ width ] )
			{
				case 0x00:
				{
					printf ( "#" );
					break;
				}
				case 0x01:
				{
					printf ( " " );
					break;
				}
				case 0x03:
				{
					printf ( VERT" "NORMAL );
					break;
				}
				case 0x05:
				{
					printf ( GRIS" "NORMAL );
					break;
				}
				case 0x09:
				{
					printf ( JAUNE" "NORMAL );
					break;
				}
				case 0x11:
				{
					printf ( RED" "NORMAL );
					break;
				}
				case 0x21:
				{
					printf ( CYAN" "NORMAL );
					break;
				}
				default:
				{
					printf ( "c'est quoi ce bordel %x\n",  *(uint8_t *)&table[ height ][ width ] );
				}
			}
		}
	}
}

static void discoverArena ( arenaCell ** const table, int height, int width, const arenaUser user[] )
{
	int i = 0;
	int l;
	int c;
	int new = 0;

	while ( ( user[ i ].line > 0 ) && 
		( user[ i ].col > 0 ) )
	{
		for ( l = -1; l <= 1 ; l++ )
		{
			for ( c = -1; c <= 1; c++ )
			{
				if ( ( c == 0 ) &&
					( l == 0 ) )
				{ // place is already know
					continue;
				}
				if ( ( user[ i ].line + l < 0 ) ||
					( user[ i ].line + l >= height ) ||
					( user [ i ].col + c < 0 ) ||
					( user [ i ].col + c >= width ) )
				{ // user is on map border
					continue;
				}

				if ( !table[ user[ i ].line + l ][ user [ i ].col + c ].discovered )
				{ // place is not discovered
					table[ user[ i ].line + l ][ user [ i ].col + c ].discovered = true;
					new = rand() % 100;
					if ( new < 2 )
					{
						table[ user[ i ].line + l ][ user [ i ].col + c ].stone = true;
					}
					else if ( new < 4 )
					{
						table[ user[ i ].line + l ][ user [ i ].col + c ].gold = true;
					}
					else if ( new < 10 )
					{
						table[ user[ i ].line + l ][ user [ i ].col + c ].wood = true;
					}
					else if ( new < 16 )
					{
						table[ user[ i ].line + l ][ user [ i ].col + c ].water = true;
					}
					else if ( new < 22 )
					{
						table[ user[ i ].line + l ][ user [ i ].col + c ].food = true;
					}
				}
			}
		}
		i++;
	}
}

static void getElement ( arenaCell ** const table, arenaUser user[] )
{
	int i = 0;
	for ( i = 0; user[ i ].line > 0 && user[ i ].col > 0; i++ )
	{
		switch ( *(uint8_t *)&table[ user[ i ].line ][ user[ i ].col ] )
		{
			case 0x00:
			case 0x01:
			{
				break;
			}
			case 0x03:
			{
				user[ i ].wood++;
				table[ user[ i ].line ][ user[ i ].col ].wood = false;
				break;
			}
			case 0x05:
			{
				user[ i ].stone++;
				break;
			}
			case 0x09:
			{
				user[ i ].gold++;
				table[ user[ i ].line ][ user[ i ].col ].gold = false;
				break;
			}
			case 0x11:
			{
				user[ i ].food += 10;
				table[ user[ i ].line ][ user[ i ].col ].food = false;
				break;
			}
			case 0x21:
			{
				user[ i ].water += 3;
				break;
			}
		}
	}
}

void arena ( void )
{
	arenaCell * table1 = NULL;
	arenaCell ** table = NULL;
	arenaUser * user = NULL;

	int cols = 0;
	int lines = 0;

	int i = 0;

	getSize ( &lines, &cols );
	lines--;

	srand ( time ( NULL ) );   // should only be called once

	table1 = malloc ( lines * cols );
	if ( !table1 )
	{
		goto errorTable1;
	}
	table = malloc ( sizeof ( *table ) * lines );
	if ( !table )
	{
		goto errorTable;
	}
	for ( i = 0; i < lines; i++ )
	{
		table[ i ] = &table1[ i * cols ];
	}
	memset ( &table[ 0 ][ 0 ], 0x00, cols * lines );

	user = malloc ( sizeof ( *user) * 2 );
	if ( !user )
	{
		goto errorUser;
	}

	user[ 0 ].line = 5;
	user[ 0 ].col = 5;
	user[ 0 ].food = 100;
	user[ 0 ].water = 100;

	user[ 1 ].line = -1;
	user[ 1 ].col = -1;

	table[ 5 ][ 5 ].discovered = true;

	while ( 1 )
	{
		switch ( getMovePad ( true ) )
		{
			case KEYCODE_ESCAPE:
			{
				goto end;
				break;
			}
			case KEYCODE_UP:
			{
				if ( user[ 0 ].line > 0 )
				{
					user[ 0 ].line--;
				}
				break;
			}
			case KEYCODE_LEFT:
			{
				if ( user[ 0 ].col > 0 )
				{
					user[ 0 ].col--;
				}
				break;
			}
			case KEYCODE_DOWN:
			{
				if ( user[ 0 ].line < ( lines - 1 ) )
				{
					user[ 0 ].line++;
				}
				break;
			}
			case KEYCODE_RIGHT:
			{
				if ( user[ 0 ].col < ( cols - 1 ) )
				{
					user[ 0 ].col++;
				}
				break;
			}
			case KEYCODE_SPACE:
			{
				getElement ( table, user );
				break;
			}
			case KEYCODE_NONE:
			{
				break;
			}
		}

		clearScreen ( lines + 1 );
		discoverArena ( table, lines, cols, user );
		printArena ( ( const arenaCell ** const )table, lines, cols, user );
		
		setPosition ( lines + 1, 1 );

		printf ( "f %3d / w %3d / s %3d / g %3d / w %3d", 
			--user[ 0 ].food, 
			--user[ 0 ].water,
			user[ 0 ].stone, 
			user[ 0 ].gold, 
			user[ 0 ].wood );

		if ( ( user[ 0 ].food == 0 ) ||
			( user[ 0 ].water == 0 ) )
		{
			printf ( "\r\e[1;31mYou're dead\e[0m \n" );
			break;
		}
	}


end:

	free ( user );
errorUser:
	free ( table );
errorTable:
	free ( table1 );
errorTable1:
	table = NULL;
}
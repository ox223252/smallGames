#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/termRequest/request.h"

enum
{
	VIDE = 0,
	TETE,
	CORP,
	QUEUE,
	FRUIT
};

typedef struct _snake_t
{
	int x;
	int y;
	struct _snake_t * next;
}
snake_t;

typedef struct
{
	int x;
	int y;
}
position_t;

static void clearScreen ( int sizeX );
static void printSnake ( snake_t * const snake );
static int moveSnake ( snake_t * const snake, const int mvt, position_t * const fruit, int cols, int lines );
static int isInSnake ( snake_t * const snake, int x, int y );
static void freeSnake ( snake_t * const snake );
static int sizeOfSnake ( snake_t * const snake );
static int initSnake ( snake_t ** const snake, int x, int y );


static void clearScreen ( int sizeX )
{
	printf ( "\e[1;1H" );
	while ( sizeX-- )
	{
		printf ( "\e[2K\n" );
	}
}

static void printSnake ( snake_t * const snake )
{
	printf ( "\e[%d;%dH%c", snake->x, snake->y, ( snake->next )? '#' : '-' );
	if ( snake->next )
	{
		printSnake ( snake->next );
	}
}

static int moveSnake ( snake_t * const snake, const int mvt, position_t * const fruit, int cols, int lines )
{
	snake_t * tmp = NULL;
	if ( ( snake->next == NULL ) &&
		( mvt == 0 ) )
	{
		return ( 0 );
	}
	else if ( snake->next )
	{
		moveSnake ( snake->next, 0, fruit, cols, lines );

		snake->next->x = snake->x;
		snake->next->y = snake->y;
	}

	if ( !mvt )
	{
		return ( 0 );
	}

	if ( isInSnake ( snake, fruit->x, fruit->y ) )
	{
		tmp = snake;

		while ( tmp->next )
		{
			tmp = tmp->next;
		}

		initSnake ( &tmp->next, fruit->x, fruit->y );


		fruit->x = -1;
		fruit->y = -1;
		return ( 0 );
	}

	switch ( mvt )
	{
		case 0x41: // up
		{
			snake->x--;
			if ( snake->x <= 0 )
			{
				snake->x = lines;
			}
			break;
		}
		case 0x42: // down
		{
			snake->x++;
			if ( snake->x > lines )
			{
				snake->x = 1;
			}
			break;
		}
		case 0x43: // right
		{
			snake->y++;
			if ( snake->y > cols )
			{
				snake->y = 1;
			}
			break;
		}
		case 0x44: // left
		{
			snake->y--;
			if ( snake->y <= 0 )
			{
				snake->y = cols;
			}
			break;
		}
	}

	return isInSnake ( snake->next, snake->x, snake->y );
}

static int isInSnake ( snake_t * const snake, int x, int y )
{
	if ( !snake )
	{
		return ( 0 );
	}

	if ( ( snake->x == x ) &&
		( snake->y == y ) )
	{
		return ( 1 );
	}
	else if ( snake->next )
	{
		return ( isInSnake ( snake->next, x, y ) );
	}
	else return ( 0 );
}

static int sizeOfSnake ( snake_t * const snake )
{
	if ( !snake )
	{
		return ( 0 );
	}
	if ( snake->next )
	{
		return ( 1 + sizeOfSnake ( snake->next ) );
	}
	else
	{
		return ( 1 );
	}
}

static void freeSnake ( snake_t * snake )
{
	snake_t * tmp;

	do
	{
		tmp = snake->next;
		free ( snake );
		snake = tmp;
	}
	while ( snake );
}

static int initSnake ( snake_t ** const snake, int x, int y )
{
	if ( !snake )
	{
		return ( __LINE__ );
	}

	*snake = malloc ( sizeof ( snake_t ) );
	(*snake)->x = x;
	(*snake)->y = y;
	(*snake)->next = NULL;

	return ( 0 );
}

void snake ( void )
{
	void * mask = NULL;

	int cols = 0;
	int lines = 0;
	uint32_t speed = 200000;

	int i = 0;
	int mvt;

	snake_t * snake = NULL;
	position_t fruit = { };

	char * menuItem[] = {
		"start normal game",
		"set size",
		"set speed",
		"speed chnage",
		"start game",
		NULL
	};

	struct
	{
		uint8_t speed:1,
			start:1;
	}
	flag = { 0 };

	srand ( time ( NULL ) );   // should only be called once
	getSize ( &lines, &cols );
	
	do
	{
		flag.start = false;

		switch ( menu ( 0, menuItem, NULL ) )
		{
			case 0:
			{
				getSize ( &lines, &cols );
				speed = 200000;
				flag.start = true;
				break;
			}
			case 1:
			{
				printf("lines (default : %d)\n", lines );
				scanf ( "%u", &lines );
				getchar ( );
				printf("cols (default : %d)\n", cols );
				scanf ( "%u", &cols );
				getchar ( );
				break;
			}
			case 2:
			{
				printf ( "set speed in ms\n" );
				scanf ( "%u", &speed );
				speed *= 1000;
				getchar ( );
				break;
			}
			case 3:
			{
				flag.speed = true;
				break;
			}
			case 4:
			{
				flag.start = true;
				break;
			}
		}
	}
	while ( !flag.start );

	lines--;

	initSnake ( &snake, rand ( ) % lines + 1, rand ( ) % cols + 1 );

	fruit.x = rand ( ) % ( lines - 1 ) + 1;
	fruit.y = rand ( ) % ( cols - 1 ) + 1;

	if ( setBlockMode ( &mask, true ) )
	{
		return;
	}
	setGetCharTimeOut ( 0, 0 );

	mvt = 0;

	while ( 1 )
	{
		if ( getchar ( ) == 0x1b )
		{
			if ( getchar ( ) > 0 )
			{
				mvt = getchar ( );
			}
			else
			{
				break;
			}
		}

		if ( moveSnake ( snake, mvt, &fruit, cols, lines ) )
		{
			break;
		}

		if ( ( fruit.x < 0 ) &&
			( fruit.y < 0 ) )
		{
			fruit.x = rand ( ) % ( lines - 1 ) + 1;
			fruit.y = rand ( ) % ( cols - 1 ) + 1;

			if ( ( flag.speed == true ) &&
				( speed > 10000 ) );
			{
				speed -= 10000;
			}
		}

		clearScreen ( lines );
		printf ( "\e[%d;%dH@", fruit.x, fruit.y );
		printSnake ( snake );

	
		printf ( "\e[%d;1Hsnake size : %3d / speed : %3ums\e[%d;%dH", lines + 1, sizeOfSnake ( snake ), speed / 1000, lines + 1, cols );
		while ( getchar ( ) > 0 );

		usleep ( speed );
	}
	setGetCharTimeOut ( 0, 1 );
	
	printf ( "\n" );

	resetBlockMode ( mask );


	freeSnake ( snake );
	snake = NULL;
}
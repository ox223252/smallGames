#include <stdio.h>

#include "lib/termRequest/request.h"
#include <time.h>
#include <stdlib.h>

#include "pendu.h"
#include "snake.h"
#include "puissance4.h"
#include "arena.h"
#include "nim.h"

enum
{
	NEW_PENDU,
	NEW_SNAKE,
	PUISSANCE_4,
	NEW_ARENA,
	NEW_NIM,
	QUIT
};

int main ( int argc, char * argv )
{
	char * tableMenu [] = {
		"Pendu",
		"Snake",
		"Puissance 4",
		"Arena",
		"Nim",
		"Quit",
		NULL
	};

	srand ( time ( NULL ) );
	
	do
	{
		switch ( menu ( 0, tableMenu, NULL ) )
		{
			case NEW_PENDU:
			{
				pendu ( );
				break;
			}
			case NEW_SNAKE:
			{
				snake ( );
				break;
			}
			case PUISSANCE_4:
			{
				puissance4 ( );
				break;
			}
			case NEW_ARENA:
			{
				arena ( );
				break;
			}
			case NEW_NIM:
			{
				nim ( );
				break;
			}
			case QUIT:
			{
				return ( 0 );
			}
		}
	}
	while ( 1 );

	return ( 0 );
}

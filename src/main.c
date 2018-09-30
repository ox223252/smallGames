#include <stdio.h>

#include "lib/termRequest/request.h"
#include "pendu.h"
#include "snake.h"
#include "puissance4.h"
#include "arena.h"

enum
{
	NEW_PENDU,
	NEW_SNAKE,
	NEW_ARENA,
	PUISSANCE_4,
	QUIT
};

int main ( int argc, char * argv )
{
	char * tableMenu [] = {
		"Pendu",
		"Snake",
		"Puissance 4",
		"Arena",
		"Quit",
		NULL
	};
	
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
			case QUIT:
			{
				return ( 0 );
			}
		}
	}
	while ( 1 );

	return ( 0 );
}

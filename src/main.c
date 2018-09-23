#include <stdio.h>

#include "lib/termRequest/request.h"
#include "pendu.h"
#include "snake.h"

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
			case QUIT:
			{
				return ( 0 );
			}
		}
	}
	while ( 1 );

	return ( 0 );
}

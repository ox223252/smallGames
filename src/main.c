#include <stdio.h>

#include "lib/termRequest/request.h"
#include "pendu.h"

enum
{
	NEW_PENDU,
	QUIT
};

int main ( int argc, char * argv )
{
	char * tableMenu [] = {
		"Pendu",
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
			case QUIT:
			{
				return ( 0 );
			}
		}
	}
	while ( 1 );

	return ( 0 );
}

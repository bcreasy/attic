/* Brian Creasy APCS Period 9 Towers of Hanoi this program will solve the
towers of hanoi problem utilizing recursion *shudders*

	TODO:
			clean up code
			change user message cout's
			multiple runs?
*/

#include <stdio.h>

void hello();
void getdata( int *disks, int *start, int *end );
int hanoi( int disks, int start, int end );
void GoodBye();

int main()
{
	// main function
	int moves;
	int disks;
	int start;
	int end;

	hello();

	getdata( &disks, &start, &end );
	moves = hanoi( disks, start, end );

	printf( "\ntotal moves: %d", moves );

	GoodBye();

	return 0;
}

void hello()
{
	// function will greet the user in a courteous manner
	printf( "yo, this is my towers of h4n0i program\n\n" );
}

void getdata( int *disks, int *start, int *end )
{
	// function will retreive user input for how many disks there should be

	do
	{
		do
		{
			printf( "which peg do you want to start on? (1, 2, or 3): " );
			scanf("%d", start );
		}	while ( !( *start == 1 || *start == 2 || *start == 3 ) );

		do
		{
			printf( "where do you want to end up? (1, 2, or 3): " );
			scanf("%d",  end );
		}   while ( !( *start == 1 || *start == 2 || *start == 3 ) );

		if ( *start == *end )
			printf( "you dumbass, pick different places to start and end up.\n\n" );
	}	while ( start == end );

	do
	{
		printf( "how many disks do you want? (the more disks, the longer it takes): " );
		scanf("%d", disks );
		if ( *disks < 3 )
			printf( "please enter a number greater than 2...\n" );
		if ( *disks > 10 )
			printf( "\nuhm.. ok, but this is going to take a while...\n\n" );
		else
			printf( "\n" );
	}	while ( *disks < 3 );
}

int hanoi ( int disks, int start, int end )
{
	static int moves = 0;
	if ( disks == 1 )
	{
		printf( "move disk from %d to peg %d\n", start, end );
		moves++;
		return moves;
	}
	else
	{
		int empty = 6 - end - start;
		hanoi ( disks - 1, start, empty );
		printf( "move disk from %d to peg %d\n", start, end );
		hanoi ( disks - 1, empty, end );
		moves++;
		return moves;
	}
}

void GoodBye()
{
	// function will say byebye
	printf( "\n\nexiting...\n" );
}



/*********************************************\
* phone encryption/decryption program         *
* coded by brian creasy                       *
* may 7, 2002 - distribute under BSD liscence *
\*********************************************/

#include <iostream.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>
using namespace std;

int xtable[26] = { 21, 22, 23, 31, 32, 33, 41, 42, 43, 51, 52, 53, 61, 62, 63, 71, 72, 73, 74, 81, 82, 83, 91, 92, 93, 94 };

void usage( char *cmd );
void encrypt( void );
void decrypt( void );
int getindex( char val );

int main( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		usage( argv[0] );
		exit(1);
	}

	if ( *argv[1] == '1' )
		encrypt();
	else
	if ( *argv[1] == '2' )
		decrypt();
	else
	{
		usage( argv[0] );
		exit(1);
	}
	
	return 0;
}

void usage( char *cmd )                                 
{                                                             
    cout << "Usage: " << cmd << " <1|2>\n";
	cout << "  1 - encrypt\n";
	cout << "  2 - decrypt\n";
}                                                             

void encrypt( void )
{
	int length;
	int charnum;
	char string[120];

	cin.getline( string, 120 );

	length = strlen( string );

	for( int i=0 ; i<length ; i++ )
	{
		string[i] = toupper( string[i] );
		charnum = string[i] - 65;
		if ( charnum == -33 )
			cout << "00";
		else
			cout << xtable[charnum];
	}
	cout << endl;
}

void decrypt( void )
{
	// ex: 81424374004374002100338262007153212332008163002232 = "this is a fun place to be"
	int length;
	int j=0;
	string m;
	string n;
	string b;
	getline( cin, b );
	b = string( b );

	length = b.length();
	int newarray[length/2];

	string::const_iterator i;
	
	for( i = b.begin() ; i != b.end() ; i+=2 )
	{
		m = *i;
		n = *( i + 1 );
		newarray[j] = ( atoi( m.c_str() ) * 10 );
		newarray[j] += ( atoi( n.c_str() ) );
		j++;
	}
	for( int j=0 ; j<(length/2) ; j++ )
	{
		if ( newarray[j] == 0 )
			cout << " ";
		else
			cout << ( char( getindex( newarray[j] ) + 65 ) );
	}
	cout << endl;
}

int getindex( char val )
{
	for( int i=0 ; i<26 ; i++ )
		if ( xtable[i] == val )
			return i;
}

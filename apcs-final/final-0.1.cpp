/*
	Brian Creasy
	APCS Period 9
	Final: Fly the Friendly Skies
	This program is a fully-functional flight record keeper.  It enables the
		user to display passengers, add/remove passengers, sort them, modify
		and eventually quit and re-write the data back to the data file.
*/

#include <ctype.h>
#include <fstream.h>
#include <iomanip.h>
#include <iostream.h>
#include "include/apstring.h"
#include "include/apvector.h"
#include "include/fileopen.h"

const int MAXPASS= 40;

struct passenger {
	apstring first;
	apstring last;
	int row;
	char seat;
	bool firstclass;
};

void loadFlightList( apstring filename, apvector <passenger> &flightlist );
bool mainmenu( apvector <passenger> &flightlist, apstring filename );
void printmenu();
void printHeader();
void printClass( bool cls );
apstring cleanstr( apstring name );
void displayFlight( apvector <passenger> flightlist );
int findPassenger( apvector <passenger> flightlist, apstring first, apstring last );
void displayPassenger( apvector <passenger> flightlist );
void numsection( apvector <passenger> flightlist );
void swap( passenger &one, passenger &two );
void sort( apvector <passenger> &flightlist );
int findmin( apvector <passenger> &flightlist, int start, int &min );

ostream &operator << ( ostream &outplace, passenger &pass );

int main(void)
{
	apstring filename="passengers";
	apvector <passenger> flightlist( MAXPASS );

//	Hello();

	loadFlightList( filename, flightlist );

	do{} while( !( mainmenu( flightlist, filename ) ) );

//	GoodBye();

	return 0;
}

void loadFlightList( apstring filename, apvector <passenger> &flightlist )
{
	// this function will read in the data from the specified file
	
	ifstream infile;
	int i=0;
	
	OpenFileForReading( infile, filename );
	
	while ( i < MAXPASS && infile >> flightlist[i].first >>
			flightlist[i].last >> flightlist[i].row >> flightlist[i].seat )
	{
		if ( flightlist[i].row < 11 )
			flightlist[i].firstclass= true;
		else
			flightlist[i].firstclass= false;
		infile.ignore( 80, '\n' ); // skip the rest of the line
		i++;
	}

	cout << i << " passengers on flight\n\n";
	flightlist.resize( i );
	infile.close();
	sort( flightlist );
}

bool mainmenu( apvector <passenger> &flightlist, apstring filename )
{
	// main menu

	char command;
	ofstream outfile;
	OpenFileForWriting( outfile, filename );
	
	printmenu();
	do
	{
		cout << "\nPlease enter a command (H to print menu): ";
		cin >> command;
		cin.ignore( 100, '\n' );
		command= tolower( command );

		switch( command )
		{
			case 'l' : displayFlight( flightlist ); break;
			case 'd' : displayPassenger( flightlist ); break;
			case 'q' :
				for( int i=0 ; i<flightlist.length() ; i++ )
					outfile << flightlist[i];
				return true; break;
			case 'v' : numsection( flightlist ); break;
			//case 'c' : changeSeat( flightlist ); break;
			//case 'a' : addPassenger( flightlist ); break;
			//case 'r' : removePassenger( flightlist ); break;
			case 'h' : break;
			default  : cout << "\n'" << command << "' is not a valid command.\n"; break;
		}
	} while ( command != 'h' );
	return false;
}

void printmenu()
{
	// prints the main menu to the screen
	cout << endl;
	cout << "Main Menu\n";
	cout << "---------\n";
	cout << "L :: Print all the passengers on the flight\n";
	cout << "D :: Search for a passenger by name\n";
	cout << "V :: Display number of passengers in a section\n";
	cout << "C :: Change a passenger's seat assignment\n";
	cout << "A :: Add a new passenger to the flight\n";
	cout << "R :: Remove a passenger fromt the flight\n";
	cout << "Q :: Quit the flight manager\n\n";
	cout << "H :: Display this menu\n";
}

void displayFlight( apvector <passenger> flightlist )
{
	int i;
	int n= flightlist.length();

	cout.setf(ios::left);
	
	printHeader();
	for( i=0 ; i<n ; i++ )
	{
		cout << flightlist[i];
		printClass( flightlist[i].firstclass );
	}
}

void displayPassenger( apvector <passenger> flightlist )
{
	// choice D
	int p;
	apstring first, last;

	cout << "\nPlease enter a name: ";
	cin >> first >> last;
	first= cleanstr( first );
	last= cleanstr( last );

	p= findPassenger( flightlist, first, last );
	if ( p == -1 )
		cout << endl << first << " " << last << " is not on this flight\n";
	else
	{
		printHeader();
		cout << flightlist[p];
		printClass( flightlist[p].firstclass );
	}
}

int findPassenger( apvector <passenger> flightlist, apstring first, apstring last )
{
	// goes through the flightlist and finds the first and last name index
	// returns -1 if not found
	int n= flightlist.length();
	for( int i=0 ; i<n ; i++ )
	{
		flightlist[i].first= cleanstr( flightlist[i].first );
		flightlist[i].last= cleanstr( flightlist[i].last );
		if ( flightlist[i].first == first && flightlist[i].last == last )
			return i;
	}

	return -1;
}

void numsection( apvector <passenger> flightlist )
{
	int count=0;
	apstring sect;
	int n= flightlist.length();
	bool flag;

	do
	{
		cout << "\nPlease enter the class (firstclass or coach): ";
		getline( cin, sect );
		
		if ( sect == "firstclass" )
			flag= true;
		else
		if ( sect == "coach" )
			flag= false;
		else
			cout << "Please enter either \"firstclass\" or \"coach\"\n";
	} while( !( sect == "firstclass" || sect == "coach" ) );

	for( int i=0 ; i<n ; i++ )
		if ( flag == flightlist[i].firstclass )		// compares the firstclass variable with
													// the flag set in the switch statement to see which
													// class we should count
			count++;
	if ( count == 0 )
		cout << "There are no passengers flying in this class\n";
	else
		cout << "There are " << count << " passengers flying in " << sect << endl;
}

ostream &operator << ( ostream &outplace, passenger &pass )
{
	outplace.setf(ios::left);
	if ( &outplace != &cout )	// if we're printing the struct out to a file
	{
        outplace << setw(13) << pass.first << setw(13) << pass.last;
		outplace << setw(7) << pass.row << setw(7) << pass.seat << endl;
	}
	else	// if its going to standard output
	{
		outplace << setw(13) << pass.last << setw(13) << pass.first;
		outplace << setw(7) << pass.row << setw(7) << pass.seat;
	}			 
	return outplace;
}

void printHeader()
{
	// prints the titles for the printouts
	cout << "\nLastname     Firstname    Row    Seat    Section\n";
	cout << "________________________________________________\n";
}

void printClass( bool cls )
{
	if ( cls == true )
		cout << "       First Class\n";
	else
		cout << "       Coach\n";
}

apstring cleanstr( apstring name )
{
	int i;
	int n= name.length();
	for( i=0 ; i<n ; i++ )
		name[i]= tolower( name[i] );
	
	return name;
}

void sort( apvector <passenger> &flightlist )
{
	int i, min=0;
	int n= flightlist.length();
	for( i=0 ; i<n ; i++ )
		swap( flightlist[i], flightlist[ findmin( flightlist, i, min ) ] );
}

void swap( passenger &one, passenger &two )
{
	passenger temp;

	temp= one;
	one= two;
	two= temp;
}

int findmin( apvector <passenger> &flightlist, int start, int &min )
{
	int i;
	int n= flightlist.length();
	min= start;
	
	for( i=start+1 ; i<n ; i++ )
	{
		if ( flightlist[i].last < flightlist[min].last )
			min= i;
		if ( flightlist[i].last == flightlist[min].last )
			if ( flightlist[i].first < flightlist[min].first )
				min= i;
	}
	return min;
}

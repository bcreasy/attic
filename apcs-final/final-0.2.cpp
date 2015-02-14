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
void changeSeat( apvector <passenger> &flightlist );
void seat( passenger &pass, apvector <passenger> &flightlist );
bool seatAvail( int row, char seat, apvector <passenger> &flightlist );
void addPassenger( apvector <passenger> &flightlist );
void addnew( apstring first, apstring last, apvector <passenger> &flightlist );
void removePassenger( apvector <passenger> &flightlist );
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
			case 'c' : changeSeat( flightlist ); break;
			case 'a' : addPassenger( flightlist ); break;
			case 'r' : removePassenger( flightlist ); break;
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
	cout << "R :: Remove a passenger from the flight\n";
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
	cin.ignore( 80, '\n' );

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

void changeSeat( apvector <passenger> &flightlist )
{
	int p;
	char ch;
	apstring first, last;

	cout << "\nPlease enter the name to change seats: ";
	cin >> first >> last;
	first= cleanstr( first );
	last= cleanstr( last );
	
	cin.ignore( 80, '\n' );
	
	p= findPassenger( flightlist, first, last );
	if ( p == -1 )
	{
		cout << endl << first << " " << last << " is not on this flight\n";
    	do
		{
			cout << "would you like to add " << first << " " << last << " to the flight?\n(y or n): ";
			cin >> ch;
			ch= tolower( ch );
			cin.ignore( 80, '\n' );
			if ( !( ch == 'y' || ch == 'n' ) )
				cout << ch << " is not a valid choice\n\n";
		} while ( !( ch == 'y' || ch == 'n' ) );
		if ( ch == 'y' )
			addnew( first, last, flightlist );
	}
	else
	{
		cout << endl << flightlist[p].first << " "<< flightlist[p].last << "'s current current assignment is:\n";
		cout << '\t' << " row #: " << flightlist[p].row << endl;
        cout << '\t' << "seat #: " << flightlist[p].seat << endl;
		seat( flightlist[p], flightlist );
	}
}

void seat( passenger &pass, apvector <passenger> &flightlist )
{
	int row;
	char seat;
	bool avail;
	do
	{
		do
		{
			cout << "what row # would you like to give " << pass.first << " " << pass.last << "? ";
			cin >> row;
			cin.ignore( 80, '\n' );
			if ( !(isdigit( row )) && ( row > 30 ) ) 
				cout << "please enter an integer 30 or less (there are only 30 rows on the plane)\n\n";
		} while( !(isdigit( row )) && ( row > 30 ) );
		do
		{
			cout << "what seat letter would you like to give " << pass.first << " "<< pass.last << "? ";
			cin >> seat;
			cin.ignore( 80, '\n' );
			seat= toupper( seat );
			if ( (!(isalpha( seat ))) || !( seat=='A' || seat=='B' || seat=='C' ||
						seat=='D' || seat=='E' || seat=='F' ) )
				cout << "please enter a letter A thru F\n\n";
		} while( (!(isalpha( seat ))) || !( seat=='A' || seat=='B' || seat=='C' || 
						seat=='D' || seat=='E' || seat=='F' ) );
		
		avail= seatAvail( row, seat, flightlist);
		if ( avail )
		{
        	if ( row < 11 )
				pass.firstclass= true;
			else
				pass.firstclass= false;
			pass.row= row;
			pass.seat= seat;
		}
		else
			cout << "that seat is already taken\n\n";
	} while ( avail= false );
}

bool seatAvail( int row, char seat, apvector <passenger> &flightlist )
{
	int n= flightlist.length();
	for( int i=0 ; i<n ; i++ )
		if ( flightlist[i].row == row && flightlist[i].seat == seat )
			return false;
	return true;
}

void addPassenger( apvector <passenger> &flightlist )
{
	int p;
	apstring first, last;

	cout << "\nPlease enter the name to add: ";
	cin >> first >> last;
	first= cleanstr( first );
	last= cleanstr( last );

	cin.ignore( 80, '\n' );

	p= findPassenger( flightlist, first, last );
	if ( p != -1 )
	{
		cout << "This passenger already exists\n\n";
		return;
	}

	addnew( first, last, flightlist);
	sort( flightlist );
}

void addnew( apstring first, apstring last, apvector <passenger> &flightlist )
{
	int n= flightlist.length();
	flightlist.resize(n+1);
	flightlist[n].first= first;
	flightlist[n].last= last;
	seat( flightlist[n], flightlist );
}

void removePassenger( apvector <passenger> &flightlist )
{
	// function will remove the passenger name entered
	int n= flightlist.length();
	int p;
	apstring first, last;

	cout << "\nPlease enter the name to remove: ";
	cin >> first >> last;
	first= cleanstr( first );
	last= cleanstr( last );

	cin.ignore( 80, '\n' );

	p= findPassenger( flightlist, first, last );
	if ( p == -1 )
	{
		cout << first << " " << last << " is not on this flight\n";
		return;
	}
	else
	{
		for( int i=p ; i<n-1 ; i++ )
			flightlist[i]= flightlist[i+1];
		flightlist.resize( n-1 );
		cout << first << " " << last << " removed from the flight\n";
	}
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
	name[0]= toupper( name[0] );
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

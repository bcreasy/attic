/*
	Brian Creasy
	APCS Period 9
	Final: Fly the Friendly Skies
	This program is a fully-functional flight record keeper.  It enables the
		user to display passengers, add/remove passengers, sort them, modify
		and eventually quit and re-write the data back to the data file.

	one warning: if this program does not end with the user choosing to quit,
	it DOES NOT SAVE THE DATA TO THE FILE.  i could think of no other way to
	fix this other than relying on the user to keep backups of their data :)
*/

#include <ctype.h>				// for to*() and is*()
#include <fstream.h>			// for file access
#include <iomanip.h>			// for setf and setw classes
#include <iostream.h>			// duh?
#include "include/apstring.h"	// for strings ( first and last names )
#include "include/apvector.h"	// for the actual database
#include "include/fileopen.h"	// for file reading and writing

const int MAXPASS= 40;

struct passenger {
	apstring first;
	apstring last;
	int row;
	char seat;
	bool firstclass;	// if they are in firstclass, firstclass == true; else
						// false
};

// begin lots and lots of prototypes
void Hello();
void GoodBye();
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
void seat( passenger &pass, apvector <passenger> &flightlist, bool &valid );
bool seatAvail( int row, char seat, apvector <passenger> &flightlist );
void addPassenger( apvector <passenger> &flightlist );
void addnew( apstring first, apstring last, apvector <passenger> &flightlist );
void removePassenger( apvector <passenger> &flightlist );
ostream &operator << ( ostream &outplace, passenger &pass );
// end prototypes

int main(void)
{
	apstring filename="passengers";
	apvector <passenger> flightlist( MAXPASS );

	Hello();

	loadFlightList( filename, flightlist );

	do{} while( !( mainmenu( flightlist, filename ) ) );

	GoodBye();

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

	cout << "There are " << i << " passengers currently on this flight\n";
	flightlist.resize( i );
	infile.close();		// closes the infile
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
		cout << "\nPlease enter a command (H to display menu): ";
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
	cout << "Q :: Quit the flight manager\n";
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
	// function will prompt for a passenger's name and display the information
	// for the passenger and will inform the user if the name entered is not on
	// the flight
	
	int p;
	apstring first, last;

	cout << "\nPlease enter a full name to display: ";
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
	// this function will print out the number of passengers in a
	// user-specified section of either 'firstclass' or 'coach'
	//
	// i thought this was a pretty nifty way of doing this *shrugs*

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
		cout << "There are no passengers flying in " << sect << endl;
	else
		cout << "There are " << count << " passengers flying in " << sect << endl;
}

void changeSeat( apvector <passenger> &flightlist )
{
	// this function will prompt for a name and then check to see if that
	// passenger does not exist.  if they do not exist, the user will be alerted
	// and offered to add the user entered to the list... if they do exist,
	// the user will be prompted for the new row and seat and seat() will be
	// called accordingly
	
	bool valid=true; // passed to seat() when adding a new user.  not used in
					 // this function
	int p;
	char ch;
	apstring first, last;

	cout << "\nWho would you like to change seats? (full name): ";
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
			cout << "Would you like to add " << first << " " << last << " to the flight?\n(y or n): ";
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
		cout << endl << flightlist[p].first << " " << flightlist[p].last << "'s current current assignment is:\n";
		cout << '\t' << " row #: " << flightlist[p].row << endl;
        cout << '\t' << "seat #: " << flightlist[p].seat << endl;
		seat( flightlist[p], flightlist, valid );
	}
}

void seat( passenger &pass, apvector <passenger> &flightlist, bool &valid )
{
	// this function will take a passenger passed into the function and ask the
	// what they want to change the seat to... if the seat they choose is
	// already taken, then the "valid" flag will be set to false and the user will
	// be alerted.  the valid flag is used so when adding a new person, it will
	// not continue to add the person if the seat the user chooses is already
	// taken: == very nasty bug that took a while to fix
	
	int row;
	char seat;
	bool avail;
	do
	{
		do
		{
			cout << "What row # would you like to assign " << pass.first << " " << pass.last << "? ";
			cin >> row;
			cin.ignore( 80, '\n' );
			if ( !(isdigit( row )) && ( row > 30 ) )
				cout << "Please enter an integer 30 or less (there are only 30 rows on the plane)\n\n";
		} while( !(isdigit( row )) && ( row > 30 ) );
		do
		{
			cout << "What seat letter would you like to assign " << pass.first << " " << pass.last << "? ";
			cin >> seat;
			cin.ignore( 80, '\n' );
			seat= toupper( seat );
			// begin "icky" comparisions ( i couldn't think of any other way to do it )
			if ( (!(isalpha( seat ))) || !( seat=='A' || seat=='B' || seat=='C' ||
						seat=='D' || seat=='E' || seat=='F' ) )
				cout << "Please enter a letter A thru F\n\n";
		} while( (!(isalpha( seat ))) || !( seat=='A' || seat=='B' || seat=='C' ||
						seat=='D' || seat=='E' || seat=='F' ) );
			// end "icky" comparisons
		avail= seatAvail( row, seat, flightlist);
		if ( avail ) // if the seat/row is not taken
		{
        	if ( row < 11 )
				pass.firstclass= true;
			else
				pass.firstclass= false;
			pass.row= row;
			pass.seat= seat;
		}
		else // the seat is already taken
		{
			cout << "Sorry, seat " << row << seat << " is already taken\n";
			valid= false;
		}
	} while ( avail= false );
}

bool seatAvail( int row, char seat, apvector <passenger> &flightlist )
{
	// this function will go through each passenger and see if the row and seat
	// passed into the function are already taken.  if available, return true;
	// else return false
	
	int n= flightlist.length();
	for( int i=0 ; i<n ; i++ )
		if ( flightlist[i].row == row && flightlist[i].seat == seat )
			return false;
	return true;
}

void addPassenger( apvector <passenger> &flightlist )
{
	// this function will prompt the user for a passenger to add to the flight
	// if the passenger is already on the flight, the user will be informed of
	// this and will be aborted
	
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
	// this is the main function for the adding of a new passenger. note the
	// valid boolean flag.  in seat() it is changed to false if the seat
	// assignment entered is already taken
	
	bool valid=true;
	int n= flightlist.length();
	flightlist.resize(n+1);
	flightlist[n].first= first;
	flightlist[n].last= last;
	seat( flightlist[n], flightlist, valid );
	if ( valid != true )
		flightlist.resize(n);
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
	// this is the insertion operator overloading function.  it allows the
	// client to print either to the screen or a file depending on how called
	// cout << passengertype; will print it to the screen
	// outfile << passengertype; will print it to a file
	// it also returns the stream so you can chain the operators
	
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
	// this function does very little but prints class for the screen output
	
	if ( cls == true )
		cout << "       First Class\n";
	else
		cout << "       Coach\n";
}

apstring cleanstr( apstring name )
{
	// ah.. this function is very important.
	// it allows each name (first and last) to remain uniform to let the
	// functions compare them with ease
	//
	// changes them to all lowercase and then changes the first character to
	// uppercase
	
	int i;
	int n= name.length();
	for( i=0 ; i<n ; i++ )
		name[i]= tolower( name[i] );
	name[0]= toupper( name[0] );
	return name;
}

void sort( apvector <passenger> &flightlist )
{
	// this function just does the main sorting stuff
	
	int i, min=0;
	int n= flightlist.length();
	for( i=0 ; i<n ; i++ )
		swap( flightlist[i], flightlist[ findmin( flightlist, i, min ) ] );
}

void swap( passenger &one, passenger &two )
{
	// swaps the first passenger passed with the second passenger passed
	
	passenger temp;

	temp= one;
	one= two;
	two= temp;
}

int findmin( apvector <passenger> &flightlist, int start, int &min )
{
	// finds the person with the "smallest" last name AND firstname if there
	// is more than one person with the same last name
	// returns the position in the apvector of this person
	
	int i;
	int n= flightlist.length();
	min= start;
	
	for( i=start+1 ; i<n ; i++ )
	{
		if ( flightlist[i].last < flightlist[min].last )
			min= i;
		if ( flightlist[i].last == flightlist[min].last ) // if more than one
										// person with the same last name exists
			if ( flightlist[i].first < flightlist[min].first )
				min= i;
	}
	return min;
}

void Hello()
{
	// this function greets the user in a polite manner
	cout << "Welcome to Flightlist++ For You++\n";
	cout << "\tcoded by: Brian Creasy\n\n";
}

void GoodBye()
{
	// this function will say goodbye to the user in a polite manner
	cout << "\nThank you for using Flightlist++ For You++\n";
}

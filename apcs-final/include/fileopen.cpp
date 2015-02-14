#include <iostream.h>
#include "apstring.h"
#include "fileopen.h"

void OpenFileForReading(ifstream &infile, apstring pathinName)
{
	do
	{
		infile.open(pathinName.c_str(), ios::in);
		if (infile.fail())
		{
			cout << "  INPUT FILE COULD NOT BE OPENED!\n";
			cout << "Enter name of INPUT file to read: ";
			infile.clear();
			getline(cin,pathinName);
		}
	} while (!infile.is_open());
}

void OpenFileForWriting(ofstream &outfile, apstring pathoutName)
{
	// NOTE: this is a destructive file open routine! If a file exists
	// in the current folder with this name, it will be erased
	// and replaced by the new file -- SELECT YOUR FILE NAMES WITH CARE.

	do
	{
		outfile.open(pathoutName.c_str(), ios::out | ios::trunc);
		if (outfile.fail())
		{
			cout << "  OUTPUT FILE COULD NOT BE OPENED!\n";
			cout << "Enter name of OUTPUT file to write: ";
			outfile.clear();
			getline(cin,pathoutName);
		}
	} while (!outfile.is_open());
}

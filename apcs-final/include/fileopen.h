#ifndef _FILE_OPEN_H
#define _FILE_OPEN_H

#include <fstream.h>
#include "apstring.h"

void OpenFileForReading(ifstream &infile, apstring pathinName);
void OpenFileForWriting(ofstream &outfile, apstring pathoutName);

#include "fileopen.cpp"
#endif
